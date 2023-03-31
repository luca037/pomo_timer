#include <iostream>

#include "../include/SimplePomoTimer.h"

using namespace std;

// check if string contains only digits
bool isdigit(const string& s) {
    for (char c : s)
        if (!isdigit(c)) return false;
    return true;
}

// ask for a number
int init_time() {
    for (string s; cin >> s; )
        if (isdigit(s)) return stoi(s);
    return 0;
}

// ask to quit session
bool end_session_asking() {
    cout << "You done a pomo!" << endl
         << "Do you want to end you session here? [y/n]" << endl 
         << "(timer restarts automatically after the answer)" << endl;
    for (char c; cin >> c; )
        if (c == 'y') return true; // end session
        else if (c == 'n') break;
    return false;
}

int main(int argc, char* argv[]) {
    // setup: work, break, pomo
    int w{}, b{}, p{};
    cout << "Set work time: ";
    w = init_time();
    cout << "Set break time: ";
    b = init_time();
    cout << "How many pomos today? ";
    p = init_time();

    timer::SimplePomoTimer t{w, b, p, argv[1], argv[2]};

    // wait to start
    cout << "Type 's' to start timer" << endl;
    for (char c; cin >> c; ) if (c == 's') break;

    // start session
    while (1) {
        t.one_pomo();
        if (t.done()) break;
        else if (end_session_asking()) break;
    }
    cout << "We're done for today. Good job" << endl;

    return 0;
}
