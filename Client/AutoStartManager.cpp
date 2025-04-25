#include "AutoStartManager.h"
#include <shlobj.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

bool AutoStartManager::AddToStartup(const std::string& appName, const std::string& appPath) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    if (RegSetValueExA(hKey, appName.c_str(), 0, REG_SZ, (BYTE*)appPath.c_str(), static_cast<DWORD>(appPath.length() + 1)) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }

    RegCloseKey(hKey);
    return true;
}

bool AutoStartManager::RemoveFromStartup(const std::string& appName) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    if (RegDeleteValueA(hKey, appName.c_str()) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }

    RegCloseKey(hKey);
    return true;
}

bool AutoStartManager::IsInStartup(const std::string& appName) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    DWORD type;
    DWORD size = 0;
    bool exists = (RegQueryValueExA(hKey, appName.c_str(), NULL, &type, NULL, &size) == ERROR_SUCCESS);

    RegCloseKey(hKey);
    return exists;
} 