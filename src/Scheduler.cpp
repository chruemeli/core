#include "Scheduler.h"

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {
    for(int i = 0; i < this->tasks.size(); i++) {
        delete this->tasks.get(i);
    }

    this->tasks.clear();
}

void Scheduler::add(Task* task) {
    if (task) {
        this->tasks.add(task);
    }
}

LList<Task*> Scheduler::calculateNextTasks() {
    // task which will be returned
    LList<Task*> nextTasks = LList<Task*>();

    // loop through all tasks and find all tasks which need to run    
    int tasksNum = this->tasks.size();
    for (int i = 0; i < tasksNum; i++) {
        // remove task from beginning of list
        Task* task = this->tasks.shift();

        if (task != nullptr) {
            if (task->shouldRun()) {
                // if task is able to run, put it in execution list
                nextTasks.add(task);
            } else {
                // if task not ready to run, add to end of list again
                this->tasks.add(task);
            }
        } else {
            delete(task);
        }
    }

    // sort tasks by priority
    {
        using namespace std::placeholders;

        nextTasks.sort(std::bind(&Scheduler::compareTasks, this, _1, _2));
    }

    return nextTasks;
}

int Scheduler::compareTasks(Task *&t1, Task *&t2) {
    return t2->getPriority() - t1->getPriority();
}

void Scheduler::execute() {
    // calculate next tasks
    LList<Task*> nextTasks = this->calculateNextTasks();

    // run all tasks in this round
    int tasksNum = nextTasks.size();
    for(int i = 0; i < tasksNum; i++) {
        Task * task = nextTasks.shift();

        // execute task
        task->run(this);

        // remove task
        delete(task);

        // allow the run of important background tasks in-between
        #if defined(ESP8266) || defined(ESP32)
        yield();
        #else
        delay(1);
        #endif
    }
}