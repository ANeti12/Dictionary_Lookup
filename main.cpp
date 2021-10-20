#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <math.h>
using namespace std;

void displayInstructions() //displays instructions at beginning and after each option
{
    cout <<
    "Select a menu option:" << endl <<
    "1. Spell commonly misspelled words (timed)" << endl <<
    "2. Type random words from full dictionary (timed)" << endl <<
    "3. Display some words from one of the lists" << endl <<
    "4. Use binary search to lookup a word in full dictionary" << endl <<
    "5. Set word length to use with full dictionary" << endl <<
    "6. Exit the program" << endl <<
    "Your choice --> ";
} //end of displayInstructions()

void misspelledWordsVector(vector<string>& misspelledWords, vector<string>& correctSpelledWords) //transfers words from misspelledWords.txt into two vectors: misspelled and correct
{
    ifstream misspelledWordsFile ("misspelledWords.txt"); //ifstream variable for text file
    string misspelledWord; //variable for misspelled words
    string correctSpelledWord; //variable for correct words
    if (misspelledWordsFile.is_open())
    {
        while (misspelledWordsFile >> misspelledWord >> correctSpelledWord) //file is structured as "misspelled correct", so >> operator will assign misspelled word to first variable and correct word to second variable
            {
            misspelledWords.push_back(misspelledWord); //appends misspelled word to misspelledWords vector
            correctSpelledWords.push_back(correctSpelledWord); //appends correct word to correctSpelledWords vector
            }
    }
} //end of misspelledWordsVector()

void dictionaryVector(vector<string>& dictionary) //transfers words from dictionary.txt to dictionary vector
{
    ifstream dictionaryFile ("dictionary.txt"); //ifstream variable for text file
    string dictionaryEntry; //variable for dictionary entries (assumed to be all correct)
    while (dictionaryFile >> dictionaryEntry) //file is structured as "correct endl" so only one >> operator is needed
        {
        dictionary.push_back(dictionaryEntry); //appends dictionary word to dictionary vector
        }
} //end of dictionaryVector()

