#include "../include/SimplePomoTimer.h"

#include <iostream>
#include <thread>
#include <chrono>

timer::SimplePomoTimer::SimplePomoTimer(int w, int b, int p, int tp, char* fn, char* d)
        : work_min_{w}, break_min_{b},
          max_pomos_{p}, today_pomos_{tp} {
    if (!fn) return;
    calendar_.open(fn, std::ofstream::app);
    // save settings in calendar file
    calendar_ << d << "  "
             << "w: " << w << "min  "
             << "b: " << b << "min  "
             << "p: " << p << "  ";
}

void timer::SimplePomoTimer::start() {
    // wait to start
    std::cout << "Type 's' to start timer" << std::endl;
    for (char c; std::cin >> c; ) if (c == 's') break;

    // start session
    while (max_pomos_ != today_pomos_) {
        one_pomo();
        if (max_pomos_ != today_pomos_ && end_session_asking()) 
            break;
    }
}

void timer::SimplePomoTimer::one_pomo() {
    // start work
    std::atomic_bool skip{}; // thread comunication
    std::thread thr{skip_timer, &skip}; // start new thread

    int min{}, sec{};
    while (work_min_ != min && !skip.load()) { // work counter
        print_state(min, sec, "Working time:");
        ++sec;
        if (sec == 60) { ++min; sec = 0; }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    print_state(min, sec, "Working time:");

    // notify and wait to restart
    std::system(kNotifySoundCmd);
    if (skip.load()) 
        for (char c; std::cin >> c; ) 
            if (c == 's') break;
    thr.join();

    // start break
    sec = min = 0; // reset time
    skip.store(false);

    thr = std::thread{skip_timer, &skip}; // start new thread
    while (break_min_ != min && !skip.load()) { // break counter
        print_state(min, sec, "Break time:");
        ++sec;
        if (sec == 60) { ++min; sec = 0; }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    ++today_pomos_;
    print_state(min, sec, "Break time:");

    // end
    std::system(kNotifySoundCmd);
    thr.join(); // wait thread
}

// ask to quit session
bool timer::SimplePomoTimer::end_session_asking() {
    std::cout << "You done a pomo!\n"
         << "Do you want to end you session here? [y/n]\n"
         << "(timer restarts automatically after the answer)\n";
    for (char c; std::cin >> c; ) {
        if (c == 'y') {
            return true;
        } else if (c == 'n') {
            break;
        }
    }
    return false;
}

void timer::SimplePomoTimer::print_state(int min, int sec, std::string state) {
    std::system("clear");
    static std::string bar{};
    int n_blocks = 28 * today_pomos_ / max_pomos_;
    while (bar.size() < n_blocks * 2)
        bar += "█";
    std::cout << "┌──────────────────────────┐\n"
              << "│      POMODORO TIMER      │\n" 
              << "└──────────────────────────┘\n"
              << "┌──────────────────────────┐\n"
              << " Your settings:          \n"
              << "    ► Work time: " << work_min_ << " min\n" 
              << "    ► Break time: " << break_min_ << " min\n"
              << "└──────────────────────────┘\n"
              << "┌──────────────────────────┐\n"
              << " " << state << "\n"
              << "    ► timer: " << min << ":" << sec << "\n"
              << "    ► pomos: " << today_pomos_ << "/" << max_pomos_ << "\n"
              << "   ┌───────────────────┐\n"
              << "    " << bar << "\n"
              << "   └───────────────────┘\n"
              << "└──────────────────────────┘\n"
              << "Type 's' start/skip timer\n";
}

void timer::skip_timer(std::atomic_bool* s) {
    for (char c; std::cin >> c && !(s->load()); )
        if (c == 's') { 
            s->store(true); return; 
        }
}
