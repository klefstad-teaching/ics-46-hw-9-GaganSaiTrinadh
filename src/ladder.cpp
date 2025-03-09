#include "./ladder.h"
using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>


void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}


bool edit_distance_within(const string& str1, const string& str2, int d) {
    int len1 = str1.size();
    int len2 = str2.size();
    
    if (abs(len1 - len2) > d) return false;
    
    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; i++) {
            if (tolower(str1[i]) != tolower(str2[i]))
                diffCount++;
            if (diffCount > d)
                return false;
        }
        return (diffCount == d);
    }
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
                j++; 
            }
        }
        if (j < longer.size())
            diffCount++;
        return (diffCount == d);
    }
}


bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2) return false;
    return edit_distance_within(word1, word2, 1);
}


vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start word and end word are the same");
        return vector<string>(); 
    }
    
    queue<vector<string>> ladder_queue;
    vector<string> initial;
    initial.push_back(begin_word);
    ladder_queue.push(initial);
    
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
    
    return vector<string>();
}

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

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1)
            cout << " -> ";
    }
    cout << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "src/words.txt");
    
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
