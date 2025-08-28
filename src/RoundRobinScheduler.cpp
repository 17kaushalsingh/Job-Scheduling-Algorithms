#include "../include/RoundRobinScheduler.h"
#include <sstream>
#include <iomanip>
#include <vector>

RoundRobinScheduler::RoundRobinScheduler(int quantum)
    : timeQuantum(quantum) {}

void RoundRobinScheduler::addJob(const Job& job) {
    rrQueue.push(job);
}

Job RoundRobinScheduler::getNextJob() {
    if (rrQueue.empty()) {
        return Job(-1, 0, 0, 0);
    }
    Job job = rrQueue.front();
    rrQueue.pop();
    return job;
}

bool RoundRobinScheduler::hasJobs() const {
    return !rrQueue.empty();
}

void RoundRobinScheduler::schedule(int currentTime) {
    // Round Robin: jobs are processed in timeQuantum slices
    // Actual logic handled in Simulator
}

RoundRobinScheduler::~RoundRobinScheduler() {}

void RoundRobinScheduler::setJobs(const std::vector<Job>& jobs) {
    std::queue<Job> empty;
    std::swap(rrQueue, empty);
    for (const auto& job : jobs) {
        rrQueue.push(job);
    }
    scheduledJobs.clear();
    timelineLog.clear();
}

std::string RoundRobinScheduler::getGanttChart() const {
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

std::string RoundRobinScheduler::getTimelineLog() const {
    std::ostringstream oss;
    oss << "Timeline Log:\n";
    for (const auto& entry : timelineLog) {
        oss << entry << "\n";
    }
    oss << "Legend: [A]=Arrival, [S]=Start, [C]=Completion, [P]=Preemption\n";
    return oss.str();
}

std::string RoundRobinScheduler::getStatistics() const {
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
    oss << "Algorithm: Round Robin\n";
    oss << "Compare with other algorithms in Statistics menu.\n";
    return oss.str();
}