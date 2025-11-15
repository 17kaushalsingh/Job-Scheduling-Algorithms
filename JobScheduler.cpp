// JobScheduler.cpp
// Advanced job scheduler with interactive CLI
// Compile: g++ -std=c++17 JobScheduler.cpp -o JobScheduler
// Run: ./JobScheduler

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <limits>

// ===================== Job Class =====================
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

    Job(int id, int arrival, int burst, int prio = 0)
        : jobId(id), name("Job" + std::to_string(id)), arrivalTime(arrival), burstTime(burst), priority(prio),
          remainingTime(burst), startTime(-1), completionTime(-1), waitingTime(0), turnaroundTime(0) {}

    Job(const std::string& name, int arrival, int burst, int prio = 0)
        : jobId(-1), name(name), arrivalTime(arrival), burstTime(burst), priority(prio),
          remainingTime(burst), startTime(-1), completionTime(-1), waitingTime(0), turnaroundTime(0) {}

    void calculateMetrics() {
        turnaroundTime = completionTime - arrivalTime;
        waitingTime = turnaroundTime - burstTime;
    }

    void display() const {
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
};

// ===================== Scheduler Interface =====================
class Scheduler {
public:
    virtual void addJob(const Job& job) = 0;
    virtual Job getNextJob() = 0;
    virtual bool hasJobs() const = 0;
    virtual void schedule(int currentTime) = 0;
    virtual void setJobs(const std::vector<Job>& jobs) = 0;
    virtual std::string getGanttChart() const = 0;
    virtual std::string getName() const = 0;
    virtual ~Scheduler() {}
protected:
    std::vector<Job> scheduledJobs;
    std::vector<std::string> timelineLog;
};

// ===================== FCFS Scheduler =====================
class FCFSScheduler : public Scheduler {
    std::queue<Job> fcfsQueue;
public:
    FCFSScheduler() {}
    std::string getName() const override { return "First Come First Serve (FCFS)"; }
    void addJob(const Job& job) override { fcfsQueue.push(job); }
    Job getNextJob() override {
        if (!fcfsQueue.empty()) {
            Job job = fcfsQueue.front();
            fcfsQueue.pop();
            scheduledJobs.push_back(job);
            return job;
        }
        return Job(-1, 0, 0, 0);
    }
    bool hasJobs() const override { return !fcfsQueue.empty(); }
    void schedule(int) override {}
    void setJobs(const std::vector<Job>& jobs) override {
        while (!fcfsQueue.empty()) fcfsQueue.pop();
        for (const auto& job : jobs) fcfsQueue.push(job);
        scheduledJobs.clear();
        timelineLog.clear();
    }
    std::string getGanttChart() const override {
        std::ostringstream oss;
        oss << "Gantt Chart:\nTime:   ";
        int time = 0;
        for (const auto& job : scheduledJobs) {
            oss << std::setw(4) << time << " ";
            time += job.burstTime;
        }
        oss << "\nJobs:   ";
        for (const auto& job : scheduledJobs) {
            oss << std::setw(4) << job.name << " ";
        }
        return oss.str();
    }
};

