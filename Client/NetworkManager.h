#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

class NetworkManager {
private:
    SOCKET clientSocket;
    bool isConnected;

public:
    NetworkManager();
    ~NetworkManager();

    bool Connect(const std::string& serverIP, int port);
    bool SendData(const std::string& data);
    bool SendData(const char* data, int size);
    bool ReceiveData(char* buffer, int bufferSize, int& bytesReceived);
    void Disconnect();
    bool IsConnected() const;
}; 