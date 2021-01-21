#ifndef Task_h
#define Task_h

#include <Arduino.h>

class Scheduler;
class Task {
    protected:
        int priority;

    public:
        // needs to be implemented by the task
        virtual bool shouldRun() = 0;
        virtual void run(Scheduler * scheduler) = 0;

        int getPriority() {
            return this->priority;
        };
};



#endif