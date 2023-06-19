
import numpy as np
import pandas as pd

from sklearn.impute import KNNImputer
from sklearn.metrics import confusion_matrix
from sklearn.tree import DecisionTreeClassifier, export_text

from itertools import combinations

import matplotlib
import matplotlib.pyplot as plt

from random import shuffle

from collections import Counter


# -------------------------------------------------------------------------
# DATA WRANGLING
# -------------------------------------------------------------------------

OFFSET = ' '*50

# -------------------------------------------------------------------------

print(OFFSET, "merge region files into a single dataframe...")

reviews_files = ["EUmatch.csv", "KRmatch.csv", "NAmatch.csv"]
df_list = []
for file_name in reviews_files:
    with open('/course/data/a2/games/'+file_name) as File:
        df = pd.read_csv(File)
        df['region'] = file_name[:2]
        df_list.append(df)

df = pd.concat(df_list, ignore_index=True)





# -------------------------------------------------------------------------
# damage_objectives and damage_turrets should contain the same data
# fill in missing data and delete one of the two columns

print(OFFSET, "combining damage_objectives and damage_turrets...")

for i in range(len(df)):
    if pd.isnull(df.loc[i, 'damage_turrets']):  
        df.loc[i, 'damage_turrets'] =  df.loc[i, 'damage_objectives']

del df['damage_objectives']




# -------------------------------------------------------------------------
print(OFFSET, "changing side.red and side.blue to red and blue...")

for i in range(len(df)):
    if not pd.isnull(df.loc[i, 'side']):
        df.loc[i, 'side'] = df.loc[i, 'side'][5:]



# -------------------------------------------------------------------------
print(OFFSET, "changing spell integers to associated names...")

INT_TO_NAME = {
    21: "Barrier",
     1: "Cleanse",
    14: "Ignite",
     3: "Exhaust",
     4: "Flash",
     6: "Ghost",
     7: "Heal",
    13: "Clarity",
    30: "To the King!",
    31: "Poro Toss",
    11: "Smite",
    39: "Mark",
    32: "Mark",
    12: "Teleport",
    54: "Placeholder",
    55: "Placeholder and Attack-Smite"
}

df = df.replace({'d_spell': INT_TO_NAME, 'f_spell': INT_TO_NAME})



# -------------------------------------------------------------------------
# TopLane_Jungle can be split into two seperate roles
# if the entry has the spell smite, it falls under the role of jungle, else TopLane

print(OFFSET, "splitting role TopLane_Jungle...")

spell_cols = ['d_spell', 'f_spell']
for i in range(len(df)):

    if df.loc[i, 'role'] == 'TopLane_Jungle':

        if 'Smite' == df.loc[i, 'd_spell'] or 'Smite' == df.loc[i, 'f_spell']:
            df.loc[i, 'role'] = 'Jungle'
        else:
            df.loc[i, 'role'] = 'TopLane'



# -------------------------------------------------------------------------
# use kda function (multivariate strategy)

# define as a function to use multiple times
def apply_kda_formula(df):
    """

    Fill out values in dataframe with the formula and it's rearrangements,
    where there is only one data point missing:

        kda = ( kills + assists ) / (deaths + !deaths)
        kills = kda * (deaths + !deaths) - assists
        assists = kda * (deaths + !deaths) - kills
        (deaths + !deaths) = ( kills + assists ) / kda      

    Note:

        There are multiple possible values for death where (deaths + !deaths) = 1 
        (deaths = 0 or deaths =  1)
        but death 1 is more common

        counts = Counter(df['deaths'])
        print('0', counts[0]) # 0 396
        print('1', counts[1]) # 1 868


    Examples:

        kda        10.0
        kills       6.0
        assists     4.0
        deaths      0.0

        kda        11.0
        kills       7.0
        assists     4.0
        deaths      1.0

    """

    print(OFFSET, "using kda formula as multivariate strategy to fill in some nan values...")

    VAR_COLUMNS = ["kda", "kills", "assists", "deaths"]

    # for each row
    for i in range(len(df)):

        df_row = df.loc[i]
        num_missing_vars = sum(pd.isnull(df_row[col]) for col in df.columns if col in VAR_COLUMNS)
        if num_missing_vars == 1: 

            row = df.loc[i]
            deaths = row['deaths'] + (not bool(row['deaths']))

            # -----------------

            #kda = ( kills + assists ) / (deaths + !deaths)
            if pd.isnull(row['kda']):
                df.loc[i, 'kda'] = ( row['kills'] + row['assists'] ) / ( deaths )

            #kills = kda * (deaths + !deaths) - assists
            elif pd.isnull(row['kills']):
                
                df.loc[i, 'kills'] = row['kda'] * deaths - row['assists']

            #assists = kda * (deaths + !deaths) - kills
            elif pd.isnull(row['assists']):
                df.loc[i, 'assists'] = row['kda'] * deaths - row['kills']

            #(deaths + !deaths) = ( kills + assists ) / kda 
            elif pd.isnull(row['deaths']):

                num = row['kills'] + row['assists']
                if num == 0: 
                    df.loc[i, 'deaths'] = 1 # as 1 is more common
                else:
                    df.loc[i, 'deaths'] = num / row['kda']
                    
            # -----------------  

            # change -0 values to 0
            for col in VAR_COLUMNS:
                if df.loc[i, col] < 0:
                    neg = str(df.loc[i, col])[0] == '-'
                    if neg: df.loc[i, VAR_COLUMNS] = 0

    return df

