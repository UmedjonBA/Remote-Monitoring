#pragma once
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

class ServerWindow {
private:
    HWND hWnd;
    HWND hListBox;
    HWND hInfoBox;
    HWND hScreenshotBox;
    HWND hGetScreenshotButton;

    SOCKET serverSocket;
    std::vector<SOCKET> clientSockets;
    std::mutex clientsMutex;
    bool isRunning;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void HandleClient(SOCKET clientSocket);
    void UpdateClientList();
    void GetScreenshot(SOCKET clientSocket);

public:
    ServerWindow();
    ~ServerWindow();
    bool Initialize();
    void Run();
}; 