#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <sstream>
#include <cctype>
#include <queue>
#include <vector>

using namespace std;

struct funcObj
{
    bool operator()(const std::string& a, const std::string& b) { return a.length() < b.length(); }
};

// Type aliases for clarity
using WORD = std::string;
using WORD_LIST = std::set<WORD>;
using WORD_FREQ_LIST = std::map<char, int>;       // Letter frequencies
using FREQ_WORD_LIST = std::multimap<int, WORD>; // Word frequencies with counts
using COUNTER = std::tuple<int, int, int>;       // To track (characters, words, lines)
using LONGEST = std::priority_queue<WORD, std::vector<WORD>, funcObj>;

// Struct to store statistics
struct DICTION {
    COUNTER stats;            // (Characters, Words, Lines)
    WORD_LIST uniqueWords;    // Unique words
    WORD_FREQ_LIST letterFreq; // Letter frequencies
    FREQ_WORD_LIST freqWord;  // Word frequencies with counts
    LONGEST longWord;         // Longest word
};

// Helper function to parse and normalize words from input
WORD getNextWord(std::istream& in);

// Function to initialize letter frequencies with 0 for all letters a to z and A to Z
void initializeLetterFreq(WORD_FREQ_LIST& letterFreq);

// Function to process the input file and update the DICTION struct
void processFile(std::istream& in, DICTION& d);

// Function to display letter frequency histogram
void printLetterHistogram(const WORD_FREQ_LIST& letterFreq);

// Function to display a vertical word frequency histogram
void printVerticalHistogram(const FREQ_WORD_LIST& freqWord);

// Function to display statistics
void displayStatistics(const DICTION& d);

#endif // DICTIONARY_H

