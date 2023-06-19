import json
import pandas as pd
import os
import math
import matplotlib.pyplot as plt



def task7():
    """
    Task 7. Detecting most indicative words of fake news (4 marks)
    What is fake news? What is real news?
    """

    directory = '/course/data/a1/'

    # load reviews
    reviews_file = "reviews/HealthStory.json"
    with open(directory+reviews_file) as File:
        review_list = json.load(File)

    # change review_list to a dict
    review_dict = {review['news_id']: review for review in review_list}
    del review_list
    
    num_real_articles = sum(review_dict[review_key]['rating'] >= 3 for review_key in review_dict)
    num_fake_articles = len(review_dict) - num_real_articles
        
    
    # load task6

    word_ratio_dic = {} # {word: word_log_odds_ratio}

    with open("task6.json") as json_file: word_articles_dic = json.load(json_file)
    assert word_articles_dic, "task6.json file is missing"

    for word in word_articles_dic:

        num_real_articles_containing_word = 0
        for article_name in word_articles_dic[word]:
            is_real = review_dict[article_name]['rating'] >= 3
            num_real_articles_containing_word += is_real

        num_fake_articles_containing_word = len(word_articles_dic[word]) - num_real_articles_containing_word

        prw = num_real_articles_containing_word / num_real_articles
        if prw == 1 or prw == 0: continue
        orw = prw / (1 - prw)

        pfw = num_fake_articles_containing_word / num_fake_articles
        if pfw == 1 or pfw == 0: continue
        ofw = pfw / (1 - pfw)
        
        odds = ofw / orw
        lorw = math.log10(odds)

        word_ratio_dic[word] = round(lorw, 5)





    """ PART A

    • Output a csv file called task7a.csv with the following headings: word,
    log_odds_ratio. Each row should represent a word in the vocabulary and
    the log odds ratio for fake news of that word. The value of log_odds_ratio
    is to be rounded to 5 digit from the decimal point. The entries in task7a.csv
    should be in ascending order of word.
    """

    data = {
        "word": word_ratio_dic.keys(),
        "log_odds_ratio": word_ratio_dic.values()
    }
        
    df = pd.DataFrame(data)
    df = df.sort_values(by='word', ascending = True)
    df.set_index('word').to_csv("task7a.csv")
    






    """ PART B
    
    Output a file called task7b.png which contains an appropriately chosen
    graph, showing the distribution of the log odds ratios for all words.
    """

    df = df.sort_values(by='log_odds_ratio')
    plt.hist(df['log_odds_ratio']) 
    plt.xlabel("long odds ratios")
    plt.ylabel("frequency")
    plt.savefig('task7b.png')






    """ PART C

    Output a file called task7c.png which contains an appropriately chosen
    graph, showing the top 15 words with the highest odds ratios for fake
    news, and the top 15 words with the lowest odds ratios. 
    """

    degrees_angle = 80
    extend_below = 0.3

    
    # highest odds ratios

    plt.subplot(1, 2, 1)

    plt.plot(df["word"][:15], df["log_odds_ratio"][:15])
    plt.title("Lowest")

    plt.xlabel("words")
    plt.ylabel("long odds ratio")
    plt.xticks(rotation = degrees_angle) 
    plt.subplots_adjust(bottom= extend_below)

    # lowest odds ratios

    plt.subplot(1, 2, 2)

    plt.plot(df["word"][-15:], df["log_odds_ratio"][-15:])
    plt.title("Highest")

    plt.xlabel("words")
    plt.xticks(rotation = degrees_angle) 
    plt.subplots_adjust(bottom= extend_below)


    # save 
    plt.savefig('task7c.png')








   
task7()

