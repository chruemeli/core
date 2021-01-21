#ifndef Scheduler_h
#define Scheduler_h

#include <Arduino.h>

#include "LList.h"
#include "Task.h"

class Scheduler {
    private:
        LList<Task*> tasks = LList<Task*>();

        LList<Task*> calculateNextTasks();
    public:
        Scheduler();
        ~Scheduler();

        void execute();
        
        int compareTasks(Task *&t1, Task *&t2);

        void add(Task* task);
};

#endif