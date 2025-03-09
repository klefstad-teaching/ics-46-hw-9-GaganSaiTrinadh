#include "./ladder.h"
#include <iostream>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    set<string> word_list;
    load_words(word_list, "src/words.txt");

    string begin_word, end_word;
    cout << "Enter start word: ";
    cin >> begin_word;
    cout << "Enter end word: ";
    cin >> end_word;
    
    transform(begin_word.begin(), begin_word.end(), begin_word.begin(), ::tolower);
    transform(end_word.begin(), end_word.end(), end_word.begin(), ::tolower);
    
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start word and end word cannot be the same.");
        return 1;
    }
    
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    
    if (ladder.empty()) {
        cout << "No word ladder found from \"" << begin_word << "\" to \"" << end_word << "\"." << endl;
    } else {
        cout << "Word ladder from \"" << begin_word << "\" to \"" << end_word << "\":" << endl;
        print_word_ladder(ladder);
    }
    
    verify_word_ladder();
    
    return 0;
}
