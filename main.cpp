/******************************************************************************
 *  AUTHOR       : Zhongming Su
 *  ASSIGNMENT #8: Hashing Shakespeare
 *  CLASS        : CS 008
 *  DUE DATE     : May 21 2020
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <set>
#include <iomanip>
#include <cmath>
#include "hashTable.h"
/******************************************************************************
 *
 * Hashing Shakespeare
 *_____________________________________________________________________________
 * Experiment the hash table and its three different probing methods on
 *     Shakespeare's document. (Linear, Quadratic, and Chaining)
 *_____________________________________________________________________________
 * Input:
 *  fileName: Name of the file to open, found in the command line argument
 * 
 * Output:
 *  table size and the number of collisions for each probing method in each round
 *      as well as a best method conclusion
 ******************************************************************************/

/******************************************************************************
 *  openFile
 *    Open the file with given name. Exit the program with error code 1 if failed
 *    - returns void
 ******************************************************************************/
void openFile(std::ifstream& inFile, std::string fileName);

/******************************************************************************
 *  getWords
 *    Get all unique words found in the given file. Only letter, numbers and '\''
 *      all considered as part of a word
 *    - returns std::set<std::string>
 ******************************************************************************/
std::set<std::string> getWords(std::ifstream& inFile);

/******************************************************************************
 *  nextWord
 *    Get the next available string in the given file, then clear all the white
 *      spaces after
 *    - returns std::string
 ******************************************************************************/
std::string nextWord(std::ifstream& inFile);

/******************************************************************************
 *  hasingFunc
 *    Give the given string a hashing value
 *      Sum of int(word[i])^3 * (i+1)
 *    - returns int
 ******************************************************************************/
int hashingFunc(std::string word);

/******************************************************************************
 *  nextPrime
 *    return the next smallest prime number that is >= to start
 *    - returns int
 ******************************************************************************/
int nextPrime(int start);

/******************************************************************************
 *  isPrime
 *    Detetermine if the given number is a prime number
 *    - returns bool
 ******************************************************************************/
bool isPrime(int num);

/******************************************************************************
 *  getCollisions
 *    Create three different hash tables and output collision results
 *      minimum collisions number and type of hash table is found
 *      type: 0 - linear, 1 - quadratic, 2 - chaining
 *    - returns int
 ******************************************************************************/
int getCollisions(const std::set<std::string>& words, int size, int& type);

int main(int argc, char** argv) {
    // constants
    const int ROUNDS = 5;
    // declare variables
    std::string fileName;
    std::ifstream inFile;
    std::set<std::string> words;
    int size;
    int minCollision = INT_MAX, minSize, minType;
    int tempCollision, tempSize, tempType;

    std::cout << "/********************************************************\n";
    std::cout << "Programmed by: Zhongming Su\n";
    std::cout << "Student ID   : 10342289\n";
    std::cout << "CS008        : MW 9:45am - 11:10am, TuTh 9:45am - 11:50am\n";
    std::cout << "LAB #8       : Hashing Shakspeare\n";
    std::cout << " ********************************************************/\n\n";

    // INPUT -- Gets input from the command line argument file name
    if(argc < 2) {
        std::cout << "No command line argument\n";
        exit(1);
    }
    std::cout << "Reading from " << argv[1] << " ...\n\n";
    fileName = argv[1];

    openFile(inFile, fileName);

    words = getWords(inFile);
    inFile.close();

    // PROCESSING & OUTPUT -- store each word in 3 types of hash tables
    //      (linear, quadratic, chaining) and display the collisions
    //      results for each method. Repeat for the number of ROUNDS,
    //      table size is increased each round
    size = words.size();
    
    std::cout << "There are " << size << " number of unique words\n\n";
    std::cout << "Number of collisions for different table sizes are:\n";
    std::cout.setf(std::ios::right);
    std::cout << std::setw(12) << "Table Size" <<  std::setw(12) << "Linear"
              << std::setw(12) << "Quadratic" <<  std::setw(12) << "Chaining";
    std::cout << "\n------------------------------------------------\n";

    for(int i = 1; i <= ROUNDS; i++) {
        tempSize = nextPrime(int(size * (1+0.19*i)));
        tempCollision = getCollisions(words, tempSize, tempType);
        if(tempCollision < minCollision) {
            minCollision = tempCollision;
            minSize = tempSize;
            minType = tempType;
        }
    }

    std::cout << "\nThe best tested combination is:\n";
    std::cout << "Table size: " << minSize << std::endl;
    std::cout << "Collision handling using: ";
    switch(tempType) {
        case 0: 
            std::cout << "Linear\n";
            break;
        case 1:
            std::cout << "Quadratic\n";
            break;
        default:
            std::cout << "Chaining\n";
            break;
    }
    std::cout << "Total collisions: " << minCollision << std::endl;

    std::cout << "\nHashing function used was the (sum of (ASCII value of\n"
              << "each letter) ^ 3  *  (index + 1)) % table size\n";

    std::cout << "\nExiting Program ...\n\n";

    return 0;
}

