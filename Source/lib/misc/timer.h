#pragma once

class Timer
{
public:
    void start() {
        m_StartTime = std::chrono::system_clock::now();
        running = true;
    }

    void stop() {
        m_EndTime = std::chrono::system_clock::now();
        running = false;
    }

    double elapsedMilliseconds() {
        std::chrono::time_point<std::chrono::system_clock> endTime;
        if (running)
            endTime = std::chrono::system_clock::now();
        else
            endTime = m_EndTime;
        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
    }

    double elapsedSeconds() {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool running = false;
};