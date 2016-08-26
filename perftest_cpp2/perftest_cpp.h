/* ===================================================================
 (c) 2005-2012  Copyright, Real-Time Innovations, Inc.  All rights reserved.
 Permission to modify and use for internal purposes granted.
 This software is provided "as is", without warranty, express or implied.

 Modification History
 --------------------
 5.2.0,22jul15,jm  PERFTEST-90 The getTime function uses now our high precision clock.
 5.2.0,21jul15,jm  PERF-53 Changes for CR-789.
 5.2.0,27apr14,jm  PERFTEST-86 Removing .ini support. Fixing warnings.
 5.2.0,03nov14,jm  PERF-53 Created. Using ../perftest_cpp as template for the
                   Product behavior.
===================================================================== */

#ifndef __PERFTEST_CPP_H__
#define __PERFTEST_CPP_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <iostream>

#include "clock/clock_highResolution.h"
#include "osapi/osapi_ntptime.h"

#include "RTIDDSImpl.h"
#include "MessagingIF.h"
#include "test.hpp"

#ifdef RTI_WIN32
  #include <windows.h>
#else
  #include <sys/time.h>
  #include <sched.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <signal.h>
#endif

#include "MessagingIF.h"

class perftest_cpp
{
  public:
    perftest_cpp();
    ~perftest_cpp();

    int Run(int argc, char *argv[]);
    bool ParseConfig(int argc, char *argv[]);
    
  private:
    int RunPublisher();
    int RunSubscriber();

  public:
    static void MilliSleep(unsigned int millisec) {
  #ifdef RTI_WIN32
        Sleep(millisec);
  #else
        usleep(millisec * 1000);
  #endif
    }

    static void ThreadYield() {
  #ifdef RTI_WIN32
        Sleep(0);
  #else
        sched_yield();
  #endif
    }

  private:
    int  _DataLen;
    int  _BatchSize;
    int  _maxBinDataSize;
    int  _SamplesPerBatch;
    unsigned long long _NumIter;
    bool _IsPub;
    bool _IsScan;
    bool _UseReadThread;
    unsigned long long _SpinLoopCount;
    int  _SleepMillisec;
    int  _LatencyCount;
    int  _NumSubscribers;
    int  _NumPublishers;
    int _InstanceCount;
    IMessaging *_MessagingImpl;
    char **_MessagingArgv;
    int _MessagingArgc;
    bool _LatencyTest;
    bool _IsReliable;
    int _pubRate;
    bool _isKeyed;
    unsigned int _executionTime;

  private:
    static char * StringDup(const char * str);
    static void SetTimeout(unsigned int executionTimeInSeconds);

    /* The following three members are used in a static callback
       and so they have to be static */
    static bool _testCompleted;
  #ifdef RTI_WIN32
    static HANDLE _hTimerQueue;
    static HANDLE _hTimer;
  #endif

  public:
    static int  _SubID;
    static int  _PubID;
    static bool _PrintIntervals;
    static bool _IsDebug;
    static struct RTIClock *clock;
    static const char *_LatencyTopicName;
    static const char *_ThroughputTopicName;
    static const char *_AnnouncementTopicName;

  #ifdef RTI_WIN32
    static LARGE_INTEGER _ClockFrequency;
  #endif
    
    // Number of bytes sent in messages besides user data
    static const int OVERHEAD_BYTES = 28;
    
    // When running a scan, this determines the number of
    // latency pings that will be sent before increasing the 
    // data size
    static const int NUM_LATENCY_PINGS_PER_DATA_SIZE = 1000;
    
    // Flag used to indicate message is used for initialization only
    static const int INITIALIZE_SIZE = 1234;
    // Flag used to indicate end of test
    static const int FINISHED_SIZE = 1235;
    // Flag used to data packet length is changing
    static const int LENGTH_CHANGED_SIZE = 1236;

   public:
    static unsigned long long GetTimeUsec(struct RTINtpTime *now);


  #ifdef RTI_WIN32
    static VOID CALLBACK Timeout(PVOID lpParam, BOOLEAN timerOrWaitFired);
  #else
    static void Timeout(int sign);
  #endif

};

#endif // __PERFTEST_CPP_H__