// ===================== SJF Scheduler =====================
class SJFScheduler : public Scheduler {
    std::vector<Job> sjfQueue;
public:
    SJFScheduler() {}
    std::string getName() const override { return "Shortest Job First (SJF)"; }
    void addJob(const Job& job) override { sjfQueue.push_back(job); }
    Job getNextJob() override {
        if (sjfQueue.empty()) return Job(-1, 0, 0, 0);
        auto it = std::min_element(sjfQueue.begin(), sjfQueue.end(),
            [](const Job& a, const Job& b) { return a.remainingTime < b.remainingTime; });
        Job job = *it;
        sjfQueue.erase(it);
        scheduledJobs.push_back(job);
        return job;
    }
    bool hasJobs() const override { return !sjfQueue.empty(); }
    void schedule(int) override {
        std::sort(sjfQueue.begin(), sjfQueue.end(),
            [](const Job& a, const Job& b) { return a.remainingTime < b.remainingTime; });
    }
    void setJobs(const std::vector<Job>& jobs) override {
        sjfQueue = jobs;
        std::sort(sjfQueue.begin(), sjfQueue.end(),
            [](const Job& a, const Job& b) { return a.burstTime < b.burstTime; });
        scheduledJobs.clear();
        timelineLog.clear();
    }
    std::string getGanttChart() const override {
        std::ostringstream oss;
        oss << "Gantt Chart:\nTime:   ";
        int time = 0;
        for (const auto& job : scheduledJobs) {
            oss << std::setw(4) << time << " ";
            time += job.burstTime;
        }
        oss << "\nJobs:   ";
        for (const auto& job : scheduledJobs) {
            oss << std::setw(4) << job.name << " ";
        }
        return oss.str();
    }
};

// ===================== Round Robin Scheduler =====================
class RoundRobinScheduler : public Scheduler {
    std::queue<Job> rrQueue;
    int timeQuantum;
public:
    RoundRobinScheduler(int quantum = 2) : timeQuantum(quantum) {}
    std::string getName() const override { return "Round Robin (RR)"; }
    void addJob(const Job& job) override { rrQueue.push(job); }
    Job getNextJob() override {
        if (rrQueue.empty()) return Job(-1, 0, 0, 0);
        Job job = rrQueue.front();
        rrQueue.pop();
        scheduledJobs.push_back(job);
        return job;
    }
    bool hasJobs() const override { return !rrQueue.empty(); }
    void schedule(int) override {}
    void setJobs(const std::vector<Job>& jobs) override {
        std::queue<Job> empty;
        std::swap(rrQueue, empty);
        for (const auto& job : jobs) rrQueue.push(job);
        scheduledJobs.clear();
        timelineLog.clear();
    }
    std::string getGanttChart() const override {
        std::ostringstream oss;
        oss << "Gantt Chart:\nTime:   ";
        int time = 0;
        for (const auto& job : scheduledJobs) {
            oss << std::setw(4) << time << " ";
            time += job.burstTime;
        }
        oss << "\nJobs:   ";
        for (const auto& job : scheduledJobs) {
            oss << std::setw(4) << job.name << " ";
        }
        return oss.str();
    }
    int getQuantum() const { return timeQuantum; }
};

// ===================== Priority Scheduler =====================
class PriorityScheduler : public Scheduler {
    std::vector<Job> priorityQueue;
    int agingThreshold, agingIncrement;
public:
    PriorityScheduler(int agingThreshold = 5, int agingIncrement = 1)
        : agingThreshold(agingThreshold), agingIncrement(agingIncrement) {}
    std::string getName() const override { return "Priority Scheduling with Aging"; }
    void addJob(const Job& job) override { priorityQueue.push_back(job); }
    Job getNextJob() override {
        if (priorityQueue.empty()) return Job(-1, 0, 0, 0);
        auto it = std::min_element(priorityQueue.begin(), priorityQueue.end(),
            [](const Job& a, const Job& b) { return a.priority < b.priority; });
        Job job = *it;
        priorityQueue.erase(it);
        scheduledJobs.push_back(job);
        return job;
    }
    bool hasJobs() const override { return !priorityQueue.empty(); }
    void schedule(int currentTime) override {
        applyAging(currentTime);
        std::sort(priorityQueue.begin(), priorityQueue.end(),
            [](const Job& a, const Job& b) { return a.priority < b.priority; });
    }
    void setJobs(const std::vector<Job>& jobs) override {
        priorityQueue = jobs;
        std::sort(priorityQueue.begin(), priorityQueue.end(),
            [](const Job& a, const Job& b) { return a.priority < b.priority; });
        scheduledJobs.clear();
        timelineLog.clear();
    }
    void applyAging(int currentTime) {
        for (auto& job : priorityQueue) {
            if (currentTime - job.arrivalTime > agingThreshold) {
                job.priority -= agingIncrement;
                if (job.priority < 0) job.priority = 0;
            }
        }
    }
    std::string getGanttChart() const override {
        std::ostringstream oss;
        oss << "Gantt Chart:\nTime:   ";
        int time = 0;
        for (const auto& job : scheduledJobs) {
            oss << std::setw(4) << time << " ";
            time += job.burstTime;
        }
        oss << "\nJobs:   ";
        for (const auto& job : scheduledJobs) {
            oss << std::setw(4) << job.name << " ";
        }
        return oss.str();
    }
};

