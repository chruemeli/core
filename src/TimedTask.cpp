#include "TimedTask.h"

TimedTask::TimedTask(Callback * cb) {
    this->nextRun = 0;
    this->priority = 0;
    this->callback = cb;
}

TimedTask::TimedTask(int ms, Callback * cb) {
    this->nextRun = ms;
    this->priority = 0;
    this->callback = cb;
}

TimedTask::TimedTask(int ms, int prio, Callback * cb) {
    this->nextRun = ms;
    this->priority = prio;
    this->callback = cb;
}

bool TimedTask::shouldRun() {
    return millis() >= this->nextRun;
}

void TimedTask::run(Scheduler * scheduler) {
    if(this->callback != NULL) {
        int nextRun = (*this->callback)();

        scheduler->add(new TimedTask(millis() + nextRun, this->priority, this->callback));
    }
}