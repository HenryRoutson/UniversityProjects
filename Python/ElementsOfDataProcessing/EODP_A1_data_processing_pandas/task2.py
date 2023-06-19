import pandas as pd
import os
import json


def task2():
    """
    In each review, you will see a news_id field containing the ID of the article.
    This information allows you to match the review with the news article.
    Implement the function task2() in task2.py which combines the articles
    with their reviews to work out how many “satisfactory” ratings that article
    receives, out of 10 criteria in total. Your function should save its output to
    a csv file called task2.csv, which contains the following headings: news_id,
    news_title, review_title, rating, num_satisfactory. Each row in the file
    should contain the details of one article, with
    • news_id being the ID of the news article in the format story_reviews_xxxxx;
    • news_title being the title of the news article;
    • review_title being the title of the review article;
    • rating being the overall rating of the article (between 0 and 5); and
    • num_satisfactory being the total number of criteria (between 0 and 10)
    that are satisfactory.
    The rows in task2.csv should be in ascending order of news_id.
    """

    directory = '/course/data/a1/'

    # read reviews data
    reviews_file = "reviews/HealthStory.json"
    with open(directory+reviews_file) as json_file: review_list = json.load(json_file)


    # create complex data

    num_satisfactory_list = []  
    for review in review_list:
        # count number of satisfactory criteria
        n_satisfactory = sum(q_and_a["answer"] == "Satisfactory" for q_and_a in review["criteria"])
        assert n_satisfactory <= 10
        num_satisfactory_list.append(n_satisfactory)
    

    # create other data with list comprehension

    data = {
        "news_id": [review["news_id"] for review in review_list],
        "news_title": [review["original_title"] for review in review_list],
        "review_title": [review["title"] for review in review_list],
        "rating": [review["rating"] for review in review_list],
        "num_satisfactory": num_satisfactory_list
    }

    print(data)

    del review_list # remove from memory


    # pandas

    df = pd.DataFrame(data)
    df = df.set_index('news_id')
    df = df.sort_values(by=['news_id'])
    df.to_csv("task2.csv") # save csv file



task2()

