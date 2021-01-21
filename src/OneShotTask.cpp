#include "OneShotTask.h"

OneShotTask::OneShotTask(Callback cb) {
    this->priority = 0;
    this->callback = cb;
}

OneShotTask::OneShotTask(int priority, Callback cb) {
    this->priority = priority;
    this->callback = cb;
}

bool OneShotTask::shouldRun() {
    return true;
}

void OneShotTask::run(Scheduler * scheduler) {
    this->callback();
}