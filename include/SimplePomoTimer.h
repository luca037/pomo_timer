#ifndef SIMPLEPOMOTIMER_H
#define SIMPLEPOMOTIMER_H

#include <fstream>
#include <atomic>

namespace timer {

static constexpr char kNotifySoundCmd[] = "notify-send \"Time's up!\" && paplay \
                                           ${HOME}/Desktop/random/pomodoro/bell.wav";

class SimplePomoTimer {
 public:
    SimplePomoTimer(int w, int b, int p, int tp=0, char* fn=nullptr, char* d=nullptr);

    // start session
    void start();

    ~SimplePomoTimer() {
        if(calendar_.is_open()) {
            calendar_ << "dp: " << today_pomos_ << "/"
                      << max_pomos_ << std::endl;
            calendar_.close();
        }
    }

    SimplePomoTimer(const SimplePomoTimer&) = delete;
    SimplePomoTimer& operator=(const SimplePomoTimer&) = delete;

 private:
    int work_min_;           // working time minutes
    int break_min_;          // break time minutes
    int max_pomos_;          // max pomos in a row
    int today_pomos_;        // pomo counter
    std::ofstream calendar_; // save history
     
    void one_pomo();           // do a complete pomo: work and break
    bool end_session_asking(); // ask user for quitting

    void print_state(int min, int sec, std::string); // print timer and settings
};

void skip_timer(std::atomic_bool* s); // check if user want to skip a timer loop

} // end timer

#endif
