#include "../include/ConnectionManager.hpp"

#include <iostream>

void NetworkEntity::GetHostname() const {
    std::cout << "Hostname: " << Hostname << std::endl;
}

std::string NetworkEntity::SetHostname(std::string& Hostname) {
    std::string oldHostname = this->Hostname;
    this->Hostname = Hostname;
    return oldHostname;
}

void NetworkEntity::GetDevice() const {
    std::cout << "Device: " << Device << std::endl;
}

std::string NetworkEntity::SetDevice(std::string& Device) {
    std::string oldDevice = this->Device;
    this->Device = Device;
    return oldDevice;
}

void NetworkEntity::GetPort() const {
    std::cout << "Port: " << Port << std::endl;
}

int NetworkEntity::SetPort(int Port) {
    int oldPort = this->Port;
    this->Port = Port;
    return oldPort;
}

