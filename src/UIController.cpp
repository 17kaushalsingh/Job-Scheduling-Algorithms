#include "../include/SchedulerFactory.h"
#include "../include/UIController.h"

void UIController::loadSchedulerPlugin(const std::string& path) {
    pluginPath = path;
    scheduler = SchedulerFactory::loadPlugin(path);
}
// UIController.cpp
#include "../include/UIController.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>

UIController::UIController() : currentAlgorithm(0) {
    switchAlgorithm(0);
}

void UIController::runSession() {
    while (true) {
        clearScreen();
        showMainMenu();
        int choice = getIntInput("Select an option: ", 1, 6);
        handleMainMenuInput(choice);
    }
}

void UIController::showMainMenu() {
    std::cout << "=== Advanced Job Scheduler ===\n";
    std::cout << "1. Manage Jobs\n";
    std::cout << "2. Select Scheduling Algorithm\n";
    std::cout << "3. Visualization\n";
    std::cout << "4. Statistics\n";
    std::cout << "5. Help\n";
    std::cout << "6. Exit\n";
}

void UIController::handleMainMenuInput(int choice) {
    switch (choice) {
        case 1: showJobMenu(); break;
        case 2: showAlgorithmMenu(); break;
        case 3: showVisualizationMenu(); break;
        case 4: showStatisticsMenu(); break;
        case 5: showHelpOverlay(); pause(); break;
        case 6: exit(0);
        default: error("Invalid choice."); pause();
    }
}

void UIController::showJobMenu() {
    clearScreen();
    std::cout << "=== Job Management ===\n";
    std::cout << "1. Create Job\n";
    std::cout << "2. Edit Job\n";
    std::cout << "3. Delete Job\n";
    std::cout << "4. Import Jobs from CSV\n";
    std::cout << "5. Export Jobs to CSV\n";
    std::cout << "6. Back\n";
    int choice = getIntInput("Select an option: ", 1, 6);
    handleJobMenuInput(choice);
}

void UIController::handleJobMenuInput(int choice) {
    switch (choice) {
        case 1: createJob(); break;
        case 2: editJob(); break;
        case 3: deleteJob(); break;
        case 4: importJobsCSV(); break;
        case 5: exportJobsCSV(); break;
        case 6: return;
        default: error("Invalid choice."); pause();
    }
}

void UIController::showAlgorithmMenu() {
    clearScreen();
    std::cout << "=== Algorithm Selection ===\n";
    std::cout << "1. FCFS\n";
    std::cout << "2. SJF\n";
    std::cout << "3. Round Robin\n";
    std::cout << "4. Priority\n";
    std::cout << "5. Back\n";
    int choice = getIntInput("Select an algorithm: ", 1, 5);
    handleAlgorithmMenuInput(choice);
}

void UIController::handleAlgorithmMenuInput(int choice) {
    if (choice >= 1 && choice <= 4) {
        switchAlgorithm(choice - 1);
        updateScheduler();
        std::cout << "Algorithm switched.\n";
        pause();
    }
}

void UIController::showVisualizationMenu() {
    clearScreen();
    std::cout << "=== Visualization ===\n";
    std::cout << "1. Display Gantt Chart\n";
    std::cout << "2. Display Timeline Log\n";
    std::cout << "3. Back\n";
    int choice = getIntInput("Select an option: ", 1, 3);
    handleVisualizationMenuInput(choice);
}

void UIController::handleVisualizationMenuInput(int choice) {
    switch (choice) {
        case 1: displayGanttChart(); pause(); break;
        case 2: displayTimelineLog(); pause(); break;
        case 3: return;
        default: error("Invalid choice."); pause();
    }
}

void UIController::showStatisticsMenu() {
    clearScreen();
    std::cout << "=== Statistics ===\n";
    displayStatistics();
    pause();
}

void UIController::handleStatisticsMenuInput(int) {}

void UIController::showHelpOverlay() {
    std::cout << "Help:\n";
    std::cout << "- Use menus to manage jobs and select algorithms.\n";
    std::cout << "- Visualization shows Gantt chart and logs.\n";
    std::cout << "- Statistics display per-job and aggregate info.\n";
    std::cout << "- You can switch algorithms anytime.\n";
    std::cout << "- Use CSV import/export for batch job management.\n";
    std::cout << "- Robust error handling is provided throughout.\n";
}

void UIController::createJob() {
    std::cout << "Enter Job Name: ";
    std::string name = getStringInput("");
    int arrival = getIntInput("Arrival Time: ", 0, 10000);
    int burst = getIntInput("Burst Time: ", 1, 10000);
    int priority = getIntInput("Priority: ", 0, 100);
    jobs.push_back(Job(name, arrival, burst, priority));
    updateScheduler();
    std::cout << "Job created.\n";
    pause();
}

