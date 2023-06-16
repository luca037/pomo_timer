#include <iostream>

#include "../include/SimplePomoTimer.h"


// check if string contains only digits
bool isdigit(const std::string& s) {
    for (char c : s)
        if (!isdigit(c)) return false;
    return true;
}

// ask for a number
int init_time() {
    for (std::string s; std::cin >> s; )
        if (isdigit(s)) return stoi(s);
    return 0;
}

// ask to quit session
bool end_session_asking() {
    std::cout << "You done a pomo!" << std::endl
         << "Do you want to end you session here? [y/n]" << std::endl 
         << "(timer restarts automatically after the answer)" << std::endl;
    for (char c; std::cin >> c; )
        if (c == 'y') return true; // end session
        else if (c == 'n') break;
    return false;
}

int main(int argc, char* argv[]) {
    // setup: work, break, pomo
    int w{}, b{}, p{};
    std::cout << "Set work time: ";
    w = init_time();
    std::cout << "Set break time: ";
    b = init_time();
    std::cout << "How many pomos today? ";
    p = init_time();

    timer::SimplePomoTimer t{w, b, p, argv[1], argv[2]};

    // wait to start
    std::cout << "Type 's' to start timer" << std::endl;
    for (char c; std::cin >> c; ) if (c == 's') break;

    // start session
    while (1) {
        t.one_pomo();
        if (t.done()) break;
        else if (end_session_asking()) break;
    }
    std::cout << "We're done for today. Good job" << std::endl;


    return 0;
}
