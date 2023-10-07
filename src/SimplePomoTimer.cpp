#include "../include/SimplePomoTimer.h"

#include <curses.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

timer::SimplePomoTimer::SimplePomoTimer(int w, int b, int p, int tp, char* fn, char* d)
        : work_min_{w}, break_min_{b},
          max_pomos_{p}, today_pomos_{tp}, win_{nullptr}  {
    if (!fn) return;
    calendar_.open(fn, std::ofstream::app);
    // save settings in calendar file
    calendar_ << d << "  "
             << "w: " << w << "min  "
             << "b: " << b << "min  "
             << "p: " << p << "  ";
}

void timer::SimplePomoTimer::start() {
    // setup window
    setlocale(LC_ALL, ""); // Unicode support
    initscr();             // init curses
    cbreak();
    noecho();
    keypad(stdscr, true);

    win_ = newwin(40, 80, 0, 0);
    refresh();

    // wait to start
    wprintw(win_, "Type any key to start.");
    wrefresh(win_);
    wgetch(win_);
    wclear(win_);

    // start session
    while (max_pomos_ != today_pomos_) {
        one_pomo();
        if (max_pomos_ != today_pomos_ && end_session_asking()) 
            break;
    }

    // end session
    wprintw(win_, "We're done for today. Good job.");
    wrefresh(win_);
    wgetch(win_);
}

void timer::SimplePomoTimer::one_pomo() {
    // start work
    int min{}, sec{};
    char skip = 0;

    nodelay(win_, true);
    while (work_min_ != min) { // work counter
        if ((skip = wgetch(win_)) == 's') break;
        print_state(min, sec, "Working time:");
        ++sec;
        if (sec == 60) { ++min; sec = 0; }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    print_state(min, sec, "Working time:");
    nodelay(win_, false);

    // notify and wait to restart
    std::system(kNotifySoundCmd);
    while (1) {
        char c = wgetch(win_);
        if (c == 's') break;
    }

    // start break
    sec = min = skip = 0; // reset var

    nodelay(win_, true);
    while (break_min_ != min) { // break counter
        if ((skip = wgetch(win_)) == 's') break;
        print_state(min, sec, "Break time:");
        ++sec;
        if (sec == 60) { ++min; sec = 0; }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    nodelay(win_, false);
    ++today_pomos_;
    print_state(min, sec, "Break time:");

    // end
    std::system(kNotifySoundCmd);
}

void timer::SimplePomoTimer::print_state(int min, int sec, std::string state) {
    static std::string bar{};
    int n_blocks = 28 * today_pomos_ / max_pomos_;
    while (bar.size() < n_blocks * 2)
        bar += "█";

    std::ostringstream to_print;
    to_print << "┌──────────────────────────┐\n"
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
             << ">> Type 's' start/skip timer\n";
    mvwprintw(win_, 0, 0, "%s", to_print.str().c_str());
    wrefresh(win_);
}

// ask to quit session
bool timer::SimplePomoTimer::end_session_asking() {
    wprintw(win_, ">> You've done a pomo!\n");
    wprintw(win_, ">> Do you want to end you session here?\n");
    wprintw(win_, "       ► 'y' quit\n");
    wprintw(win_, "       ► 'n' continue and start timer\n");
    wrefresh(win_);
    bool quit = false;
    while (1) {
        char r = wgetch(win_);
        if (r == 'y') {
            quit = true;
            break;
        } else if (r == 'n') {
            break;
        }
    }
    wclear(win_);
    return quit;
}
