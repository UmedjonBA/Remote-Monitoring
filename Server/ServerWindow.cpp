#include "ServerWindow.h"
#include <sstream>
#include <iomanip>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

ServerWindow::ServerWindow() : hWnd(NULL), serverSocket(INVALID_SOCKET), isRunning(false) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

ServerWindow::~ServerWindow() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    for (SOCKET client : clientSockets) {
        closesocket(client);
    }
    WSACleanup();
}

bool ServerWindow::Initialize() {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"RemoteMonitoringServer";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(
        0,
        L"RemoteMonitoringServer",
        L"Remote Monitoring Server",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, GetModuleHandle(NULL), this
    );

    if (!hWnd) return false;

    hListBox = CreateWindow(
        L"LISTBOX",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 10, 200, 500,
        hWnd,
        (HMENU)1,
        GetModuleHandle(NULL),
        NULL
    );

    hInfoBox = CreateWindow(
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
        220, 10, 300, 200,
        hWnd,
        (HMENU)2,
        GetModuleHandle(NULL),
        NULL
    );

    hScreenshotBox = CreateWindow(
        L"STATIC",
        NULL,
        WS_CHILD | WS_VISIBLE | SS_BITMAP,
        220, 220, 300, 300,
        hWnd,
        (HMENU)3,
        GetModuleHandle(NULL),
        NULL
    );

    hGetScreenshotButton = CreateWindow(
        L"BUTTON",
        L"Get Screenshot",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        220, 530, 300, 30,
        hWnd,
        (HMENU)4,
        GetModuleHandle(NULL),
        NULL
    );

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) return false;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(serverSocket);
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(serverSocket);
        return false;
    }

    return true;
}

void ServerWindow::Run() {
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    isRunning = true;
    std::thread acceptThread([this]() {
        while (isRunning) {
            SOCKET clientSocket = accept(serverSocket, NULL, NULL);
            if (clientSocket != INVALID_SOCKET) {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clientSockets.push_back(clientSocket);
                std::thread(&ServerWindow::HandleClient, this, clientSocket).detach();
                UpdateClientList();
            }
        }
    });

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    isRunning = false;
    acceptThread.join();
}

void ServerWindow::HandleClient(SOCKET clientSocket) {
    char buffer[4096];
    int bytesReceived;

    while (isRunning) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;

        std::string data(buffer, bytesReceived);
        if (data.find("SYSTEM_INFO") == 0) {
            std::lock_guard<std::mutex> lock(clientsMutex);
            SetWindowTextA(hInfoBox, data.c_str());
        }
    }

    std::lock_guard<std::mutex> lock(clientsMutex);
    auto it = std::find(clientSockets.begin(), clientSockets.end(), clientSocket);
    if (it != clientSockets.end()) {
        clientSockets.erase(it);
        UpdateClientList();
    }
    closesocket(clientSocket);
}

void ServerWindow::UpdateClientList() {
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
    for (size_t i = 0; i < clientSockets.size(); ++i) {
        std::wstring item = L"Client " + std::to_wstring(i + 1);
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)item.c_str());
    }
}

void ServerWindow::GetScreenshot(SOCKET clientSocket) {
    std::string command = "SCREENSHOT";
    send(clientSocket, command.c_str(), command.length(), 0);

    int size;
    recv(clientSocket, (char*)&size, sizeof(size), 0);

    std::vector<char> buffer(size);
    int totalReceived = 0;
    while (totalReceived < size) {
        int received = recv(clientSocket, buffer.data() + totalReceived, size - totalReceived, 0);
        if (received <= 0) break;
        totalReceived += received;
    }

    if (totalReceived == size) {
        IStream* stream = SHCreateMemStream((BYTE*)buffer.data(), size);
        Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromStream(stream);
        HBITMAP hBitmap;
        bitmap->GetHBITMAP(Gdiplus::Color::White, &hBitmap);
        SendMessage(hScreenshotBox, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        delete bitmap;
        stream->Release();
    }
}

LRESULT CALLBACK ServerWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    ServerWindow* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (ServerWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    } else {
        pThis = (ServerWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (pThis) {
        switch (uMsg) {
            case WM_COMMAND:
                if (LOWORD(wParam) == 4) { // Get Screenshot button
                    int selected = SendMessage(pThis->hListBox, LB_GETCURSEL, 0, 0);
                    if (selected != LB_ERR) {
                        std::lock_guard<std::mutex> lock(pThis->clientsMutex);
                        if (selected < pThis->clientSockets.size()) {
                            pThis->GetScreenshot(pThis->clientSockets[selected]);
                        }
                    }
                }
                break;

            case WM_DESTROY:
                PostQuitMessage(0);
                break;

            default:
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
} 