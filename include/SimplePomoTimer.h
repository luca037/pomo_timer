#ifndef SIMPLEPOMOTIMER_H
#define SIMPLEPOMOTIMER_H

#include <fstream>

namespace timer {

class SimplePomoTimer {
public:
    SimplePomoTimer(int w, int b, int p, char* fn=nullptr, char* d=nullptr);
    SimplePomoTimer(const SimplePomoTimer&) = delete;
    SimplePomoTimer& operator=(const SimplePomoTimer&) = delete;

    void one_pomo();
    bool done() { return max_pomos_ == today_pomos_; }

    ~SimplePomoTimer() {
        if(calendar_.is_open()) {
            calendar_ << "dp: " << today_pomos_ << "/" << max_pomos_ << std::endl;
            calendar_.close();
        }
    }

    static constexpr char kNotifySoundCmd[] = "notify-send \"Time's up! Go back to work\" && paplay ../mixkit-gaming-lock-2848.wav";

private:
    int work_min_; // working time minutes
    int break_min_; // break time minutes
    int max_pomos_; // max pomos in a row
    int today_pomos_;
    std::ofstream calendar_;
    void print_state(int min, int sec, bool w); // print timer and info state
    void wait_input(); // wait user input
};


} // end namespace

#endif
