#pragma once

#include "Scheduler.h"
#include <queue>

class RoundRobinScheduler : public Scheduler {
public:
    RoundRobinScheduler(int quantum);
    void addJob(const Job& job) override;
    Job getNextJob() override;
    bool hasJobs() const override;
    void schedule(int currentTime) override;
    void setJobs(const std::vector<Job>& jobs) override;
    std::string getGanttChart() const override;
    std::string getTimelineLog() const override;
    std::string getStatistics() const override;
    ~RoundRobinScheduler() override;

private:
    std::queue<Job> rrQueue;
    std::vector<Job> scheduledJobs;
    std::vector<std::string> timelineLog;
    int timeQuantum;
};