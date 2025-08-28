#pragma once

#include <vector>
#include <memory>
#include "Scheduler.h"
#include "Job.h"

class Simulator {
public:
    Simulator(std::unique_ptr<Scheduler> scheduler, std::vector<Job> jobs);
    void run();
    void reportMetrics() const;
    void printGanttChart() const;

private:
    int currentTime;
    std::unique_ptr<Scheduler> scheduler;
    std::vector<Job> allJobs;
    std::vector<Job> finishedJobs;
    std::vector<std::pair<int, int>> ganttChart; // (jobId, time)
};