/******************************************************************************
 *
 *  FUNCTION openFile
 *_____________________________________________________________________________
 *  Open the file with given name. Exit the program with error code 1 if failed
 *  - returns void
 *_____________________________________________________________________________
 *  PRE-CONDITIONS
 *    fileName is valid
 *
 *  POST-CONDITIONS
 *    file with fileName is opened
 ******************************************************************************/
void openFile(std::ifstream& inFile, std::string fileName) {
    inFile.open(fileName);
    if(inFile.fail()) {
        std::cout << fileName << " failed to open\n";
        exit(2);
    }
}

/******************************************************************************
 *
 *  FUNCTION getWords
 *_____________________________________________________________________________
 *  Get all unique words found in the given file. Only letter, numbers and '\''
 *      all considered as part of a word
 *  - returns std::set<std::string>
 *_____________________________________________________________________________
 *  PRE-CONDITIONS
 *    inFile is opened
 *
 *  POST-CONDITIONS
 *    All unique words in the file are returned
 ******************************************************************************/
std::set<std::string> getWords(std::ifstream& inFile) {
    std::set<std::string> words;    
    while(!inFile.eof()) {
        words.insert(nextWord(inFile));
    }

    return words;
}

/******************************************************************************
 *
 *  FUNCTION nextWord
 *_____________________________________________________________________________
 *  Get the next available string in the given file, then clear all the white
 *      spaces after
 *  - returns std::string
 *_____________________________________________________________________________
 *  PRE-CONDITIONS
 *    inFile is opened
 *
 *  POST-CONDITIONS
 *    next available word is returned
 ******************************************************************************/
std::string nextWord(std::ifstream& inFile) {
    char next;
    std::string word = "";
    inFile.get(next);
    while(isalpha(next) || isdigit(next) || next == '\'') {
        word += next;
        inFile.get(next);
    }
    while(!isalpha(next) && !isdigit(next) && next != '\'') {
        if(inFile.eof()) {
            return word;
        }
        inFile.get(next);
    }
    inFile.putback(next);
    return word;
}

/******************************************************************************
 *
 *  FUNCTION hashingFunc
 *_____________________________________________________________________________
 *  Give the given string a hashing value
 *      Sum of int(word[i])^3 * (i+1)
 *  - returns int
 *_____________________________________________________________________________
 *  PRE-CONDITIONS
 *    none
 *
 *  POST-CONDITIONS
 *    converted hashing value of word is returned
 ******************************************************************************/
int hashingFunc(std::string word) {
    int value = 0;
    for(int i = 1; i <= word.size(); i++) {
        value += i * std::pow((int(word[i-1])), 3);
    }
    return value;
}

/******************************************************************************
 *
 *  FUNCTION nextPrime
 *_____________________________________________________________________________
 *  return the next smallest prime number that is >= to start
 *  - returns int
 *_____________________________________________________________________________
 *  PRE-CONDITIONS
 *    none
 *
 *  POST-CONDITIONS
 *    a prime number is returned
 ******************************************************************************/
int nextPrime(int start) {
    int prime = start;

    if(prime % 2 == 0) {
        prime++;
    }

    while(!isPrime(prime)) {
        prime += 2;
    }
    return prime;
}

/******************************************************************************
 *
 *  FUNCTION isPrime
 *_____________________________________________________________________________
 *  Detetermine if the given number is a prime number
 *  - returns bool
 *_____________________________________________________________________________
 *  PRE-CONDITIONS
 *    none
 *
 *  POST-CONDITIONS
 *    return true if num is prime
 ******************************************************************************/
bool isPrime(int num) {
    for(int i = 2; i < num/2; i++) {
        if(num % i == 0) {
            return false;
        }
    }
    return true;
}

/******************************************************************************
 *
 *  FUNCTION getCollisions
 *_____________________________________________________________________________
 *  Create three different hash tables and output collision results
 *      minimum collisions number and type of hash table is found
 *      type: 0 - linear, 1 - quadratic, 2 - chaining
 *  - returns int
 *_____________________________________________________________________________
 *  PRE-CONDITIONS
 *    size >= words.size()
 *
 *  POST-CONDITIONS
 *    the minimum collision and hash table type is returned
 ******************************************************************************/
int getCollisions(const std::set<std::string>& words, int size, int& type) {
    HashTable<std::string> linear(size, 0);
    HashTable<std::string> quadratic(size, 1);
    HashTable<std::string> chaining(size, 2);

    for(std::string s : words) {
        linear.insertKey(s, hashingFunc(s));
        quadratic.insertKey(s, hashingFunc(s));
        chaining.insertKey(s, hashingFunc(s));
    }

    int l = linear.getCollisions();
    int q = quadratic.getCollisions();
    int c = chaining.getCollisions();

    std::cout << std::setw(12) << size << std::setw(12) << l
              << std::setw(12) << q << std::setw(12) << c << std::endl;

    if(l <= q && l <= c) {
        type = 0;
        return l;
    } else if(q <= l && q <= c) {
        type = 1;
        return q;
    } else {
        type = 2;
        return c;
    }
    return 0;
}