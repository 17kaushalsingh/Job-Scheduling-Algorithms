# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C++ job scheduling simulator implementing multiple CPU scheduling algorithms (FCFS, SJF, Round Robin, Priority with aging). The project exists in two forms:
1. **Unified single-file** (`JobScheduler.cpp`) - **RECOMMENDED** - Interactive UI with all scheduling algorithms
2. **Modular structure** (`src/` and `include/`) - Production codebase with separated concerns

## Build Commands

### Unified Single-File (Recommended)
```bash
# Compile
g++ -std=c++17 JobScheduler.cpp -o JobScheduler

# Run (interactive UI with all scheduling algorithms)
./JobScheduler
```

### Modular Version (Production)
```bash
# Compile with C++17 standard
g++ -std=c++17 -I include src/*.cpp -o ModularScheduler

# Run the scheduler
./ModularScheduler
```

## Architecture

### Core Design Pattern
The codebase follows a **Strategy Pattern** for scheduling algorithms:
- `Scheduler` (abstract base class) defines the interface
- Concrete schedulers (`FCFSScheduler`, `SJFScheduler`, `RoundRobinScheduler`, `PriorityScheduler`) implement specific algorithms
- `Simulator` orchestrates job execution and metrics collection
- `UIController` handles user interaction and scheduler selection

### Key Components

**Job** (`include/Job.h`, `src/Job.cpp`)
- Represents a single job/process with timing metrics
- Tracks: arrival time, burst time, priority, waiting time, turnaround time, completion time
- Immutable once created; metrics calculated after scheduling

**Scheduler Interface** (`include/Scheduler.h`)
- Pure virtual interface that all algorithms implement
- Key methods: `addJob()`, `getNextJob()`, `hasJobs()`, `schedule()`, `setJobs()`
- Returns visualization data via `getGanttChart()`, `getTimelineLog()`, `getStatistics()`

**Concrete Schedulers** (`include/*Scheduler.h`, `src/*Scheduler.cpp`)
- **FCFS**: First-Come-First-Served using queue
- **SJF**: Shortest Job First with preemption support
- **RoundRobin**: Time-slice based with configurable quantum
- **Priority**: Priority-based with aging to prevent starvation

**Simulator** (`include/Simulator.h`, `src/Simulator.cpp`)
- Executes scheduling algorithm on job set
- Maintains current time and manages job lifecycle
- Generates Gantt charts and performance metrics

**UIController** (`include/UIController.h`, `src/UIController.cpp`)
- Interactive CLI menu system
- Job management (create, edit, delete, import CSV, export CSV)
- Algorithm selection and visualization
- Session persistence and theme customization
- Plugin loading via `SchedulerFactory`

### Plugin System

**SchedulerFactory** (`include/SchedulerFactory.h`)
- Dynamically loads scheduler implementations from shared libraries
- Platform-specific: uses `LoadLibrary`/`GetProcAddress` on Windows, `dlopen`/`dlsym` on Unix
- Plugins must export a `createScheduler()` function returning a `Scheduler*`

## Data Format

Job CSV file (`jobs.csv` in root directory):
```csv
id,arrival,burst,priority
1,0,5,2
2,2,3,1
3,4,1,3
4,6,2,2
```

The CSV file supports two formats:
- `id,arrival,burst,priority` (numeric ID)
- `name,arrival,burst,priority` (string name)

## Code Style
- C++17 standard required
- Header files use `#pragma once`
- Smart pointers (`std::unique_ptr`) for scheduler ownership
- Member variables use camelCase
- Class names use PascalCase

## Platform Considerations
- Cross-platform support for Windows and Unix-like systems
- Platform-specific code isolated in `SchedulerFactory` for DLL/shared library loading
- VSCode configuration uses clang compiler with C++17 standard
