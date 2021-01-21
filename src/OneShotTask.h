#ifndef OneShotTask_h
#define OneShotTask_h

#include "Scheduler.h"

class OneShotTask : public Task {
    public:
        typedef std::function<void()> Callback;
        
    private:
        Callback callback;

    public:
        OneShotTask(Callback cb);
        OneShotTask(int priority, Callback callback);
        ~OneShotTask();

        bool shouldRun() override;
        void run(Scheduler * scheduler) override;
};

#endif