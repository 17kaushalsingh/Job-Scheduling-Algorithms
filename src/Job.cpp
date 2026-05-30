#include "../include/Job.h"

Job::Job() : jobId(-1), arrivalTime(0), burstTime(0), priority(0),
             remainingTime(0), startTime(-1), completionTime(-1),
             waitingTime(0), turnaroundTime(0) {}

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

std::string Job::serialize() const {
    return std::to_string(jobId) + "," + name + "," + std::to_string(arrivalTime) + "," +
           std::to_string(burstTime) + "," + std::to_string(priority);
}

void Job::deserialize(const std::string& data) {
    size_t pos1 = data.find(',');
    size_t pos2 = data.find(',', pos1 + 1);
    size_t pos3 = data.find(',', pos2 + 1);
    size_t pos4 = data.find(',', pos3 + 1);
    
    if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos && pos4 != std::string::npos) {
        jobId = std::stoi(data.substr(0, pos1));
        name = data.substr(pos1 + 1, pos2 - pos1 - 1);
        arrivalTime = std::stoi(data.substr(pos2 + 1, pos3 - pos2 - 1));
        burstTime = std::stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
        priority = std::stoi(data.substr(pos4 + 1));
        remainingTime = burstTime;
        startTime = -1;
        completionTime = -1;
        waitingTime = 0;
        turnaroundTime = 0;
    }
}