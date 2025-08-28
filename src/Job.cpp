#include "../include/Job.h"

Job::Job(int id, int arrival, int burst, int prio)
    : jobId(id), arrivalTime(arrival), burstTime(burst), priority(prio),
      remainingTime(burst), startTime(-1), completionTime(-1),
      waitingTime(0), turnaroundTime(0) {}

Job::Job(const std::string& name, int arrival, int burst, int prio)
    : jobId(-1), name(name), arrivalTime(arrival), burstTime(burst), priority(prio),
      remainingTime(burst), startTime(-1), completionTime(-1),
      waitingTime(0), turnaroundTime(0) {}

void Job::calculateMetrics() {
    turnaroundTime = completionTime - arrivalTime;
    waitingTime = turnaroundTime - burstTime;
}

void Job::display() const {
    std::cout << "Job Name: " << name
              << " | Job ID: " << jobId
              << " | Arrival: " << arrivalTime
              << " | Burst: " << burstTime
              << " | Priority: " << priority
              << " | Start: " << startTime
              << " | Completion: " << completionTime
              << " | Waiting: " << waitingTime
              << " | Turnaround: " << turnaroundTime
              << std::endl;
}