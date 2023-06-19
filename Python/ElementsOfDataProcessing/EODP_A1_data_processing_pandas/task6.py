# https://edstem.org/au/courses/7278/discussion/new



from collections import defaultdict
from os import listdir
from json import dumps, load
from re import sub
from nltk.corpus import stopwords

def task6():
    """
    News articles often have some characteristics that make them difficult to process;
    for example, they might contain unicode characters and even emojis. To make
    our analysis easier, in this task you will perform some preprocessing on the
    articles.
    Implement the function task6() in task6.py that performs the following
    pre-processing steps on the content of the news articles: The content of an
    article is in the text field.

    1. Convert all non-alphabetic characters (for example, numbers, apostrophes
    and punctuation), except for spacing characters (for example, whitespaces,
    tabs and newlines) to single-space characters. For example, ‘&’ should be
    converted to ‘ ’.

    2. Convert all spacing characters such as tabs and newlines into single-space
    characters, and ensure that only one whitespace character exists between
    each token.

    3. Change all uppercase characters to lowercase.

    4. Tokenise the resulting string into a list of words, using the space delimiter.

    5. Remove all stop words in nltk’s list of English stop words from the resulting list. You may find the function filter in Python useful, but this
    is not necessary.

    6. Remove all remaining words that are only a single character long from the
    resulting list.

    7. Once steps 1 – 6 are done, build a vocabulary of distinct words as a JSON
    object and output it to a file called task6.json. Each key is a word and
    the value is an array of the news_id of articles containing that word. The
    entries in task6.json should be in ascending order of word and the list of
    articles for each word should also be in ascending order of news_id. The
    format of each key-value pair of the JSON object should be
    "word": ["story_reviews_xxxxx", "story_reviews_yyyyy"]

    8. The creation of vocabulary should be implemented reasonably efficiently.
    The run time of task 6 should be no more than 45 seconds. Excessively
    long execution time for this task will result in a deduction of 1 mark. For
    example, using the re package is likely to be more efficient than writing
    for loops.
    
    Note that you might want to use an output from a previous task for convenience, but this is not required.

    """

    word_dic = defaultdict(list)
    stop_words_set = set(stopwords.words('english'))

    articles_full_path = r'/course/data/a1/content/HealthStory/'
    article_list = listdir(articles_full_path)
    for article in article_list:

        with open(articles_full_path + article) as File: 
            reviews_file = load(File) # convert json file

        text = reviews_file["text"]

        # 1
        text = sub(r'[^a-zA-Z\s]', ' ', text)  # returns string with matches swapped for sub

        # 2
        text = sub(r'\s+', ' ', text)

        # 3
        text = text.lower()

        # 4
        text = text.split()

        # 5
        text = set(text) - stop_words_set  # sets are faster than lists

        # 6
        for word in list(text): # create copy by converting to list
            if len(word) == 1: 
                text.discard(word)

        # 7
        article_name = article[:-5]
        for word in text: 
            word_dic[word].append(article_name)


            
    # sort keys and values

    sorted_word_dic = {}
    for key in sorted(word_dic): 
        sorted_word_dic[key] = sorted(word_dic[key])
    del word_dic

    # write to file

    with open("task6.json",'w') as File:
        File.write(dumps(sorted_word_dic)) # (json.dumps) 
   
    






task6()

