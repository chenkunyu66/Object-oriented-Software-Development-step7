/**
 * \file TimeManagement.h
 *
 * \author Charles B. Owen
 *
 * This class manages the timing of the simulation
 * \cond
 */


#pragma once

/** Manages the timing of the simulation
 */
class CTimeManagement
{
public:
    CTimeManagement();
    ~CTimeManagement();

    double Update();

    /** Get the simulation time
     * \returns Time in seconds, starting at zero */
    double GetTime() { return mTime; }

private:
    void Initialize();

    bool   mFirstUpdate = true;
    long long mLastTime = 0;    ///< Last time we read the timer
    double mTimeFreq = 0;       ///< Rate the timer updates
    double mTime = 0;           ///< The current time as far as the system is concerned
};

/// \endcond
