#ifndef ContinousTask_h
#define ContinousTask_h

#include "Scheduler.h"

class ContinousTask : public Task {
    public:
        typedef std::function<void()> Callback;

    private:
        Callback *callback;

    public:
        ContinousTask(Callback * cb);
        ContinousTask(int prio, Callback * cb);

        bool shouldRun() override;
        void run(Scheduler * scheduler) override;
};

#endif