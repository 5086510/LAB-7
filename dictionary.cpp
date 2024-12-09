#include "dictionary.h"

// Helper function to parse and normalize words from input
WORD getNextWord(std::istream& in) {
    WORD word;
    char ch;
    bool inWord = false;
    while (in.get(ch)) {
        if (isalpha(ch)) {
            word.push_back(ch); // Keep case sensitivity
            inWord = true;
        } else if (inWord) {
            return word;
        }
    }
    return word;
}

// Function to initialize letter frequencies with 0 for all letters a to z and A to Z
void initializeLetterFreq(WORD_FREQ_LIST& letterFreq) {
    for (char c = 'A'; c <= 'Z'; ++c) {
        letterFreq[c] = 0; // Initialize frequency of uppercase letters to 0
    }
    for (char c = 'a'; c <= 'z'; ++c) {
        letterFreq[c] = 0; // Initialize frequency of lowercase letters to 0
    }
}

// Function to process the input file and update the DICTION struct
void processFile(std::istream& in, DICTION& d) {
    int charCount = 0, wordCount = 0, lineCount = 0;

    // Initialize letter frequency map for all letters (a to z and A to Z)
    initializeLetterFreq(d.letterFreq);

    // Temporary map to store word frequencies for insertion into the multimap
    std::map<WORD, int> tempWordFreq;

    string line;
    while (getline(in, line)) {
        charCount += line.size(); // Add all characters in the line to the character count
        if (!line.empty()) {
            ++lineCount; // Increment line count
        }

        istringstream lineStream(line);
        WORD word;

        while ((word = getNextWord(lineStream)) != "") {
            ++wordCount;             // Increment word count
            d.uniqueWords.insert(word); // Add to unique word set

            // Increment word frequency in temporary map
            tempWordFreq[word]++;

            // Increment letter frequency (both lowercase and uppercase)
            for (char c : word) {
                d.letterFreq[c]++;
            }

            // Push word to priority queue for longest tracking
            d.longWord.push(word);
        }
    }

    // Transfer data from tempWordFreq to multimap
    for (const auto& entry : tempWordFreq) {
        d.freqWord.insert({entry.second, entry.first});
    }

    // Update stats
    get<0>(d.stats) = charCount;
    get<1>(d.stats) = wordCount;
    get<2>(d.stats) = lineCount;
}

// Function to display letter frequency histogram
void printLetterHistogram(const WORD_FREQ_LIST& letterFreq) {
    cout << "\n/-------------\\\n| Letter Freq |\n\\-------------/\n";

    // Display uppercase letters (A to Z)
    for (char c = 'A'; c <= 'Z'; ++c) {
        auto it = letterFreq.find(c);
        int count = (it != letterFreq.end()) ? it->second : 0;

        cout << c << " | ";
        for (int i = 0; i < min(count, 10); ++i) {
            cout << '*';
        }
        if (count > 10) {
            cout << " (" << (count - 1) << ")";
        }
        cout << '\n';
    }

    // Display lowercase letters (a to z)
    for (char c = 'a'; c <= 'z'; ++c) {
        auto it = letterFreq.find(c);
        int count = (it != letterFreq.end()) ? it->second : 0;

        std::cout << c << " | ";
        for (int i = 0; i < std::min(count, 10); ++i) {
            cout << '*';
        }
        if (count > 10) {
            cout << " (" << count - 1 << ")";
        }
        cout << '\n';
    }
}

// Function to display a vertical word frequency histogram
void printVerticalHistogram(const FREQ_WORD_LIST& freqWord) {
    string longestWord;
    cout << "\n/------------\\\n| Histogram  |\n\\------------/\n";

    // Prepare a list of words and their frequencies
    vector<pair<int, string>> wordList;
    for (const auto& entry : freqWord) {
        wordList.push_back({entry.first, entry.second});
    }

    // Find the length of the longest word
    size_t maxWordLength = 0;
    for (const auto& wordEntry : wordList) {
        if (wordEntry.second.length() > maxWordLength) {
            maxWordLength = wordEntry.second.length();
            longestWord = wordEntry.second; // Update the longest word
        }
    }

    // Find the maximum frequency to determine the number of rows (stars)
    int maxFreq = 0;
    for (const auto& entry : freqWord) {
        if (entry.first > maxFreq) {
            maxFreq = entry.first;
        }
    }

    size_t dashCount = 2 * maxWordLength + 2;  // Start with the longest word length + 2 dashes
    size_t offset = dashCount;

    for (int row = maxFreq; row > 0; --row) {
        // Add leading spaces to align stars with the first word's "/"
        cout << string(offset, ' ');

        for (size_t i = 0; i < wordList.size(); ++i) {
            if (wordList[i].first >= row) {
                cout << "* "; // Print a star for each word at the current level
            } else {
                cout << "  "; // Print spaces for words with lower frequency
            }
        }
        cout << '\n';
    }

    size_t offset2 = (2 * maxWordLength) + 2 + (2 * (wordList.size() - 1)) + 1;
    cout << string(offset2, '-');
    cout << '\n';

    // Print the words one under another at the bottom, with dashes connecting them to the stars
    size_t dashLength = (maxWordLength) + 2;  // Start with the longest word length + 2 dashes

    for (size_t i = 0; i < wordList.size(); ++i) {
        const auto& wordEntry = wordList[i];

        // Print word and pad spaces to align with longest word
        cout << setw(maxWordLength) << left << wordEntry.second;

        // Print the corresponding number of dashes
        for (size_t j = 0; j < dashLength; ++j) {
            cout << "-";  // Print dashes based on the calculated length
        }

        cout << "/ ";  // End the line with a "/"

        // Print "|" based on how many words remain after the current one
        int remainingWords = wordList.size() - i - 1;  // Remaining words after current word
        for (int j = 0; j < remainingWords; ++j) {
            if (i < wordList.size()) {
                cout << "| ";
            }
        }

        cout << '\n';

        // Increase dash length for the next word (add 2 for each subsequent word)
        dashLength += 2;
    }

    cout << "\nThe longest word is: " << longestWord << '\n';
}

// Function to display statistics
void displayStatistics(const DICTION& d) {
    // Display all words in the dictionary
    cout << "\nWords in dictionary: ";
    for (const auto& word : d.uniqueWords) {
        cout << word << " ";
    }
    cout << '\n';

    // Display basic stats
    cout << "\nNumber of Chars: " << std::get<0>(d.stats);
    cout << "\nNumber of Words: " << std::get<1>(d.stats);
    cout << "\nNumber of Lines: " << std::get<2>(d.stats);

    // Display letter frequency histogram
    printLetterHistogram(d.letterFreq);

    // Display dictionary with frequencies
    cout << "\n/------------\\\n| Dictionary |\n\\------------/\n";
    cout << "Word          Frequency\n-----------------------\n";
    for (const auto& entry : d.freqWord) {
        cout << setw(22) << left << entry.second << entry.first << '\n';
    }

    // Display word frequency histogram
    printVerticalHistogram(d.freqWord);
}

