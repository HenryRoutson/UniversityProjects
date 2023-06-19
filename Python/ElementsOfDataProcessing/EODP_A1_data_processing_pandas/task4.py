import pandas as pd
import json
import matplotlib.pyplot as plt


def task4():
    """
    Are news article published by The New York Times more credible than those
    by Fox News, according to the judgement of experts? Recall that each article is
    given an overall rating, which is a score between 0 and 5 (inclusive) indicating
    the credibility of the information presented in the article. In this task, you will
    compare the average rating of articles published by each news source.
    In each review, you will see a news_source field, which tells you the where
    the article was published. 
    """

    directory = '/course/data/a1/'


    """ TASK A
    
    Output a csv file called task4a.csv, which contains the following headings: news_source, num_articles, avg_rating. Each row should contain
    details about one news source.
    The rows in task4a.csv in ascending order of news_source.
    Some reviews leave this field as blank (an empty string), in which case you can exclude them.
    """

    sources_count_and_rating = {} # news_source: [count, rating_sum]

    # read reviews data
    reviews_file = "reviews/HealthStory.json"
    with open(directory+reviews_file) as json_file: review_list = json.load(json_file)

    for review in review_list:

        news_source = review['news_source']
        if not news_source: continue # exclude blanks
        if news_source not in sources_count_and_rating: 
            sources_count_and_rating[news_source] = [0, 0] # create key:value if none

        sources_count_and_rating[news_source][0] += 1 # count
        sources_count_and_rating[news_source][1] += review['rating'] # rating sum

    del review_list # remove from memory


    # change dict to              news_source: [count, average_rating]
    for news_source in sources_count_and_rating:
        # average_rating = rating_sum / count
        sources_count_and_rating[news_source][1] /= sources_count_and_rating[news_source][0] 


    data = {
        "news_source": sources_count_and_rating.keys(), 
        "num_articles": (x[0] for x in sources_count_and_rating.values()), # count
        "avg_rating":   (x[1] for x in sources_count_and_rating.values())  # average_rating
    }


    df = pd.DataFrame(data)
    df = df.sort_values(by=['news_source'])
    df.set_index('news_source').to_csv("task4a.csv")







    """ TASK B

    Output a file called task4b.png comparing the average ratings of all news
    sources, that have at least 5 articles. Choose an appropriate plot for this
    task. Also, sort the axes so that the most and least credible news sources
    are easily detected.
    """

    df = df[df.num_articles >= 5]
    df = df.sort_values(by=['avg_rating'])

    x = list(df['news_source']) # list might help plt avoid getting messed up
    y = list(df['avg_rating'])

    # matplot lib
    plt.scatter(x, y)
    plt.xticks(rotation= 90)
    plt.subplots_adjust(bottom=0.5)
    plt.xlabel('source')
    plt.ylabel('Average credibility out of 5')
    plt.savefig('task4b.png')









task4()