void spellMisspelledWords(vector<string>& correct, vector<string>& incorrect, int& scoreTemp) //option 1, spelling correct form of misspelled words, score updated
{
    vector<string> words; //all user input, regardless of correct or incorrect, stored here
    string word; //variable to store values in words vector
    int iSize = incorrect.size();
    vector<int> wrongWordIndex; //vector to store the index of where the incorrect word was from in words vector, used later in function
    vector<string> validationWords; //vector to store the correct version of all the incorrect words, used to compare words.at(i) with validationWords.at(i)
    cout << "Using commonly misspelled words" << endl;
    for (int i = 0; i < 5; i++)
    {
        int random = rand() % iSize; //generates random integer in range of size of incorrect vector
        cout << incorrect.at(random) << " "; //prints out value the at the index of random in incorrect vector
        validationWords.push_back(correct.at(random)); //appends to validationWords the word at the index of random in correct vector
    }
    cout << endl << "Type in those words within goal of 15 seconds:" << endl;
    time_t start = time(NULL); //starts the timer here
    bool flag = true; //boolean condition for do-while loop
    do
    {
        for (int i = 0; i < 5; i++) //will execute the loop five times for five words to be inputted
            {
            cin >> word;
            words.push_back(word);
            }
        if (words.size() == 5) //will wait until 5 words have been inputted and appended to words vector before boolean condition is false
            {
            flag = false;
            }
    } while (flag);
    time_t end = time(NULL); //ends the timner here
    double time = end - start; //takes the difference between start and end time, used for scoring later
    vector<string> incorrectWords; //vector to store the incorrect words from user input
    for (int i = 0; i < words.size(); i++) //will execute the loop for the size of words vector
        {
        string wordCompare = words.at(i);
        string valid = validationWords.at(i);
        if (wordCompare != valid) //word at i is not the same as validation word at i, aka incorrect, then if loop executes
            {
            incorrectWords.push_back(words.at(i)); //appends word to incorrectWords vector if incorrect
            wrongWordIndex.push_back(i); //appends the index of where wrong word was found
            }
        }
    int mistakeCounter = 0; //variable for how many mistakes were found
    for (int i = 0; i < incorrectWords.size(); i++) //as incorrect words vector is all incorrect, mistake counter will be whatever the size of incorrectwords vector is
        {
        mistakeCounter++;
        }
    if (mistakeCounter == 0) //if no mistakes were found, then display no misspelled words
        {
        cout << "No misspelled words!" << endl;
        }
    else
    {
        cout << "Misspelled words: " << endl;
        for (int i = 0; i < incorrectWords.size(); i++) //for the words in incorrectwords, incorrectwords.at(i) will be printed following the validationwords at wrongwordindex at i, to print the proper spelling of the word
            {
            cout << "   " << incorrectWords.at(i) << " should be: " << validationWords.at(wrongWordIndex.at(i)) << endl;
            }
    }
    double timeScoring; //scoring from what the time was, calculated above as end - start
    if (time <= 15) //if less than or equal to 15 seconds, then points are awarded based on formula below
        {
        timeScoring = 2 * (15 - time);
        cout << "Elapsed time: " << time << " seconds." << " " << timeScoring << " point bonus for finishing " << 15 - time << " seconds early." << endl;
        }
    else if (time > 15) //if greater than 15 seconds, then points are taken away based on formula below
        {
        timeScoring = (time - 15.0) * -3.0;
        cout << "Elapsed time: " << time << " seconds." << " " << timeScoring << " points for finishing " << time - 15 << " seconds late." << endl;
        }
    int spelledCorrectlyPoint = (5 - mistakeCounter) * 3; //points for words spelled correctly
    cout << spelledCorrectlyPoint << " points: " << 5 - mistakeCounter << " spelled correctly x 3 points each" << endl;
    int penalty = mistakeCounter * 6; //points taken away for misspelling words, but only applicable if penalty is greater than 0
    if (penalty > 0)
    {
        cout << penalty << " point penalty: " << mistakeCounter << " spelled incorrectly x 6 points each" << endl;
    }
    int score = timeScoring + spelledCorrectlyPoint - penalty; //calculating total score
    if (score > scoreTemp) //if score is greater than reference parameter scoreTemp, then scoreTemp is updated to be score for best score feature
        {
        scoreTemp = score;
        }
    cout << "Score: " << score << endl;
} //spellMisspelledWords()

