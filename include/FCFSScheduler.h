#pragma once

#include "Scheduler.h"
#include <queue>

class FCFSScheduler : public Scheduler {
public:
    FCFSScheduler();
    void addJob(const Job& job) override;
    Job getNextJob() override;
    bool hasJobs() const override;
    void schedule(int currentTime) override;
    void setJobs(const std::vector<Job>& jobs) override;
    std::string getGanttChart() const override;
    std::string getTimelineLog() const override;
    std::string getStatistics() const override;
    ~FCFSScheduler() override;

private:
    std::queue<Job> fcfsQueue;
    std::vector<Job> scheduledJobs;
    std::vector<std::string> timelineLog;
};