// ===================== Simulator =====================
class Simulator {
    int currentTime;
    std::unique_ptr<Scheduler> scheduler;
    std::vector<Job> allJobs;
    std::vector<Job> finishedJobs;
    std::vector<std::pair<int, int>> ganttChart; // (jobId, time)
public:
    Simulator(std::unique_ptr<Scheduler> sched, std::vector<Job> jobs)
        : currentTime(0), scheduler(std::move(sched)), allJobs(std::move(jobs)) {}

    void run() {
        while (!allJobs.empty() || scheduler->hasJobs()) {
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

    void reportMetrics() const {
        double totalTurnaround = 0, totalWaiting = 0;
        for (const auto& job : finishedJobs) {
            job.display();
            totalTurnaround += job.turnaroundTime;
            totalWaiting += job.waitingTime;
        }
        int n = finishedJobs.size();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nAverage Turnaround Time: " << (n ? totalTurnaround / n : 0) << std::endl;
        std::cout << "Average Waiting Time: " << (n ? totalWaiting / n : 0) << std::endl;
    }

    void printGanttChart() const {
        std::cout << "\nGantt Chart:\n";
        for (const auto& entry : ganttChart) {
            std::cout << "JobID: " << entry.first << " at Time: " << entry.second << std::endl;
        }
    }
};

// ===================== Job Loader =====================
std::vector<Job> loadJobsFromCSV(const std::string& filename) {
    std::vector<Job> jobs;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return jobs;
    }

    std::string line;
    std::getline(file, line); // Skip header
    int id = 1;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int arrival, burst, priority;
        char comma;

        // Try to parse as: id,arrival,burst,priority
        int fileId;
        if (ss >> fileId >> comma >> arrival >> comma >> burst >> comma >> priority) {
            jobs.emplace_back(fileId, arrival, burst, priority);
        } else {
            // Try to parse as: name,arrival,burst,priority
            ss.clear();
            ss.str(line);
            std::string name;
            if (std::getline(ss, name, ',') &&
                ss >> arrival >> comma &&
                ss >> burst >> comma &&
                ss >> priority) {
                jobs.emplace_back(name, arrival, burst, priority);
                jobs.back().jobId = id++;
            }
        }
    }
    return jobs;
}

// ===================== UI Controller =====================
class UIController {
    std::vector<Job> jobs;
    int currentAlgorithm;
    int rrQuantum;
    int agingThreshold, agingIncrement;
    std::string jobsFile;
public:
    UIController() : currentAlgorithm(0), rrQuantum(2), agingThreshold(5), agingIncrement(1), jobsFile("jobs.csv") {}

    void runSession() {
        loadJobs();
        while (true) {
            clearScreen();
            showMainMenu();
            int choice = getIntInput("Select an option: ", 1, 7);
            handleMainMenuInput(choice);
        }
    }

    void showMainMenu() {
        std::cout << "=== Advanced Job Scheduler ===\n";
        std::cout << "1. Manage Jobs\n";
        std::cout << "2. Select Scheduling Algorithm\n";
        std::cout << "3. Run Scheduler & View Visualization\n";
        std::cout << "4. Run Scheduler & View Statistics\n";
        std::cout << "5. Session Persistence\n";
        std::cout << "6. Customization\n";
        std::cout << "7. Exit\n";
    }

