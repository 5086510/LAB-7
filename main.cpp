#include "dictionary.h"

int main() {
    DICTION diction;

    // Process input (from file redirection)
    processFile(cin, diction);

    // Display the results
    displayStatistics(diction);

    return 0;
}

