#pragma once

#include "Scheduler.h"
#include <string>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

class SchedulerFactory {
public:
    // Loads a scheduler plugin from a DLL/shared object
    static std::unique_ptr<Scheduler> loadPlugin(const std::string& path) {
#ifdef _WIN32
        HMODULE handle = LoadLibraryA(path.c_str());
        if (!handle) return nullptr;
        typedef Scheduler* (*CreateSchedulerFunc)();
        CreateSchedulerFunc createScheduler = (CreateSchedulerFunc)GetProcAddress(handle, "createScheduler");
        if (!createScheduler) {
            FreeLibrary(handle);
            return nullptr;
        }
        return std::unique_ptr<Scheduler>(createScheduler());
#else
        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) return nullptr;
        typedef Scheduler* (*CreateSchedulerFunc)();
        CreateSchedulerFunc createScheduler = (CreateSchedulerFunc)dlsym(handle, "createScheduler");
        if (!createScheduler) {
            dlclose(handle);
            return nullptr;
        }
        return std::unique_ptr<Scheduler>(createScheduler());
#endif
    }
};