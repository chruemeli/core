#ifndef TimedTask_h
#define TimedTask_h

#include "Scheduler.h"

class TimedTask : public Task {
    public:
        typedef std::function<int()> Callback;
        
    private:
        long nextRun = 0;

        Callback *callback;

    public:
        TimedTask(Callback *cb);
        TimedTask(int ms, Callback *callback);
        TimedTask(int ms, int priority, Callback *callback);
        ~TimedTask();

        bool shouldRun() override;
        void run(Scheduler * scheduler) override;
};

#endif