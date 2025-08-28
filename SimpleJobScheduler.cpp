#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

struct Job {
    int jobId, arrivalTime, burstTime, priority;
    int remainingTime, startTime, completionTime, waitingTime, turnaroundTime;
    Job(int id, int arrival, int burst, int prio)
        : jobId(id), arrivalTime(arrival), burstTime(burst), priority(prio),
          remainingTime(burst), startTime(-1), completionTime(-1),
          waitingTime(0), turnaroundTime(0) {}
    void calculateMetrics() {
        turnaroundTime = completionTime - arrivalTime;
        waitingTime = turnaroundTime - burstTime;
    }
    void display() const {
        std::cout << "Job " << jobId << ": Arrival=" << arrivalTime
                  << " Burst=" << burstTime << " Priority=" << priority
                  << " Start=" << startTime << " Completion=" << completionTime
                  << " Waiting=" << waitingTime << " Turnaround=" << turnaroundTime << "\n";
    }
};

std::vector<Job> loadJobs(const std::string& filename) {
    std::vector<Job> jobs;
    std::ifstream file(filename);
    std::string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        std::stringstream ss(line);
        int id, arrival, burst, priority;
        char comma;
        ss >> id >> comma >> arrival >> comma >> burst >> comma >> priority;
        jobs.emplace_back(id, arrival, burst, priority);
    }
    return jobs;
}

void runFCFS(std::vector<Job> jobs) {
    std::queue<Job> q;
    int time = 0;
    std::vector<Job> finished;
    while (!jobs.empty() || !q.empty()) {
        for (auto it = jobs.begin(); it != jobs.end();) {
            if (it->arrivalTime <= time) {
                q.push(*it);
                it = jobs.erase(it);
            } else ++it;
        }
        if (!q.empty()) {
            Job job = q.front(); q.pop();
            if (job.startTime == -1) job.startTime = time;
            time += job.burstTime;
            job.completionTime = time;
            job.calculateMetrics();
            finished.push_back(job);
        } else {
            time++;
        }
    }
    std::cout << "\nFCFS Results:\n";
    double totalTurn = 0, totalWait = 0;
    for (const auto& job : finished) {
        job.display();
        totalTurn += job.turnaroundTime;
        totalWait += job.waitingTime;
    }
    int n = finished.size();
    std::cout << "Average Turnaround: " << (n ? totalTurn/n : 0) << "\n";
    std::cout << "Average Waiting: " << (n ? totalWait/n : 0) << "\n";
}

int main() {
    std::cout << "Simple CPU Job Scheduler (FCFS only)\n";
    std::cout << "Place jobs.csv in the same folder. Press Enter to run...\n";
    std::cin.get();
    std::vector<Job> jobs = loadJobs("jobs.csv");
    if (jobs.empty()) {
        std::cout << "No jobs loaded. Check jobs.csv format.\n";
        return 1;
    }
    runFCFS(jobs);
    return 0;
}