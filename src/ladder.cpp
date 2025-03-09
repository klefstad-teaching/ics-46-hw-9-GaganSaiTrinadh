#include "./ladder.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;

// Print an error message.
void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

// Returns true if the edit distance between str1 and str2 is within d (i.e. ≤ d).
bool edit_distance_within(const string& str1, const string& str2, int d) {
    int len1 = str1.size();
    int len2 = str2.size();

    // If length difference is greater than d, they cannot be neighbors.
    if (abs(len1 - len2) > d) return false;

    // Case 1: Same length: check substitutions.
    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; i++) {
            if (tolower(str1[i]) != tolower(str2[i]))
                diffCount++;
            if (diffCount > d)
                return false;
        }
        return (diffCount <= d);
    }
    // Case 2: Lengths differ by one: check for one insertion/deletion.
    else {
        const string& shorter = (len1 < len2 ? str1 : str2);
        const string& longer  = (len1 < len2 ? str2 : str1);
        int i = 0, j = 0, diffCount = 0;
        while (i < shorter.size() && j < longer.size()) {
            if (tolower(shorter[i]) == tolower(longer[j])) {
                i++;
                j++;
            } else {
                diffCount++;
                if (diffCount > d)
                    return false;
                j++;  // Skip a character in the longer string.
            }
        }
        // Account for an extra character at the end.
        if (j < longer.size())
            diffCount++;
        return (diffCount <= d);
    }
}

// Two words are adjacent if their edit distance is within 1.
// (Now identical words are considered adjacent.)
bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

// Uses BFS to generate a word ladder from begin_word to end_word.
// Intermediate words (and end_word) must be in word_list,
// but begin_word is allowed to be outside the dictionary.
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start word and end word are the same");
        return vector<string>();
    }
    
    queue<vector<string>> ladder_queue;
    vector<string> initial;
    initial.push_back(begin_word);
    ladder_queue.push(initial);
    
    // Keep track of words that have been used in any ladder.
    set<string> visited;
    visited.insert(begin_word);
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();
        
        for (const string& word : word_list) {
            if (visited.find(word) == visited.end() && is_adjacent(last_word, word)) {
                visited.insert(word);
                vector<string> new_ladder(ladder);
                new_ladder.push_back(word);
                if (word == end_word) {
                    return new_ladder;
                }
                ladder_queue.push(new_ladder);
            }
        }
    }
    
    // No ladder found.
    return vector<string>();
}

// Loads words from a file into the word_list set, converting to lowercase.
void load_words(set<string> & word_list, const string& file_name) {
    ifstream infile(file_name);
    if (!infile) {
        cerr << "Error: could not open file " << file_name << endl;
        return;
    }
    string word;
    while (infile >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        word_list.insert(word);
    }
}

// Prints the word ladder in the expected format.
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    cout << "Word ladder found:";
    for (const auto &w : ladder) {
        cout << " " << w;
    }
    cout << "\n";
}

// Test function to verify that generate_word_ladder produces ladders of expected lengths.
void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    auto my_assert = [](bool condition, const string & msg) {
        cout << msg << (condition ? " passed" : " failed") << endl;
    };
    
    vector<string> ladder;
    ladder = generate_word_ladder("cat", "dog", word_list);
    my_assert(ladder.size() == 4, "generate_word_ladder(\"cat\", \"dog\", word_list).size() == 4: ");
    
    ladder = generate_word_ladder("marty", "curls", word_list);
    my_assert(ladder.size() == 6, "generate_word_ladder(\"marty\", \"curls\", word_list).size() == 6: ");
    
    ladder = generate_word_ladder("code", "data", word_list);
    my_assert(ladder.size() == 6, "generate_word_ladder(\"code\", \"data\", word_list).size() == 6: ");
    
    ladder = generate_word_ladder("work", "play", word_list);
    my_assert(ladder.size() == 6, "generate_word_ladder(\"work\", \"play\", word_list).size() == 6: ");
    
    ladder = generate_word_ladder("sleep", "awake", word_list);
    my_assert(ladder.size() == 8, "generate_word_ladder(\"sleep\", \"awake\", word_list).size() == 8: ");
    
    ladder = generate_word_ladder("car", "cheat", word_list);
    my_assert(ladder.size() == 4, "generate_word_ladder(\"car\", \"cheat\", word_list).size() == 4: ");
}