    void handleMainMenuInput(int choice) {
        switch (choice) {
            case 1: showJobMenu(); break;
            case 2: showAlgorithmMenu(); break;
            case 3: showVisualizationMenu(); break;
            case 4: showStatisticsMenu(); break;
            case 5: showPersistenceMenu(); break;
            case 6: showCustomizationMenu(); break;
            case 7: exit(0);
            default: error("Invalid choice."); pause();
        }
    }

    void showJobMenu() {
        clearScreen();
        std::cout << "=== Manage Jobs ===\n";
        std::cout << "1. Add Job\n";
        std::cout << "2. Remove Job\n";
        std::cout << "3. List Jobs\n";
        std::cout << "4. Back\n";
        int choice = getIntInput("Select an option: ", 1, 4);
        switch (choice) {
            case 1: addJob(); break;
            case 2: removeJob(); break;
            case 3: listJobs(); pause(); break;
            case 4: return;
        }
    }

    void addJob() {
        std::string name;
        int arrival, burst, prio;
        std::cout << "Enter job name: ";
        std::cin >> name;
        arrival = getIntInput("Enter arrival time: ", 0, 10000);
        burst = getIntInput("Enter burst time: ", 1, 10000);
        prio = getIntInput("Enter priority (lower is higher): ", 0, 100);
        int nextId = jobs.empty() ? 1 : jobs.back().jobId + 1;
        jobs.emplace_back(name, arrival, burst, prio);
        jobs.back().jobId = nextId;
        std::cout << "Job added.\n";
        pause();
    }

    void removeJob() {
        if (jobs.empty()) {
            std::cout << "No jobs to remove.\n";
            pause();
            return;
        }
        listJobs();
        int idx = getIntInput("Enter job index to remove: ", 1, jobs.size());
        jobs.erase(jobs.begin() + (idx - 1));
        std::cout << "Job removed.\n";
        pause();
    }

    void listJobs() {
        std::cout << "Current Jobs:\n";
        if (jobs.empty()) {
            std::cout << "No jobs loaded.\n";
            return;
        }
        for (size_t i = 0; i < jobs.size(); ++i) {
            std::cout << i + 1 << ". ";
            jobs[i].display();
        }
    }

    void showAlgorithmMenu() {
        clearScreen();
        std::cout << "=== Select Scheduling Algorithm ===\n";
        std::cout << "Current: ";
        switch (currentAlgorithm) {
            case 0: std::cout << "FCFS\n"; break;
            case 1: std::cout << "SJF\n"; break;
            case 2: std::cout << "Round Robin\n"; break;
            case 3: std::cout << "Priority\n"; break;
        }
        std::cout << "\n1. FCFS (First Come First Serve)\n";
        std::cout << "2. SJF (Shortest Job First)\n";
        std::cout << "3. Round Robin\n";
        std::cout << "4. Priority (with aging)\n";
        std::cout << "5. Back\n";
        int choice = getIntInput("Select an option: ", 1, 5);
        if (choice >= 1 && choice <= 4) {
            currentAlgorithm = choice - 1;
            std::cout << "Algorithm changed.\n";
            pause();
        }
    }

    void showVisualizationMenu() {
        if (jobs.empty()) {
            clearScreen();
            std::cout << "No jobs loaded. Please add jobs or load from file.\n";
            pause();
            return;
        }
        clearScreen();
        std::cout << "=== Visualization ===\n";
        auto sim = createSimulator();
        std::cout << "Running " << getCurrentSchedulerName() << "...\n\n";
        sim->run();
        sim->printGanttChart();
        pause();
    }

    void showStatisticsMenu() {
        if (jobs.empty()) {
            clearScreen();
            std::cout << "No jobs loaded. Please add jobs or load from file.\n";
            pause();
            return;
        }
        clearScreen();
        std::cout << "=== Statistics ===\n";
        auto sim = createSimulator();
        std::cout << "Running " << getCurrentSchedulerName() << "...\n\n";
        sim->run();
        sim->reportMetrics();
        pause();
    }

