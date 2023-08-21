#ifndef SIMPLEPOMOTIMER_H
#define SIMPLEPOMOTIMER_H

#include <fstream>
#include <atomic>
#include <curses.h>

namespace timer {

static constexpr char kNotifySoundCmd[] = "notify-send \"Time's up!\" && paplay \
                                           ${HOME}/Desktop/random/pomodoro/bell.wav";

class SimplePomoTimer {
 public:
    SimplePomoTimer(int w, int b, int p, int tp=0, char* fn=nullptr, char* d=nullptr);

    SimplePomoTimer(const SimplePomoTimer&) = delete;
    SimplePomoTimer& operator=(const SimplePomoTimer&) = delete;
    SimplePomoTimer(SimplePomoTimer&&) = delete;
    SimplePomoTimer& operator=(SimplePomoTimer&&) = delete;

    // non mod
    int get_work_min() const { return work_min_; }
    int get_break_min() const { return break_min_; }
    int get_max_pomo() const { return max_pomos_; }
    int get_today_pomos() const { return today_pomos_; }

    // mod
    void set_work_min(int n) { work_min_ = n; }
    void set_break_min(int n) { break_min_ = n; }
    void set_max_pomo(int n) { max_pomos_ = n; }
    void set_today_pomos(int n) { today_pomos_ = n; }
 
    // start study session
    void start();

    ~SimplePomoTimer() {
        delwin(win_);
        endwin(); // close curses
        if(calendar_.is_open()) {
            calendar_ << "dp: " << today_pomos_ << "/"
                      << max_pomos_ << std::endl;
            calendar_.close();
        }
    }

 private:
    int work_min_;           // working time minutes
    int break_min_;          // break time minutes
    int max_pomos_;          // max pomos in a row
    int today_pomos_;        // pomo counter
    std::ofstream calendar_; // save history
    WINDOW* win_;            // output timer
     
    // do a complete pomo: 1 work and 1 break
    void one_pomo();           

    // ask user for quitting
    bool end_session_asking(); 

    // print timer and settings
    void print_state(int min, int sec, std::string); 
};

} // end timer

#endif
