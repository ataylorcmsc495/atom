#pragma once

#include <chrono>

using namespace std::chrono;

class GameClock {
  private:
    float deltaTime = 0.0;
    
    time_point<high_resolution_clock> lastTime = now();

    time_point<high_resolution_clock> now() {
        return high_resolution_clock::now();
    }

    void setDeltaTime() {
        deltaTime = duration_cast<duration<float>>(now() - lastTime).count();
    }
   
    void updateLastTime() {
        lastTime = now();
    }

  public:
    float refreshDeltaTime() {
        setDeltaTime();
        updateLastTime();
        return getDeltaTime();
    }
    float getDeltaTime() {
        return deltaTime;
    }
};
