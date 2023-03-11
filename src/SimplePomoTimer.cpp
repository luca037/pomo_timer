#include "../include/SimplePomoTimer.h"

#include <iostream>
#include <thread>
#include <chrono>

timer::SimplePomoTimer::SimplePomoTimer(int w, int b, int p, char* fn, char* d)
    : work_min_{w}, break_min_{b}, max_pomos_{p}, today_pomos_{0}, is_working_{0} {
    if (!fn) return;
    calendar_.open(fn, std::ofstream::app);
    // save settings in calendar file
    calendar_ << d << "  "
             << "w: " << w << "min  "
             << "b: " << b << "min  "
             << "p: " << p << "  ";
}

void timer::SimplePomoTimer::one_pomo() {
    if (done()) return; // no more pomo

    // work
    is_working_ = 1;
    int min{}, sec{};
    bool skip{}; // thread comunication

    std::thread thr{skip_timer, &skip}; // start new thread
    while (work_min_ != min && !skip) { // work counter
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++sec;
        if (sec == 60) { ++min; sec = 0; } // minutes check
        print_state(min, sec); // print timer
    }
    system(kNotifySoundCmd);
    if (skip) for (char c; std::cin >> c; ) if (c == 's') break; // if skipped
    thr.join(); // wait thread

    // break
    is_working_ = sec = min = skip = 0; // reset var

    thr = std::thread{skip_timer, &skip}; // start new thread
    while (break_min_ != min && !skip) { // break counter
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++sec;
        if (sec == 60) { ++min; sec = 0; } // minutes check
        print_state(min, sec); // print timer
    }

    // end
    ++today_pomos_;
    system(kNotifySoundCmd);
    thr.join(); // wait thread
}

void timer::SimplePomoTimer::print_state(int min, int sec) {
    system("clear");
    std::cout << "+-------------------------+" << std::endl 
              << " POMODORO TIMER" << std::endl << " Your settings:" << std::endl 
              << "    * Work time: " << work_min_ << " min" << std::endl
              << "    * Break time: " << break_min_ << " min" << std::endl
              << "+-------------------------+" << std::endl
              << ((is_working_)? " Working time:" : " Break time:") << std::endl
              << "  timer: " << min << ":" << sec << std::endl
              << "  pomos: " << today_pomos_ << "/" << max_pomos_ << std::endl
              << "+-------------------------+" << std::endl
              << "Type 's' start/skip timer" << std::endl;
}

void timer::skip_timer(bool* s) {
    for (char c; std::cin >> c && !(*s); )
        if (c == 's') { *s = true; return; }
}
