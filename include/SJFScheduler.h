#pragma once

#include "Scheduler.h"
#include <vector>
#include <algorithm>

class SJFScheduler : public Scheduler {
public:
    SJFScheduler();
    void addJob(const Job& job) override;
    Job getNextJob() override;
    bool hasJobs() const override;
    void schedule(int currentTime) override;
    void setJobs(const std::vector<Job>& jobs) override;
    std::string getGanttChart() const override;
    std::string getTimelineLog() const override;
    std::string getStatistics() const override;
    ~SJFScheduler() override;

private:
    std::vector<Job> sjfQueue;
    std::vector<Job> scheduledJobs;
    std::vector<std::string> timelineLog;
};