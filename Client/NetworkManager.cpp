#include "NetworkManager.h"
#include <iostream>

NetworkManager::NetworkManager() : clientSocket(INVALID_SOCKET), isConnected(false) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
}

NetworkManager::~NetworkManager() {
    Disconnect();
    WSACleanup();
}

bool NetworkManager::Connect(const std::string& serverIP, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        return false;
    }

    isConnected = true;
    return true;
}

bool NetworkManager::SendData(const std::string& data) {
    return SendData(data.c_str(), static_cast<int>(data.length()));
}

bool NetworkManager::SendData(const char* data, int size) {
    if (!isConnected) return false;

    int totalSent = 0;
    while (totalSent < size) {
        int sent = send(clientSocket, data + totalSent, size - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            isConnected = false;
            return false;
        }
        totalSent += sent;
    }
    return true;
}

bool NetworkManager::ReceiveData(char* buffer, int bufferSize, int& bytesReceived) {
    if (!isConnected) return false;

    bytesReceived = recv(clientSocket, buffer, bufferSize, 0);
    if (bytesReceived <= 0) {
        isConnected = false;
        return false;
    }
    return true;
}

void NetworkManager::Disconnect() {
    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
    isConnected = false;
}

bool NetworkManager::IsConnected() const {
    return isConnected;
} 