void randomWordsFromDictionary(vector<string>& dictionary, int& wordLength, int& scoreTemp) //displays random words from dictionary vector, user tries to spell words correctly
{
    vector<string> words; //vector to store user input, regardless of correct or incorrect
    string word; //variable to pass in words to words vector
    int dSize = dictionary.size();
    vector<int> wrongWordIndex; //vector to store index of incorrect words
    vector<string> correctWords; //vector to store correct words
    vector<string> validationWords; //vector to store specific correct words when incorrect word is found
    cout << "Using full dictionary" << endl;
    while (correctWords.size() < 5) //will execute until 5 words have been generated
        {
        int random = rand() % dSize; //generates words
        if (dictionary.at(random).length() == wordLength) //checks if length of random word is the same as wordlength
            {
            cout << dictionary.at(random) << " ";
            correctWords.push_back(dictionary.at(random)); //appends random word to correctwords
            }
        }
    cout << endl << "Type in those words within goal of 15 seconds:" << endl;
    time_t start = time(NULL); //start of timer here
    bool flag = true; //boolean condition for do-while loop
    while (flag)
    {
        for (int i = 0; i < 5; i++) //loop will execute 5 times, word will be added to words
            {
            cin >> word;
            words.push_back(word);
            }
        if (words.size() == 5) //while loop will stop once 5 words have been inputted
            {
            flag = false;
            }
    }
    time_t end = time(NULL); //end of timer here
    double time = end - start; //calculating time elapsed, used later for scoring
    vector<string> incorrectWords; //vector to store all the incorrect words
    for (int i = 0; i < words.size(); i++) //executing for however many words are in the words vector
        {
        if (words.at(i).compare(correctWords.at(i)) != 0) // if words at i is not equal to the correct words, add word to incorrect words and also add the correct spelling to validation words vector
            {
            incorrectWords.push_back(words.at(i));
            validationWords.push_back(correctWords.at(i));
            }
        }
    int mistakeCounter = 0;
    for (int i = 0; i < incorrectWords.size(); i++) //mistake counter will be the size of incorrect words vector
        {
        mistakeCounter++;
        }
    if (mistakeCounter == 0) // if no mistakes were found, aka incorrect vector size is 0, then print no misspelled words
        {
        cout << "No misspelled words!" << endl;
        }
    else
    {
        cout << "Misspelled words:" << endl;
        for (int i = 0; i < incorrectWords.size(); i++) // for incorrect words size, incorrect word will be displayed along with the correct spelling
            {
            cout << "   "<< incorrectWords.at(i) << " should be: " << validationWords.at(i) << endl;
            }
    }
    double timeScoring; //scoring variable given time
    if (time <= 15) //if time is less than or equal to 15, then points are awarded
        {
        timeScoring = 2 * (15 - time);
        cout << "Elapsed time: " << time << " seconds." << " " << timeScoring << " point bonus for finishing " << 15 - time << " seconds early." << endl;
        }
    else if (time > 15) //if time is greater than 15 seconds, then points are taken away
        {
        timeScoring = (time - 15.0) * -3.0;
        cout << "Elapsed time: " << time << " seconds." << " " << timeScoring << " points for finishing " << time - 15 << " seconds late." << endl;
        }
    int spelledCorrectly = (5 - mistakeCounter) * (wordLength - 4);
    cout << spelledCorrectly << " points: " << 5 - mistakeCounter << " spelled correctly x " << wordLength - 4 <<  " points each" << endl;
    int penalty = mistakeCounter * ((wordLength -  4) * 2); //penalty for mispelling words, but only applicable if mistakecounter > 0
    if (penalty > 0)
    {
        cout << penalty << " point penalty: " << mistakeCounter << " spelled incorrectly x " <<
        ((wordLength -  4) * 2) << " points each" << endl;
    }
    int score = timeScoring + spelledCorrectly - penalty;
    if (score > scoreTemp) //compares score with reference parameter scoreTemp, if score > scoreTemp then scoreTemp is updated as score
        {
        scoreTemp = score;
        }
    cout << "Score: " << score << endl;
    wordLength++; //increases wordLength after each time the function executes
} //end of randomWordsFromDictionary()

void displaySomeWords(vector<string>& dictionary, vector<string>& correct, vector<string>& incorrect) //Option 3, displays some words from any of the three vectors
{
    char option; //option A, B, or C
    int start; //index for start
    int end; //index for end
    cout << "Displaying word list entries." << endl
    << "Which words do you want to display?" << endl
    << "      A. Dictionary of all words\n"
       "      B. Wrongly spelled common misspelled words\n"
       "      C. Correctly spelled common misspelled words\n"
       "Your choice --> "<< endl;
    cin >> option;
    option = toupper(option); //changes all input to uppercase for simplicity
    if (option == 'A') //option for dictionary of all words, takes indices and displays all words in that range
        {
        cout << "Enter the start and end indices between 0 and 263532: ";
        cin >> start;
        cin >> end;
        for (int i = start; i <= end; i++)
        {
            cout << i << ". " << dictionary.at(i) << endl;
        }
        cout << endl;
        }
    else if (option == 'B') //option for misspelled words, takes indices and displays all words in that range
        {
        cout << "Enter the start and end indices between 0 and 113: ";
        cin >> start;
        cin >> end;
        for (int i = start; i <= end; i++)
        {
            cout << i << ". " << incorrect.at(i) << endl;
        }
        cout << endl;
        }
    else if (option == 'C') //option for correctly spelled commonly misspelled words, takes indices and displays all words in that range
        {
        cout << "Enter the start and end indices between 0 and 113: ";
        cin >> start;
        cin >> end;
        for (int i = start; i <= end; i++)
        {
            cout << i << ". " << correct.at(i) << endl;
        }
        cout << endl;
        }
    else //exception in the case that option is not a, b, or c
    {
        cout << "Please select an option above: ";
        cin >> option;
    }
} //end of displaySomeWords()

