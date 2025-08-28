#include "../include/FCFSScheduler.h"
#include <sstream>
#include <iomanip>

FCFSScheduler::FCFSScheduler() {}

void FCFSScheduler::addJob(const Job& job) {
    fcfsQueue.push(job);
}

Job FCFSScheduler::getNextJob() {
    if (!fcfsQueue.empty()) {
        Job job = fcfsQueue.front();
        fcfsQueue.pop();
        return job;
    }
    // Return a dummy job if queue is empty (should be handled by caller)
    return Job(-1, 0, 0, 0);
}

bool FCFSScheduler::hasJobs() const {
    return !fcfsQueue.empty();
}

void FCFSScheduler::schedule(int currentTime) {
    // FCFS does not require sorting or preemption
}

FCFSScheduler::~FCFSScheduler() {}

void FCFSScheduler::setJobs(const std::vector<Job>& jobs) {
    while (!fcfsQueue.empty()) fcfsQueue.pop();
    for (const auto& job : jobs) {
        fcfsQueue.push(job);
    }
    scheduledJobs.clear();
    timelineLog.clear();
}

std::string FCFSScheduler::getGanttChart() const {
    std::ostringstream oss;
    // ANSI color codes for jobs
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

std::string FCFSScheduler::getTimelineLog() const {
    std::ostringstream oss;
    oss << "Timeline Log:\n";
    for (const auto& entry : timelineLog) {
        // Example entry: "JobA: Arrived at 0, Started at 2, Completed at 5"
        oss << entry << "\n";
    }
    oss << "Legend: [A]=Arrival, [S]=Start, [C]=Completion, [P]=Preemption\n";
    return oss.str();
}

std::string FCFSScheduler::getStatistics() const {
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
    // Aggregate metrics
    oss << "Total Jobs: " << count << "\n";
    oss << "Total Burst Time: ";
    int totalBurst = 0;
    for (const auto& job : scheduledJobs) totalBurst += job.burstTime;
    oss << totalBurst << "\n";
    // Algorithm comparison placeholder
    oss << "Algorithm: FCFS\n";
    oss << "Compare with other algorithms in Statistics menu.\n";
    return oss.str();
}