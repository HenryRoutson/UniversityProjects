import os
import json
import datetime
import pandas as pd
from collections import Counter
import matplotlib.pyplot as plt

def task3():
    """
    Each news article comes with a publish_date field, which specifies when the
    article was published. The field is in the millisecond precision format, which
    is a floating point number. Convert it into a more readable format using the
    datetime.datetime.fromtimestamp() function. It should return you an object of type datetime, and you will need to consult the documentation to extract
    the year, month and date of the article.
    """

    # " Also note that you will likely write helper functions "
    #    where???

    directory = '/course/data/a1/'



    """ PART A

    Extract the year, month, and day components from the publish_date of
    each article, and output a csv file called task3a.csv, which contains the
    following headings: news_id, year, month, day. Each row should contain
    the ID of an article in the format story_reviews_xxxxx, and the year,
    month, day on which it was published. The formats for year, month,
    and day are 4-digit year, 2-digit month, and 2-digit day of the month. For
    example, for the date 17 April 2022, the year, month, and day components
    are 2022, 04, and 17 respectively.
    The rows in task3a.csv should be in ascending order of news_id.

    There might be articles for which the publish date is unspecified; exclude
    those articles in your output.
    """ 

    # create seperate lists to store data
    news_id_list = []
    date_list = []
    
    articles_folder = "content/HealthStory/"
    articleIDs = os.listdir(directory+articles_folder) # get list of files
     
    for articleID in articleIDs: # to iterate over

        with open(directory+articles_folder+articleID) as File: # open each files to read data
            article_dict = json.load(File) # and convert json file to workable python type

        date_published = article_dict["publish_date"]

        if date_published:
            date_published = datetime.datetime.fromtimestamp(date_published)

            # write to lists
            
            articleID = articleID[:-len(".json")] # remove .json file extension
            news_id_list.append(articleID) 
            date_list.append(date_published)


    year_list = [date.year for date in date_list] # store for later

    data = {
        "news_id": news_id_list,
        "year": year_list,
        "month": [date.month for date in date_list],
        "day": [date.day for date in date_list]
    }

    del date_list

    # pandas

    df = pd.DataFrame(data)
    df = df.set_index('news_id')
    df = df.sort_values(by=['news_id'])
    df.to_csv("task3a.csv") # write to csv file

    del df # remove from memory
 


    """ PART B
    
    Count the number of articles in each calendar year in the dataset, and
    output a file called task3b.png, describing the yearly article counts using
    an appropriately chosen graph.
    """

    # pandas
    
    year_count = Counter(year_list)
    sr = pd.Series(year_count) 
    sr = sr.sort_index()

    # matplotlib

    sr.plot()
    plt.xlabel('Year')
    plt.ylabel('Number of articles')
    plt.savefig('task3b.png')



task3()

