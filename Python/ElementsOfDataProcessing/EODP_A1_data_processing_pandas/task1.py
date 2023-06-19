import json
import os
import pandas as pd


def task1():
    """
    Implement the function task1() in task1.py that outputs a json file called
    task1.json in the following format:
    {
    "Total number of articles": X,
    "Total number of reviews": Y,
    "Total number of tweets": Z
    }
    where X, Y and Z are the number of news articles, number of reviews, and number
    of tweets respectively.
    """

    directory = '/course/data/a1/' # main directory 


    # get number of articles --------
    articles_folder = "content/HealthStory/"
    list_articles = os.listdir(directory+articles_folder)
    n_articles = len(list_articles) # https://stackoverflow.com/questions/2632205/how-to-count-the-number-of-files-in-a-directory-using-python


    # get number of reviews --------
    reviews_file = "reviews/HealthStory.json"
    with open(directory+reviews_file) as File: reviews_file = json.load(File)
    n_reviews = len(reviews_file)
    del reviews_file # remove from memory



    # get number of tweets --------
    tweets_file = "engagements/HealthStory.json"
    with open(directory+tweets_file) as File: tweets_dict = json.load(File)

    all_tweets = set() # use a set to remove duplicates
    for article_tweet_list in tweets_dict.values():
        for tweet_list in article_tweet_list.values(): # iterate through each type of tweet, which has a different tweet list
            all_tweets.update(tweet_list) # update adds all elements in a list to a set

    n_tweets = len(all_tweets)
    del tweets_dict, all_tweets # remove from memory



    # create dictionary and write to json 
    data = {
        "Total number of articles": n_articles,
        "Total number of reviews": n_reviews,
        "Total number of tweets": n_tweets
    }
    
    sr = pd.Series(data)
    sr.to_json("task1.json")





task1()
