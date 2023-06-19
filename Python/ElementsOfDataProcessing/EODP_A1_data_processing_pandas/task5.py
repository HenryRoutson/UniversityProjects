"""

Q "Why does “fake news” propagate much faster online 
(that is, why do people share much more fake news )?"

A It's more exciting, most news is boring

Q "Do people tend to share more accurate, credible news than less credible ones on social media?"

A
People only share the most outragous and exciting things,
which most news is not if you do your research and don't theorize.
However you also might avoiding sharing something which could be fake
as it could make you look unintelligent if you believed it.

"""


import json
import matplotlib.pyplot as plt


def n_article_tweets(tweet_dict):
    tweet_set = set()
    for tweet_type in tweet_dict.values():
        tweet_set.update(tweet_type)
    return len(tweet_set)


def task5():
    """
    Implement the function task5() in task5.py which outputs a file called
    task5.png comparing the average number of tweets for each article-rating group.
    For example, for an article with a rating of 0, how many tweets does it have on
    average? Note that the tweets here include both original tweets and retweets.
    To run your solution, open the terminal and run python main.py task5
    """


    directory = '/course/data/a1/'


    # read tweet data
    tweets_file = "engagements/HealthStory.json"
    with open(directory+tweets_file) as File: tweets_dict = json.load(File) 
    # tweets_dict = { news_id: {tweet type[tweet_id1, tweet_id2, ...], ...  }

    # read reviews data
    reviews_file = "reviews/HealthStory.json"
    with open(directory+reviews_file) as File: reviews_list = json.load(File)
    # reviews_list = [ {review}, ]



    # Calculate average number of tweets for each credibility ranking
    
    RATINGS = 6 # rating 0 .. 5 
    rating_sums = [0]*RATINGS
    rating_counts = [0]*RATINGS

    for review in reviews_list:

        tweets = tweets_dict[review["news_id"]]
        rating_sums[review["rating"]] += n_article_tweets(tweets)
        rating_counts[review["rating"]] += 1

    

    rating_averages = [rating_sums[i] / rating_counts[i] for i in range(RATINGS)] # y


    # matplotlib

    plt.scatter(list(range(RATINGS)), rating_averages) # x, y
    plt.xlabel('Credibility-rating out of 5')
    plt.ylabel('Average number of tweets')
    plt.savefig('task5.png')






task5()
