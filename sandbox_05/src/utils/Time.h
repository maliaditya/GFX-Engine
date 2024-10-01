#include <chrono>

// Type aliases for easier usage
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

class Timer {
public:
    Timer() : lastTime(Clock::now()) {}

    float getElapsedTime() {
        TimePoint currentTime = Clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime; // Get the elapsed time
        lastTime = currentTime; // Update lastTime for the next frame
        return elapsed.count(); // Return elapsed time in seconds
    }

private:
    TimePoint lastTime; // Store the last time point
};