df = apply_kda_formula(df)




# -------------------------------------------------------------------------
# Apply level scaling as if all columns were at level 18 to standardise

print(OFFSET, "scaling based on level...  (slow)")

ScaleDict = {
 6 : 7.65,
 7 : 5.77,
 8 : 4.52,
 9 : 3.64,
10 : 3.00,
11 : 2.52,
12 : 2.14,
13 : 1.84,
14 : 1.60,
15 : 1.41,
16 : 1.25,
17 : 1.11,
18 : 1 }

columns = ["assists", "damage_turrets", "damage_building", "deaths", "kda", "kills", "time_cc", "damage_taken", "vision_score", "damage_total","gold_earned"]


for i in range(len(df)):
    level = df.loc[i, "level"]
    if not pd.isnull(level): 
        df.loc[i, columns] *= ScaleDict[int(level)]
        #df["level"] = 18.0 #
del df["level"] #

# -----------------------------





# -------------------------------------------------------------------------
# DATA IMPUTATION
# -------------------------------------------------------------------------

def impute(df):

    df = df.reset_index()

    # MAKE DEFINITION

    KDA_COLS = ['kills', 'assists', 'deaths']
    OTHR_COLS = ['damage_building', 'damage_turrets', 'time_cc', 'damage_total', 'gold_earned'] 
    ALL_COLS = KDA_COLS + OTHR_COLS

    imputer = KNNImputer(n_neighbors=3)
    imputed_array = imputer.fit_transform(df[ALL_COLS])
    imputed_df = pd.DataFrame(imputed_array, columns = ALL_COLS)

    df.loc[:, OTHR_COLS] = imputed_df.loc[:, OTHR_COLS] # do not update KDA columns


    # -------------------------------------------------------------------------
    # assign imputed predictions to missing values in KDA columns
    # and make predictions follow KDA formula

    for col in KDA_COLS:

        df.loc[:, col] = imputed_df[col]
        print(OFFSET, "predicting missing entries...")
        df = apply_kda_formula(df)

    return df

# -----------------------------



# -------------------------------------------------------------------------
# DATA VISUALIZATION
# -------------------------------------------------------------------------


def avg(x): return sum(x) / len(x)

def visualise(df):

    print()
    print("Column correlations")
    print(df.corr())
    print()

    print()
    print(OFFSET, "saving plots...")

    # ---------------------------------
    print(OFFSET, " "*5 + "- champion average stats bar graphs")


    champions = list(set(df['champion']) - {np.nan})

    for col in df.columns:
        if col != 'champion' and type(df.loc[0, col]) != str:

            avgs = [ ( champion, avg(df.loc[df['champion'] == champion, col].dropna()))  for champion in champions ]
            avgs = pd.DataFrame(avgs)

            avgs = avgs.sort_values(by = 1)

            plt.figure(figsize=(10,40))

            plt.barh(avgs[0], avgs[1])

            plt.xlabel(col)
            plt.ylabel('champions')
            plt.title('champion '+col+' distribution', fontsize = 20)
            
            save_location = 'bar_graphs/champions_' + col + '.png'
            plt.savefig(save_location)                
            plt.clf() # clear figure


    # ---------------------------------

    ALPHA = 0.05
    DOT_SIZE = 12

    # ---------------------------------
    print(OFFSET, " "*5 + "- color scatter graphs")

    points_colours = ['#00ff00', '#ff0000', '#0000ff', '#d62728', '#9467bd', '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf']
    
    # graphs are automatically generated based on color_graphs_list
    # the color_graphs_list contains lists of 
    # [x_axis_col_name, y_axis_col_name, color_col_name]

    color_graphs_list = [] # manually specify graphs

    #"""
    # this code generates combinations of graphs
    color_cols = ['role'] # ['role'] or ['f_spell', 'd_spell']    (runs out of space on device if you create too many graphs)
    for color_col in color_cols: 
        for x_y_cols in combinations(df.columns, 2):
            if color_col not in x_y_cols:
                color_graphs_list.append([x_y_cols[0], x_y_cols[1], color_col])
    #"""

    for col_names in color_graphs_list:

        cols = df[col_names].dropna().reset_index()
        x_col_name, y_col_name, color_col_name = col_names
        color_col = list(cols[color_col_name])
        axes_names = x_col_name + '-' + y_col_name

        plt.figure(figsize=(9.8,7.2))

        if type(color_col[0]) == str:

            levels, categories = pd.factorize(cols[color_col_name])
            color_col = [points_colours[i] for i in levels]
            handles = [matplotlib.patches.Patch(color = points_colours[i], label=c) for i, c in enumerate(categories)]
            plt.legend(handles=handles, title=color_col_name)
            plt.scatter(cols[x_col_name], cols[y_col_name], alpha = ALPHA, c = color_col, s = DOT_SIZE)

        else:

            plt.scatter(cols[x_col_name], cols[y_col_name], alpha = ALPHA, c = color_col, s = DOT_SIZE)
            clb = plt.colorbar()
            clb.ax.set_title(color_col_name)

        plt.xticks(rotation = 90)
        plt.title(x_col_name + " vs " + y_col_name, fontsize=18)
        plt.xlabel(x_col_name)
        plt.ylabel(y_col_name)

        plt.subplots_adjust(left = 0.2, bottom = 0.3)

        save_location = 'color_graphs/' + axes_names + '-' + color_col_name +'.png'
        plt.savefig(save_location)
        plt.clf() # clear figure
        plt.close()

    # -----------------------------

