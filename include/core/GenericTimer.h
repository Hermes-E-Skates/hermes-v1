// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef GENERIC_TIMER_H_
#define GENERIC_TIMER_H_


#include "CommonDefs.h"
#include "Arduino.h"


namespace hermes {
namespace core {

class BaseTimer
{
public:
    virtual void stop(void) = 0;

    virtual TimerId_t getTimerId(void) const = 0;

    virtual void executeCallback(void) const = 0;

    virtual bool hasExpired(void) = 0;

    virtual bool isEnabled(void) const = 0;

    virtual TimerType_t getType(void) const = 0;
};

template<typename T>
class GenericTimer : public BaseTimer {
public:
    typedef void(T::* TimerCallback)(uint32_t);

    GenericTimer(T* classPtr, TimerCallback callbackFunction)
        : mClassPtr(classPtr)
        , mTimerCallbackFunction(callbackFunction)
        , mTimerId(getNextId())
        , mType(PERIODIC)
        , mPeriod(0U)
        , mUserData(0U)
        , mLastExpired(0UL)
        , mEnabled(false) {
        return;
    }

    ~GenericTimer()
    {
        return;
    }

    void stop(void) override
    {
        mEnabled = false;
    }

    TimerId_t getTimerId(void) const override
    {
        return mTimerId;
    }

    uint32_t getPeriod(void) const
    {
        return mPeriod / 1000U;
    }

    void executeCallback(void) const override
    {
        (mClassPtr->*mTimerCallbackFunction)(mUserData);
    }

    bool hasExpired(void) override
    {
        bool expired = false;

        uint64_t currentTime = micros();
        if (currentTime - mLastExpired > mPeriod)
        {
            mLastExpired = currentTime;
            expired = true;
        }

        return expired;
    }

    bool isEnabled(void) const override
    {
        return mEnabled;
    }

    TimerType_t getType(void) const override
    {
        return mType;
    }

    void start(uint32_t period, TimerType_t type, uint32_t userData = 0)
    {
        startMicros(period * 1000, type, userData);
        return;
    }

    void startMicros(uint32_t period, TimerType_t type, uint32_t userData = 0)
    {
        mPeriod = period;
        mType = type;
        mUserData = userData;
        mEnabled = true;
        mLastExpired = micros();
        return;
    }

    void restart(void)
    {
        mLastExpired = micros();
    }
private:
    TimerId_t getNextId(void) const
    {
        return NEXT_AVAILABLE_TIMER_ID++;
    }

    T* mClassPtr;
    TimerCallback mTimerCallbackFunction;
    TimerId_t mTimerId;
    TimerType_t mType;
    uint32_t mPeriod;
    uint32_t mUserData;
    uint64_t mLastExpired;
    bool mEnabled;
    static TimerId_t NEXT_AVAILABLE_TIMER_ID;
};

template<typename T>
TimerId_t GenericTimer<T>::NEXT_AVAILABLE_TIMER_ID = 0;
}
}

#endif