#include <iostream>
#include <ostream>
#include <string>

#include "../include/SimplePomoTimer.h"

// check if string contains only digits
bool isdigit(const std::string& s) {
    for (char c : s)
        if (!std::isdigit(c)) return false;
    return true;
}

// ask for a number
int init_time() {
    for (std::string s; std::cin >> s; )
        if (isdigit(s)) return std::stoi(s);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "ERROR - You must pass args" << std::endl;
        return 0;
    }

    // setup: work, break, pomo
    std::cout << "Set work time: ";
    int wait_t{init_time()};
    std::cout << "Set break time: ";
    int break_t{init_time()};
    std::cout << "How many pomos today? ";
    int max_pomo{init_time()};

    // parse cmd args
    int add_done_pomos = std::stoi(argv[1]);
    int add_max_pomos = std::stoi(argv[2]);
    char* calendar_file = argv[3];
    char* date = argv[4];

    timer::SimplePomoTimer timer{
        wait_t, 
        break_t, 
        max_pomo+add_max_pomos, 
        add_done_pomos, 
        calendar_file, 
        date
    };

    // start session
    timer.start();
    std::cout << "We're done for today. Good job" << std::endl;

    return 0;
}