    void showPersistenceMenu() {
        clearScreen();
        std::cout << "=== Session Persistence ===\n";
        std::cout << "1. Save Jobs to CSV\n";
        std::cout << "2. Load Jobs from CSV\n";
        std::cout << "3. Back\n";
        int choice = getIntInput("Select an option: ", 1, 3);
        switch (choice) {
            case 1: saveJobs(); break;
            case 2: loadJobs(); break;
            case 3: return;
        }
        pause();
    }

    void showCustomizationMenu() {
        clearScreen();
        std::cout << "=== Customization ===\n";
        std::cout << "1. Set Round Robin Quantum (Current: " << rrQuantum << ")\n";
        std::cout << "2. Set Priority Aging Threshold (Current: " << agingThreshold << ")\n";
        std::cout << "3. Set Priority Aging Increment (Current: " << agingIncrement << ")\n";
        std::cout << "4. Back\n";
        int choice = getIntInput("Select an option: ", 1, 4);
        switch (choice) {
            case 1: rrQuantum = getIntInput("Enter new quantum: ", 1, 100);
                    std::cout << "Quantum updated.\n"; pause(); break;
            case 2: agingThreshold = getIntInput("Enter new aging threshold: ", 1, 100);
                    std::cout << "Aging threshold updated.\n"; pause(); break;
            case 3: agingIncrement = getIntInput("Enter new aging increment: ", 1, 100);
                    std::cout << "Aging increment updated.\n"; pause(); break;
            case 4: return;
        }
    }

    std::string getCurrentSchedulerName() {
        switch (currentAlgorithm) {
            case 0: return "FCFS (First Come First Serve)";
            case 1: return "SJF (Shortest Job First)";
            case 2: return "Round Robin";
            case 3: return "Priority Scheduling with Aging";
            default: return "Unknown";
        }
    }

    std::unique_ptr<Simulator> createSimulator() {
        std::unique_ptr<Scheduler> sched;
        switch (currentAlgorithm) {
            case 0: sched = std::make_unique<FCFSScheduler>(); break;
            case 1: sched = std::make_unique<SJFScheduler>(); break;
            case 2: sched = std::make_unique<RoundRobinScheduler>(rrQuantum); break;
            case 3: sched = std::make_unique<PriorityScheduler>(agingThreshold, agingIncrement); break;
            default: sched = std::make_unique<FCFSScheduler>();
        }
        sched->setJobs(jobs);
        return std::make_unique<Simulator>(std::move(sched), jobs);
    }

    void saveJobs() {
        std::string filename;
        std::cout << "Enter filename (default: jobs.csv): ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, filename);
        if (filename.empty()) filename = "jobs.csv";

        std::ofstream ofs(filename);
        ofs << "id,arrival,burst,priority\n";
        for (const auto& job : jobs) {
            ofs << job.jobId << "," << job.arrivalTime << "," << job.burstTime << "," << job.priority << "\n";
        }
        std::cout << "Jobs saved to " << filename << ".\n";
    }

    void loadJobs() {
        std::string filename;
        std::cout << "Enter filename (default: jobs.csv): ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, filename);
        if (filename.empty()) filename = "jobs.csv";

        std::vector<Job> loadedJobs = loadJobsFromCSV(filename);
        if (loadedJobs.empty()) {
            std::cout << "No jobs loaded from " << filename << ".\n";
        } else {
            jobs = loadedJobs;
            std::cout << "Loaded " << jobs.size() << " jobs from " << filename << ".\n";
        }
    }

    // Utility functions
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }

    void pause() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    int getIntInput(const std::string& prompt, int min, int max) {
        int value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;
            if (std::cin.fail() || value < min || value > max) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
            } else {
                return value;
            }
        }
    }

    void error(const std::string& msg) {
        std::cout << "Error: " << msg << std::endl;
    }
};

// ===================== Main Entry =====================
int main() {
    UIController ui;
    ui.runSession();
    return 0;
}
