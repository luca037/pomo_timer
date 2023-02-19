#include "../include/SimplePomoTimer.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

timer::SimplePomoTimer::SimplePomoTimer(int w, int b, int p, char* fn, char* d)
    : work_min_{w}, break_min_{b}, max_pomos_{p}, today_pomos_{0} {
    if (!fn) return;
    calendar_.open(fn, std::ofstream::app);
    // save settings in calendar
    calendar_ << d << "  "
             << "w: " << w << "min  "
             << "b: " << b << "min  "
             << "p: " << p << "  ";
}

void timer::SimplePomoTimer::one_pomo() {
    if (done()) return; // no more pomo

    // work
    wait_input();
    int min{};
    int sec{50};
    while (work_min_ != min) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++sec;
        if (sec == 60) { ++min; sec = 0; } // minutes check
        print_state(min, sec, true); // print timer

    }
    system(kNotifySoundCmd);

    // break
    wait_input();
    min = 0;
    sec = 50;
    while (break_min_ != min) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++sec;
        if (sec == 60) { ++min; sec = 0; } // minutes check
        print_state(min, sec, false); // print timer
    }
    
    // end
    ++today_pomos_;
    if (done()) std::cout << "We're done for today. Good job" << std::endl;
    else system(kNotifySoundCmd);
}

void timer::SimplePomoTimer::print_state(int min, int sec, bool w) {
    system("clear");
    std::cout << "+-------------------------+" << std::endl 
              << " POMODORO TIMER" << std::endl << " Your settings:" << std::endl 
              << "    * Work time: " << work_min_ << " min" << std::endl
              << "    * Break time: " << break_min_ << " min" << std::endl;
    std::cout << "+-------------------------+" << std::endl
              << ((w)? "Working time:" : "Break time:") << std::endl
              << "  timer: " << min << ":" << sec << std::endl
              << "  pomos: " << today_pomos_ << "/" << max_pomos_ << std::endl;
}

void timer::SimplePomoTimer::wait_input() {
    std::cout << "Type 's' start timer" << std::endl;
    for (char c; std::cin >> c; ) if (c == 's') break; // wait to restart
}
