// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef GENERICOBSERVER_H_
#define GENERICOBSERVER_H_


#include "CommonDefs.h"
#include "Arduino.h"


namespace eskates {
namespace core {

class BaseObserver
{
public:
    virtual void enable(void) = 0;

    virtual void disable(void) = 0;

    virtual ObserverId_t getObserverId(void) const = 0;

    virtual bool hasPinChanged(void) = 0;

    virtual void executeCallback(void) const = 0;

    virtual bool isEnabled(void) const = 0;
};

template<typename T>
class GenericObserver : public BaseObserver {
public:
    typedef void(T::* PinChangeCallback)(Pin_t, int16_t);
    GenericObserver(T* classPtr, PinChangeCallback callbackFunction, Pin_t pin)
        : mClassPtr(classPtr)
        , mPinChangeCallback(callbackFunction)
        , mObserverId(getNextId())
        , mPin(pin)
        , mLastState(LOW)
        , mEnabled(false)
    {
        return;
    }

    ~GenericObserver()
    {
        return;
    }

    void enable() override
    {
        mEnabled = true;
        mLastState = digitalRead(mPin);
    }

    void disable(void) override
    {
        mEnabled = false;
    }

    ObserverId_t getObserverId(void) const override
    {
        return mObserverId;
    }

    void executeCallback(void) const override
    {
        (mClassPtr->*mPinChangeCallback)(mPin, mLastState);
    }

    bool hasPinChanged(void)
    {
        int16_t state = digitalRead(mPin);
        bool changed = (state != mLastState);
        mLastState = state;
        return changed;
    }

    bool isEnabled(void) const override
    {
        return mEnabled;
    }

private:
    ObserverId_t getNextId(void)
    {
        return NEXT_AVAILABLE_OBSERVER_ID++;
    }

    T* mClassPtr;
    PinChangeCallback mPinChangeCallback;
    ObserverId_t mObserverId;
    Pin_t mPin;
    int16_t mLastState;
    bool mEnabled;
    static ObserverId_t NEXT_AVAILABLE_OBSERVER_ID;
};

template<typename T>
ObserverId_t GenericObserver<T>::NEXT_AVAILABLE_OBSERVER_ID = 0;

}
}
#endif // OBSERVER_H_