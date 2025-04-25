#include "SystemInfo.h"
#include <iphlpapi.h>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "iphlpapi.lib")

std::string SystemInfo::GetComputerName() {
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    GetComputerNameA(computerName, &size);
    return std::string(computerName);
}

std::string SystemInfo::GetUserName() {
    char username[UNLEN + 1];
    DWORD size = sizeof(username);
    GetUserNameA(username, &size);
    return std::string(username);
}

std::string SystemInfo::GetDomainName() {
    char domainName[DNLEN + 1];
    DWORD size = sizeof(domainName);
    GetComputerNameExA(ComputerNameDnsDomain, domainName, &size);
    return std::string(domainName);
}

std::string SystemInfo::GetIPAddress() {
    PIP_ADAPTER_INFO adapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    ULONG bufferLength = sizeof(IP_ADAPTER_INFO);
    
    if (GetAdaptersInfo(adapterInfo, &bufferLength) == ERROR_BUFFER_OVERFLOW) {
        free(adapterInfo);
        adapterInfo = (IP_ADAPTER_INFO*)malloc(bufferLength);
    }

    std::string ipAddress;
    if (GetAdaptersInfo(adapterInfo, &bufferLength) == NO_ERROR) {
        PIP_ADAPTER_INFO adapter = adapterInfo;
        while (adapter) {
            if (adapter->Type == MIB_IF_TYPE_ETHERNET) {
                ipAddress = adapter->IpAddressList.IpAddress.String;
                break;
            }
            adapter = adapter->Next;
        }
    }

    free(adapterInfo);
    return ipAddress;
}

std::string SystemInfo::GetLastActiveTime() {
    LASTINPUTINFO lastInputInfo;
    lastInputInfo.cbSize = sizeof(LASTINPUTINFO);
    GetLastInputInfo(&lastInputInfo);

    DWORD currentTick = GetTickCount();
    DWORD idleTime = (currentTick - lastInputInfo.dwTime) / 1000; // Convert to seconds

    std::stringstream ss;
    ss << idleTime << " seconds";
    return ss.str();
}

std::string SystemInfo::GetAllSystemInfo() {
    std::stringstream ss;
    ss << "Computer: " << GetComputerName() << "\n"
       << "User: " << GetUserName() << "\n"
       << "Domain: " << GetDomainName() << "\n"
       << "IP: " << GetIPAddress() << "\n"
       << "Last Active: " << GetLastActiveTime() << "\n";

    return ss.str();
} 