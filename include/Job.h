#pragma once

#include <iostream>

class Job {
public:
    int jobId;
    std::string name;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int startTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;

    Job(int id, int arrival, int burst, int prio = 0);
    Job(const std::string& name, int arrival, int burst, int prio = 0);

    // Getters
    std::string getName() const { return name; }
    int getArrivalTime() const { return arrivalTime; }
    int getBurstTime() const { return burstTime; }
    int getPriority() const { return priority; }

    // Setters
    void setArrivalTime(int arrival) { arrivalTime = arrival; }
    void setBurstTime(int burst) { burstTime = burst; }
    void setPriority(int prio) { priority = prio; }
    void setName(const std::string& n) { name = n; }

    void calculateMetrics();
    void display() const;
};