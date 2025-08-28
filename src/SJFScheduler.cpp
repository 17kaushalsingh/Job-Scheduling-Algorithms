#include "../include/SJFScheduler.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

SJFScheduler::SJFScheduler() {}

void SJFScheduler::addJob(const Job& job) {
    sjfQueue.push_back(job);
}

Job SJFScheduler::getNextJob() {
    if (sjfQueue.empty()) {
        return Job(-1, 0, 0, 0);
    }
    // Find job with minimum remainingTime
    auto it = std::min_element(sjfQueue.begin(), sjfQueue.end(),
        [](const Job& a, const Job& b) {
            return a.remainingTime < b.remainingTime;
        });
    Job job = *it;
    sjfQueue.erase(it);
    return job;
}

bool SJFScheduler::hasJobs() const {
    return !sjfQueue.empty();
}

void SJFScheduler::schedule(int currentTime) {
    // SJF: sort jobs by remainingTime, only those that have arrived
    std::sort(sjfQueue.begin(), sjfQueue.end(),
        [](const Job& a, const Job& b) {
            return a.remainingTime < b.remainingTime;
        });
}

SJFScheduler::~SJFScheduler() {}

void SJFScheduler::setJobs(const std::vector<Job>& jobs) {
    sjfQueue = jobs;
    std::sort(sjfQueue.begin(), sjfQueue.end(), [](const Job& a, const Job& b) {
        return a.burstTime < b.burstTime;
    });
    scheduledJobs.clear();
    timelineLog.clear();
}

std::string SJFScheduler::getGanttChart() const {
    std::ostringstream oss;
    const std::string colors[] = { "\033[41m", "\033[42m", "\033[43m", "\033[44m", "\033[45m", "\033[46m", "\033[47m" };
    oss << "Gantt Chart:\n";
    oss << "Time:   ";
    int time = 0;
    for (const auto& job : scheduledJobs) {
        oss << std::setw(4) << time << " ";
        time += job.burstTime;
    }
    oss << std::setw(4) << time << "\n";
    oss << "        ";
    int colorIdx = 0;
    time = 0;
    for (const auto& job : scheduledJobs) {
        std::string color = colors[colorIdx % 7];
        oss << color << " " << job.getName() << " " << "\033[0m" << " ";
        colorIdx++;
        time += job.burstTime;
    }
    oss << "\n";
    oss << "Labels: ";
    for (const auto& job : scheduledJobs) {
        oss << "[" << job.getName() << ":A=" << job.getArrivalTime() << ",B=" << job.burstTime << "] ";
    }
    oss << "\n";
    return oss.str();
}

std::string SJFScheduler::getTimelineLog() const {
    std::ostringstream oss;
    oss << "Timeline Log:\n";
    for (const auto& entry : timelineLog) {
        oss << entry << "\n";
    }
    oss << "Legend: [A]=Arrival, [S]=Start, [C]=Completion, [P]=Preemption\n";
    return oss.str();
}

std::string SJFScheduler::getStatistics() const {
    std::ostringstream oss;
    double totalWT = 0, totalTT = 0;
    int count = scheduledJobs.size();
    oss << std::left << std::setw(10) << "Job"
        << std::setw(10) << "WT"
        << std::setw(10) << "TT" << "\n";
    for (const auto& job : scheduledJobs) {
        oss << std::setw(10) << job.getName()
            << std::setw(10) << job.waitingTime
            << std::setw(10) << job.turnaroundTime << "\n";
        totalWT += job.waitingTime;
        totalTT += job.turnaroundTime;
    }
    if (count > 0) {
        oss << "Avg WT: " << totalWT / count << "\n";
        oss << "Avg TT: " << totalTT / count << "\n";
    }
    oss << "Total Jobs: " << count << "\n";
    oss << "Total Burst Time: ";
    int totalBurst = 0;
    for (const auto& job : scheduledJobs) totalBurst += job.burstTime;
    oss << totalBurst << "\n";
    oss << "Algorithm: SJF\n";
    oss << "Compare with other algorithms in Statistics menu.\n";
    return oss.str();
}