#pragma once

#include <vector>
#include "Job.h"

class Scheduler {
public:
    virtual void addJob(const Job& job) = 0;
    virtual Job getNextJob() = 0;
    virtual bool hasJobs() const = 0;
    virtual void schedule(int currentTime) = 0;
    virtual void setJobs(const std::vector<Job>& jobs) = 0;
    virtual std::string getGanttChart() const = 0;
    virtual std::string getTimelineLog() const = 0;
    virtual std::string getStatistics() const = 0;
    virtual ~Scheduler() {}

protected:
    std::vector<Job> jobQueue;
};