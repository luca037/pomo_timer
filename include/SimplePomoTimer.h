#ifndef SIMPLEPOMOTIMER_H
#define SIMPLEPOMOTIMER_H

#include <fstream>

namespace timer {

    static constexpr char kNotifySoundCmd[] = "notify-send \"Time's up!\" && paplay ../mixkit-gaming-lock-2848.wav";

class SimplePomoTimer {
public:
    SimplePomoTimer(int w, int b, int p, char* fn=nullptr, char* d=nullptr);

    // do a complete pomo: work and break
    void one_pomo();

    // return true if max number pomos equals today pomos
    bool done() { return max_pomos_ == today_pomos_; }

    ~SimplePomoTimer() {
        if(calendar_.is_open()) {
            calendar_ << "dp: " << today_pomos_ << "/" << max_pomos_ << std::endl;
            calendar_.close();
        }
    }

    SimplePomoTimer(const SimplePomoTimer&) = delete;
    SimplePomoTimer& operator=(const SimplePomoTimer&) = delete;

private:
    int work_min_; // working time minutes
    int break_min_; // break time minutes
    int max_pomos_; // max pomos in a row
    int today_pomos_; // pomo counter
    bool is_working_; // current state
    std::ofstream calendar_;
    void print_state(int min, int sec); // print timer and info state
    void wait_start(); // wait user input
};

void skip_timer(bool* s); // check if user want to skip a timer loop

} // end namespace

#endif