void UIController::editJob() {
    if (jobs.empty()) { error("No jobs to edit."); pause(); return; }
    std::cout << "Select job to edit:\n";
    for (size_t i = 0; i < jobs.size(); ++i)
        std::cout << i + 1 << ". " << jobs[i].getName() << "\n";
    int idx = getIntInput("Job number: ", 1, jobs.size()) - 1;
    std::cout << "Editing " << jobs[idx].getName() << "\n";
    jobs[idx].setArrivalTime(getIntInput("New Arrival Time: ", 0, 10000));
    jobs[idx].setBurstTime(getIntInput("New Burst Time: ", 1, 10000));
    jobs[idx].setPriority(getIntInput("New Priority: ", 0, 100));
    updateScheduler();
    std::cout << "Job updated.\n";
    pause();
}

void UIController::deleteJob() {
    if (jobs.empty()) { error("No jobs to delete."); pause(); return; }
    std::cout << "Select job to delete:\n";
    for (size_t i = 0; i < jobs.size(); ++i)
        std::cout << i + 1 << ". " << jobs[i].getName() << "\n";
    int idx = getIntInput("Job number: ", 1, jobs.size()) - 1;
    jobs.erase(jobs.begin() + idx);
    updateScheduler();
    std::cout << "Job deleted.\n";
    pause();
}

void UIController::importJobsCSV() {
    std::string filename = getStringInput("CSV filename to import: ");
    std::ifstream file(filename);
    if (!file) { error("File not found."); pause(); return; }
    jobs.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string name;
        int arrival, burst, priority;
        char delim;
        if (ss >> name >> delim >> arrival >> delim >> burst >> delim >> priority)
            jobs.push_back(Job(name, arrival, burst, priority));
    }
    updateScheduler();
    std::cout << "Jobs imported.\n";
    pause();
}

void UIController::exportJobsCSV() {
    std::string filename = getStringInput("CSV filename to export: ");
    std::ofstream file(filename);
    if (!file) { error("Cannot open file."); pause(); return; }
    for (const auto& job : jobs)
        file << job.getName() << "," << job.getArrivalTime() << "," << job.getBurstTime() << "," << job.getPriority() << "\n";
    std::cout << "Jobs exported.\n";
    pause();
}

void UIController::switchAlgorithm(int algo) {
    currentAlgorithm = algo;
    switch (algo) {
        case 0: scheduler = std::make_unique<FCFSScheduler>(); break;
        case 1: scheduler = std::make_unique<SJFScheduler>(); break;
        case 2: scheduler = std::make_unique<RoundRobinScheduler>(); break;
        case 3: scheduler = std::make_unique<PriorityScheduler>(); break;
        default: scheduler = std::make_unique<FCFSScheduler>();
    }
    updateScheduler();
}

void UIController::updateScheduler() {
    if (scheduler) scheduler->setJobs(jobs);
}

void UIController::displayGanttChart() {
    if (!scheduler) { error("No scheduler selected."); return; }
    auto chart = scheduler->getGanttChart();
    std::cout << "Gantt Chart:\n" << chart << "\n";
}

void UIController::displayTimelineLog() {
    if (!scheduler) { error("No scheduler selected."); return; }
    auto log = scheduler->getTimelineLog();
    std::cout << "Timeline Log:\n" << log << "\n";
}

void UIController::displayStatistics() {
    if (!scheduler) { error("No scheduler selected."); return; }
    auto stats = scheduler->getStatistics();
    std::cout << stats << "\n";
}

int UIController::getIntInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            error("Invalid input. Try again.");
        } else break;
    }
    return value;
}

std::string UIController::getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::cin >> value;
    return value;
}

void UIController::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void UIController::pause() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void UIController::error(const std::string& msg) {
    std::cout << "Error: " << msg << "\n";
}
// Session persistence implementation
#include <fstream>
#include <sstream>

void UIController::saveSession(const std::string& filename) {
    std::ofstream out(filename);
    out << "algorithm," << currentAlgorithm << "\n";
    out << "pluginPath," << pluginPath << "\n";
    out << "jobs\n";
    for (const auto& job : jobs) {
        out << job.serialize() << "\n"; // Assumes Job::serialize() returns CSV string
    }
    // Optionally add results/statistics here
}

void UIController::loadSession(const std::string& filename) {
    std::ifstream in(filename);
    std::string line;
    jobs.clear();
    while (std::getline(in, line)) {
        if (line.rfind("algorithm,", 0) == 0) {
            currentAlgorithm = std::stoi(line.substr(10));
        } else if (line.rfind("pluginPath,", 0) == 0) {
            pluginPath = line.substr(11);
        } else if (line == "jobs") {
            while (std::getline(in, line) && !line.empty()) {
                Job job;
                job.deserialize(line); // Assumes Job::deserialize(const std::string&)
                jobs.push_back(job);
            }
        }
    }
    if (!pluginPath.empty()) {
        loadSchedulerPlugin(pluginPath);
    } else {
        switchAlgorithm(currentAlgorithm);
    }
}
// Theme and settings customization
void UIController::setTheme(const std::string& themeName) {
    theme = themeName;
    // Apply theme to UI output (colors, etc.)
}

void UIController::setUserSetting(const std::string& key, const std::string& value) {
    userSettings[key] = value;
    // Apply setting if needed (e.g., default algorithm, time quantum)
}