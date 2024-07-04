#include <chrono>
#include <iostream>

using namespace std::chrono;
#define HOUR_IN_MICROSECONDS (3600 * 1000000LL)
#define MINUTE_IN_MICROSECONDS (60 * 1000000)
#define SECOND_IN_MICROSECONDS 1000000
#define MILLI_IN_MICROSECONDS 1000
class Timer {
private:
    time_point<high_resolution_clock> start_time;

public:
    Timer()
        : start_time(high_resolution_clock::now())
    {
    }

    ~Timer()
    {
        auto end_time = high_resolution_clock::now();
        auto totalTime = duration_cast<microseconds>(end_time - start_time).count();
        auto hours_ = totalTime / HOUR_IN_MICROSECONDS;
        auto minutes_ = (totalTime - hours_ * HOUR_IN_MICROSECONDS) / MINUTE_IN_MICROSECONDS;
        auto seconds_
            = (totalTime - hours_ * HOUR_IN_MICROSECONDS - minutes_ * MINUTE_IN_MICROSECONDS) / SECOND_IN_MICROSECONDS;
        auto milliseconds_ = (totalTime - hours_ * HOUR_IN_MICROSECONDS - minutes_ * MINUTE_IN_MICROSECONDS
                                 - seconds_ * SECOND_IN_MICROSECONDS)
            / MILLI_IN_MICROSECONDS;

        std::cout << hours_ << "h " << minutes_ << "min " << seconds_ << "s " << milliseconds_ << "ms " << totalTime % 1000
                  << "us" << std::endl;
    }
};