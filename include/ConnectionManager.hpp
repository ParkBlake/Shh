#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <utility>

class NetworkEntity {

    private:

        std::string Hostname;
        std::string Device;
        int Port;

    public:

        NetworkEntity(std::string Hostname, std::string Device, int Port) 
            : Hostname(std::move(Hostname)), Device(std::move(Device)), Port(Port) {}

        void GetHostname() const;
        std::string SetHostname(std::string& Hostname);

        void GetDevice() const;
        std::string SetDevice(std::string& Device);

        void GetPort() const;
        int SetPort(int Port);
};

class Host {

    private:

        NetworkEntity Entity;

    public:

        Host(std::string Hostname, std::string Device, int Port) 
            : Entity(std::move(Hostname), std::move(Device), Port) {}

        // Delegate methods to NetworkEntity
        void GetHostname() const { Entity.GetHostname(); }
        std::string SetHostname(std::string& Hostname) { return Entity.SetHostname(Hostname); }

        void GetDevice() const { Entity.GetDevice(); }
        std::string SetDevice(std::string& Device) { return Entity.SetDevice(Device); }

        void GetPort() const { Entity.GetPort(); }
        int SetPort(int Port) { return Entity.SetPort(Port); }

};

class Client {

    private:

        NetworkEntity Entity;

    public:

        Client(std::string Hostname, std::string Device, int Port) 
            : Entity(std::move(Hostname), std::move(Device), Port) {}

        void GetHostname() const { Entity.GetHostname(); }
        std::string SetHostname(std::string& Hostname) { return Entity.SetHostname(Hostname); }

        void GetDevice() const { Entity.GetDevice(); }
        std::string SetDevice(std::string& Device) { return Entity.SetDevice(Device); }

        void GetPort() const { Entity.GetPort(); }
        int SetPort(int Port) { return Entity.SetPort(Port); }

};

class Connection {

    private:

        std::string ConnectionType;
        int Port;
        std::unique_ptr<Host> HostPtr;
        std::unique_ptr<Client> ClientPtr;

    public:

        Connection(std::string ConnectionType, int Port, std::unique_ptr<Host> HostPtr, std::unique_ptr<Client> ClientPtr) 
            : ConnectionType(std::move(ConnectionType)), Port(Port), HostPtr(std::move(HostPtr)), ClientPtr(std::move(ClientPtr)) {}

        void GetConnectionType() const {
            std::cout << "Connection Type: " << ConnectionType << std::endl;
        }

        std::string SetConnectionType(std::string& newConnectionType) {
            std::string oldConnectionType = ConnectionType;
            ConnectionType = newConnectionType;
            return oldConnectionType;
        }

        void GetPort() const {
            std::cout << "Port: " << Port << std::endl;
        }

        int SetPort(int newPort) {
            int oldPort = Port;
            Port = newPort;
            return oldPort;
        }

        void GetHost() const {
            if (HostPtr) {
                HostPtr->GetHostname();
                HostPtr->GetDevice();
                HostPtr->GetPort();
            }
        }

        std::unique_ptr<Host> SetHost(std::unique_ptr<Host> newHost) {
            std::unique_ptr<Host> oldHost = std::move(HostPtr);
            HostPtr = std::move(newHost);
            return oldHost;
        }

        void GetClient() const {
            if (ClientPtr) {
                ClientPtr->GetHostname();
                ClientPtr->GetDevice();
                ClientPtr->GetPort();
            }
        }

        std::unique_ptr<Client> SetClient(std::unique_ptr<Client> newClient) {
            std::unique_ptr<Client> oldClient = std::move(ClientPtr);
            ClientPtr = std::move(newClient);
            return oldClient;
        }
};