string searchForWord(vector<string>& dictionary) //uses a binary search algorithm to check if a word exists in dictionary
{
    int lower = 0; //lower bound
    int upper = dictionary.size() - 1; //upper bound
    int comparisons = 1; //number of comparisons made
    string wordLookup; //word that needs to be looked up
    cout << "Enter the word to lookup: ";
    cin >> wordLookup;
    while (lower <= upper) //will execute as long as lower is less than or equal to upper
        {
        int middle = lower + ((upper - lower) / 2); //middle term regardless of interation
        cout << comparisons << ". Comparing to: " << dictionary.at(middle) << endl;
        if (wordLookup.compare(dictionary.at(middle)) == 0) //if word is found
            {
            cout << wordLookup << " was found." << endl << endl;
            break;
            }
        else
        {
            if (wordLookup > dictionary.at(middle)) //if the word is above the middle, we need to increase the lower bound
                {
                lower = middle + 1;
                comparisons++;
                }
            else if (wordLookup < dictionary.at(middle)) //if the word is below the middle, we need to lower the upper bound
                {
                upper = middle - 1;
                comparisons++;
                }
        }
        }
    if (lower > upper) //means that the word does not exist in dictionary
        {
        cout << wordLookup << " was NOT found." << endl << endl;
        }
    return wordLookup;
} //end of searchForWord()

int setWordLength(int& wordLength) //changes the value of wordLength
{
    cout << "Enter new wordLength: ";
    cin >> wordLength;
    return wordLength;
} //end of setWordLength

int main()
{
    int wordLength = 5; //wordLength variable set to default of 5, used as a reference parameter elsewhere
    int userInput; //used for seleting menu options
    int bestScore = 0; //bestScore set to default of 0, used as a reference parameter elsewhere
    srand(1); //seeds random integers throughout program where random integers are needed
    vector<string> dictionary;
    vector<string> correct;
    vector<string> incorrect;
    dictionaryVector(dictionary); //creates a dictionary vector used elsewhere
    misspelledWordsVector(incorrect, correct); //creates an incorrect and correct vector used elsewhere
    cout << "There are 263533 words in the full dictionary." << endl;
    cout << "There are 114 commonly misspelled words." << endl << endl;
    displayInstructions();
    cin >> userInput;
    while (userInput != 6) //executes the various functions based on user input as long as option is not 6
        {
        if (userInput == 1)
        {
            spellMisspelledWords(correct, incorrect, bestScore);
            displayInstructions();
            cin >> userInput;
        }
        else if (userInput == 2)
        {
            randomWordsFromDictionary(dictionary, wordLength, bestScore);
            displayInstructions();
            cin >> userInput;
        }
        else if (userInput == 3)
        {
            displaySomeWords(dictionary, correct, incorrect);
            displayInstructions();
            cin >> userInput;
        }
        else if (userInput == 4)
        {
            searchForWord(dictionary);
            displayInstructions();
            cin >> userInput;
        }
        else if (userInput == 5)
        {
            setWordLength(wordLength);
            displayInstructions();
            cin >> userInput;
        }
        }
    if (userInput == 6) //exits program
        {
        cout << "Exiting program" << endl;
        cout << "Best score was " << bestScore << endl;
        exit(0);
        }
    return 0;
} //end of main()