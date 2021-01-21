#include "ContinousTask.h"

ContinousTask::ContinousTask(Callback * cb) {
    this->priority = 0;
    this->callback = cb;
}

ContinousTask::ContinousTask(int prio, Callback * cb) {
    this->priority = prio;
    this->callback = cb;
}

bool ContinousTask::shouldRun() {
    return true;
}

void ContinousTask::run(Scheduler * scheduler) {
    if(this->callback != nullptr) {
        (*this->callback)();

        scheduler->add(new ContinousTask(this->priority, this->callback));
    }
}

