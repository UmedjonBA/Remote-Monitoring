#pragma once
#include <string>
#include <windows.h>

class SystemInfo {
public:
    static std::string GetComputerName();
    static std::string GetUserName();
    static std::string GetDomainName();
    static std::string GetIPAddress();
    static std::string GetLastActiveTime();
    static std::string GetAllSystemInfo();
}; 