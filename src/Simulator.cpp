#include "../include/Simulator.h"
#include <iostream>
#include <iomanip>

Simulator::Simulator(std::unique_ptr<Scheduler> sched, std::vector<Job> jobs)
    : currentTime(0), scheduler(std::move(sched)), allJobs(std::move(jobs)) {}

#include <optional>

void Simulator::run() {
    std::optional<Job> currentJob;
    int currentJobRunTime = 0;

    while (!allJobs.empty() || scheduler->hasJobs() || currentJob.has_value()) {
        // Add jobs that have arrived to the scheduler
        for (auto it = allJobs.begin(); it != allJobs.end();) {
            if (it->arrivalTime <= currentTime) {
                scheduler->addJob(*it);
                it = allJobs.erase(it);
            } else {
                ++it;
            }
        }
        
        scheduler->schedule(currentTime);

        // Preemption check
        if (currentJob.has_value()) {
            if (scheduler->shouldPreempt(currentJob.value(), currentJobRunTime)) {
                scheduler->addJob(currentJob.value());
                currentJob.reset();
            }
        }

        // Get next job if idle
        if (!currentJob.has_value() && scheduler->hasJobs()) {
            currentJob = scheduler->getNextJob();
            currentJobRunTime = 0;
            if (currentJob->startTime == -1) currentJob->startTime = currentTime;
        }

        // Execute current job
        if (currentJob.has_value()) {
            ganttChart.push_back({currentJob->jobId, currentTime});
            currentJob->remainingTime--;
            currentJobRunTime++;
            currentTime++;

            if (currentJob->remainingTime == 0) {
                currentJob->completionTime = currentTime;
                currentJob->calculateMetrics();
                finishedJobs.push_back(currentJob.value());
                currentJob.reset();
            }
        } else {
            currentTime++;
        }
    }
}

void Simulator::reportMetrics() const {
    double totalTurnaround = 0, totalWaiting = 0;
    for (const auto& job : finishedJobs) {
        job.display();
        totalTurnaround += job.turnaroundTime;
        totalWaiting += job.waitingTime;
    }
    int n = finishedJobs.size();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Turnaround Time: " << (n ? totalTurnaround / n : 0) << std::endl;
    std::cout << "Average Waiting Time: " << (n ? totalWaiting / n : 0) << std::endl;
}

void Simulator::printGanttChart() const {
    std::cout << "Gantt Chart:\n|";
    for (const auto& entry : ganttChart) {
        std::cout << " J" << entry.first << " |";
    }
    std::cout << std::endl << " ";
    for (size_t i = 0; i <= ganttChart.size(); ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl;
}