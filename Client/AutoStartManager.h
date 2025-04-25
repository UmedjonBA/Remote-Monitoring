#pragma once
#include <windows.h>
#include <string>

class AutoStartManager {
public:
    static bool AddToStartup(const std::string& appName, const std::string& appPath);
    static bool RemoveFromStartup(const std::string& appName);
    static bool IsInStartup(const std::string& appName);
}; 