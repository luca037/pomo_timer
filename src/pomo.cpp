#include <iostream>

#include "../include/SimplePomoTimer.h"

using namespace std;

bool isdigit(const string& s) {
    for (char c : s)
        if (!isdigit(c)) return false;
    return true;
}

int init_time() {
    for (string s; cin >> s; )
        if (isdigit(s)) return stoi(s);
    return 0;
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
    std::cout << "Type 's' to start timer" << std::endl;
    for (char c; std::cin >> c; ) if (c == 's') break;

    // start session
    while (!t.done()) {
        t.one_pomo(); 
        if (!t.done()) { // end session asking
            cout << "You done a pomo!" << endl
                 << "Do you want to end you session here? [y/n]" << endl 
                 << "(timer restarts automatically after the answer)" << endl;
            for (char c; cin >> c; )
                if (c == 'y') return 0; // end session
                else if (c == 'n') break;
        }
    }
    cout << "We're done for today. Good job" << std::endl;

    return 0;
}
