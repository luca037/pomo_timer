#include "../include/SimplePomoTimer.h"

#include <iostream>
#include <thread>
#include <chrono>

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
    wait_start();
    is_working_ = true;
    int min{}, sec{};
    bool skip{}; // comunication for the thread
    std::thread thr{skip_timer, &skip}; // start new thread
    while (work_min_ != min && !skip) { // work counter
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++sec;
        if (sec == 60) { ++min; sec = 0; } // minutes check
        print_state(min, sec, is_working_, skip); // print timer

    }
    thr.join(); // wait thread
    system(kNotifySoundCmd);

    // break
    wait_start();
    is_working_ = false;
    min = sec = skip = 0; // reset var
    thr = std::thread{skip_timer, &skip}; // start new thread
    while (break_min_ != min && !skip) { // break counter
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++sec;
        if (sec == 60) { ++min; sec = 0; } // minutes check
        print_state(min, sec, false, skip); // print timer
    }
    thr.join();
    
    // end
    ++today_pomos_;
    if (done()) std::cout << "We're done for today. Good job" << std::endl;
    else system(kNotifySoundCmd);
}

void timer::SimplePomoTimer::print_state(int min, int sec, bool w, bool s) {
    system("clear");
    std::cout << "+-------------------------+" << std::endl 
              << " POMODORO TIMER" << std::endl << " Your settings:" << std::endl 
              << "    * Work time: " << work_min_ << " min" << std::endl
              << "    * Break time: " << break_min_ << " min" << std::endl
              << "+-------------------------+" << std::endl
              << ((w)? "Working time:" : "Break time:") << std::endl
              << "  timer: " << min << ":" << sec << std::endl
              << "  pomos: " << today_pomos_ << "/" << max_pomos_ << std::endl
              << ((!s)? "Type 'q' to skip timer" : "") << std::endl;
}

void timer::SimplePomoTimer::wait_start() {
    std::cout << "Type 's' start timer" << std::endl;
    for (char c; std::cin >> c; ) if (c == 's') break; // wait to restart
}

void timer::skip_timer(bool* s) {
    for (char c; std::cin >> c; )
        if (c == 'q') {
            *s = true;
            return;
        }
}