visualise(df)




# -------------------------------------------------------------------------
# SUPERVISED LEARNING / DECISION TREE
# https://www.geeksforgeeks.org/decision-tree-implementation-python/
# -------------------------------------------------------------------------


INPUT_COLS = ['kills', 'deaths', 'assists', 'gold_earned', 'damage_total', 'damage_taken', 'vision_score', 'time_cc', 'damage_building', 'turret_kills']
OUTPUT_COLS = ['d_spell', 'f_spell', 'role', 'champion']
TEST_PERCENT = 0.2


# -------------------------------------------------------------------------
# DATA SPLITTING
print(OFFSET, "splitting data...")

df_indexes = list(range(len(df)))
shuffle(df_indexes)

i = int(len(df) * TEST_PERCENT) # i is the index to split on
train_df = impute(df.loc[df_indexes[i+1:], :]).dropna()
test_df  = df.loc[df_indexes[:i], :].dropna()
del df


# ------------------------------------------------------------------------- 

"""
# Over-Sampling
# to address dataset imbalances of categorical variables 
# https://www.analyticsvidhya.com/blog/2017/03/imbalanced-data-classification/#h2_4



print(OFFSET, "performing over-Sampling...")


for output_col in OUTPUT_COLS:

    col_categories = list(set(train_df[output_col]))
    counts = {category : list(train_df[output_col]).count(category) for category in col_categories}
    max_count = max(counts.values())

    for category in col_categories:
        
        rows_with_category = train_df[train_df[output_col] == category]
        duplics = int(max_count / counts[category]) - 1 # -1 because original is still in df
        train_df = pd.concat([train_df] + [rows_with_category] * duplics, ignore_index = True)


print("Num datapoints: ", len(train_df))
"""

# -------------------------------------------------------------------------

Input_train = train_df[INPUT_COLS]
Input_test = test_df[INPUT_COLS]

for output_col in OUTPUT_COLS:

    # -------------------------------------------------------------------------
    # DECISION TREE

    print('\n'*3)
    print(output_col + " decision tree")
    print()

    Output_train = train_df[output_col]


    clf = DecisionTreeClassifier(max_depth = 3) # low max_depth to keep interpretable
    clf.fit(Input_train, Output_train)
    
    tree_text = export_text(clf, feature_names = list(Input_test.columns))
    print(tree_text)


    # -------------------------------------------------------------------------
    # EVALUATION

    Output_test = test_df[output_col]

    print("Percent of correct predictions")
    num_total = len(Output_test)
    pred_Output_test = clf.predict(Input_test)
    num_correct = sum(list(Output_test)[i] == list(pred_Output_test)[i] for i in range(num_total))
    print(num_correct / num_total)
    print()


    # -------------------------------------------------------------------------



    confus = confusion_matrix(Output_test, pred_Output_test, normalize = 'true')
    confus_cols = sorted(list(set(Output_test)))
    confus = pd.DataFrame(confus, columns = confus_cols)
    confus['*Actual*'] = confus_cols
    confus = confus.set_index(keys = '*Actual*')

    print("Confusion Matrix:")
    print("*Predicted*")
    print(confus)


# -----------------------------





######
print("\n\ndone")
