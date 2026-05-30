#pragma once

#include "Scheduler.h"
#include <vector>
#include <algorithm>

class SRTFScheduler : public Scheduler {
public:
    SRTFScheduler();
    void addJob(const Job& job) override;
    Job getNextJob() override;
    bool hasJobs() const override;
    void schedule(int currentTime) override;
    void setJobs(const std::vector<Job>& jobs) override;
    std::string getGanttChart() const override;
    std::string getTimelineLog() const override;
    std::string getStatistics() const override;
    bool shouldPreempt(const Job& currentJob, int currentJobRunTime) override;
    std::string getName() const override;
    ~SRTFScheduler() override;

private:
    std::vector<Job> srtfQueue;
    std::vector<Job> scheduledJobs;
    std::vector<std::string> timelineLog;
};