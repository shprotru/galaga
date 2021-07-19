#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

namespace TIMER {
    class timer
    {
        Uint32 mStartTicks;
        Uint32 mElapsedTicks;
        double mTimeScale;
        double mDeltaTime;
    public:
        timer();

        void reset();
        void update();
        double delta();
        void timeScale(double t);
        double timeScale();
    };
}

#endif // TIMER_H
