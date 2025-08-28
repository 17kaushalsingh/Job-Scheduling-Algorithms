#pragma once

#include "Scheduler.h"
#include <vector>
#include <algorithm>

class PriorityScheduler : public Scheduler {
public:
    PriorityScheduler(int agingThreshold = 5, int agingIncrement = 1);
    void addJob(const Job& job) override;
    Job getNextJob() override;
    bool hasJobs() const override;
    void schedule(int currentTime) override;
    void setJobs(const std::vector<Job>& jobs) override;
    std::string getGanttChart() const override;
    std::string getTimelineLog() const override;
    std::string getStatistics() const override;
    ~PriorityScheduler() override;

private:
    std::vector<Job> priorityQueue;
    std::vector<Job> scheduledJobs;
    std::vector<std::string> timelineLog;
    int agingThreshold;
    int agingIncrement;
    void applyAging(int currentTime);
};