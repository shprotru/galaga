#include "timer.h"

namespace TIMER {
    timer::timer()
    {
        reset();
        mElapsedTicks = 0;
        mDeltaTime = 0.0;
        mTimeScale = 1.0;
    }

    void timer::reset()
    {
        mStartTicks = SDL_GetTicks();
    }

    void timer::update() {
        mElapsedTicks = SDL_GetTicks() - mStartTicks;
        //Converting milliseconds to seconds
        mDeltaTime = mElapsedTicks * 0.001;
    }

    double timer::delta()
    {
        return mDeltaTime;
    }

    void timer::timeScale(double t)
    {
        mTimeScale = t;
    }

    double timer::timeScale()
    {
        return mTimeScale;
    }
}
