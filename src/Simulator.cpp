#include "../include/Simulator.h"
#include <iostream>
#include <iomanip>

Simulator::Simulator(std::unique_ptr<Scheduler> sched, std::vector<Job> jobs)
    : currentTime(0), scheduler(std::move(sched)), allJobs(std::move(jobs)) {}

void Simulator::run() {
    while (!allJobs.empty() || scheduler->hasJobs()) {
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

        if (scheduler->hasJobs()) {
            Job job = scheduler->getNextJob();
            if (job.startTime == -1) job.startTime = currentTime;
            ganttChart.push_back({job.jobId, currentTime});
            job.remainingTime--;
            currentTime++;
            if (job.remainingTime == 0) {
                job.completionTime = currentTime;
                job.calculateMetrics();
                finishedJobs.push_back(job);
            } else {
                scheduler->addJob(job);
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