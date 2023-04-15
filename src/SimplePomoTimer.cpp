#include "../include/SimplePomoTimer.h"

#include <iostream>
#include <thread>
#include <chrono>

timer::SimplePomoTimer::SimplePomoTimer(int w, int b, int p, char* fn, char* d)
        : work_min_{w}, break_min_{b},
          max_pomos_{p}, today_pomos_{0}, is_working_{0} {
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

    // start work
    is_working_ = 1;
    int min{}, sec{};
    std::atomic_bool skip{}; // thread comunication

    std::thread thr{skip_timer, &skip}; // start new thread
    while (work_min_ != min && !skip.load()) { // work counter
        print_state(min, sec);
        ++sec;
        if (sec == 60) { ++min; sec = 0; }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    print_state(min, sec);

    // notify and wait to restart
    system(kNotifySoundCmd);
    if (skip.load()) for (char c; std::cin >> c; ) if (c == 's') break;
    thr.join();

    // start break
    is_working_ = sec = min = 0; // reset var
    skip.store(false);

    thr = std::thread{skip_timer, &skip}; // start new thread
    while (break_min_ != min && !skip.load()) { // break counter
        print_state(min, sec);
        ++sec;
        if (sec == 60) { ++min; sec = 0; }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    print_state(min, sec);

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

void timer::skip_timer(std::atomic_bool* s) {
    for (char c; std::cin >> c && !(s->load()); )
        if (c == 's') { s->store(true); return; }
}
