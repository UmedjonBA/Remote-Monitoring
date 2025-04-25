#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>

class ScreenshotManager {
private:
    ULONG_PTR gdiplusToken;

public:
    ScreenshotManager();
    ~ScreenshotManager();

    bool CaptureScreen(std::vector<BYTE>& imageData);
    bool SaveScreenshot(const std::string& filename);
    bool SendScreenshot(NetworkManager& network);
}; 