#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <fstream>
#include "include/ConnectionManager.hpp"
#include "include/HardwareInfo.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n"
              << "Options:\n"
              << "  -p, --port PORT     Port number (default: 22)\n"
              << "  -h, --host HOST     Hostname\n"
              << "  -d, --device DEVICE Device name\n"
              << "  -t, --type TYPE     Connection type (default: SSH)\n"
              << "  --help              Print this help message\n";
}

std::unordered_map<std::string, std::string> parseArguments(int argc, char* argv[], int& port, std::string& connectionType) {
    std::unordered_map<std::string, std::string> args;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help") {
            printUsage(argv[0]);
            exit(0);
        } else if (arg == "-p" || arg == "--port") {
            if (i + 1 < argc) port = std::stoi(argv[++i]);
        } else if (arg == "-h" || arg == "--host") {
            if (i + 1 < argc) args["host"] = argv[++i];
        } else if (arg == "-d" || arg == "--device") {
            if (i + 1 < argc) args["device"] = argv[++i];
        } else if (arg == "-t" || arg == "--type") {
            if (i + 1 < argc) connectionType = argv[++i];
        }
    }
    return args;
}

void promptForDetails(std::string& hostname, std::string& device, int& port, std::string& connectionType) {
    std::cout << "Enter hostname (default: " << hostname << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty()) hostname = input;

    std::cout << "Enter device name (default: " << device << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) device = input;

    std::cout << "Enter port (default: " << port << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) port = std::stoi(input);

    std::cout << "Enter connection type (default: " << connectionType << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) connectionType = input;
}

bool loadFromJson(const std::string& filename, std::string& hostname, std::string& device, int& port, std::string& connectionType) {
    std::ifstream file(filename);
    if (file.is_open()) {
        json j;
        file >> j;
        hostname = j["hostname"];
        device = j["device"];
        port = j["port"];
        connectionType = j["connectionType"];
        return true;
    }
    return false;
}

void saveToJson(const std::string& filename, const std::string& hostname, const std::string& device, int port, const std::string& connectionType) {
    json j;
    j["hostname"] = hostname;
    j["device"] = device;
    j["port"] = port;
    j["connectionType"] = connectionType;
    std::ofstream file(filename);
    file << j.dump(4);
}

int main(int argc, char* argv[]) {
    int port = 22;
    std::string connectionType = "SSH";
    auto args = parseArguments(argc, argv, port, connectionType);

    std::string hostname = args.count("host") ? args["host"] : HardwareInfo::getHostname();
    std::string device = args.count("device") ? args["device"] : HardwareInfo::getHardwareId();

    if (args.empty()) {
        std::string jsonFile = "connection_details.json";
        if (!loadFromJson(jsonFile, hostname, device, port, connectionType)) {
            std::cout << "No command-line arguments provided and no JSON file found.\n";
            promptForDetails(hostname, device, port, connectionType);
            
            std::cout << "Would you like to save these details for future use? (y/n): ";
            std::string saveChoice;
            std::getline(std::cin, saveChoice);
            if (saveChoice == "y" || saveChoice == "Y") {
                saveToJson(jsonFile, hostname, device, port, connectionType);
                std::cout << "Details saved to " << jsonFile << std::endl;
            }
        } else {
            std::cout << "Loaded details from " << jsonFile << std::endl;
        }
    }

    try {
        if (port < 1 || port > 65535) {
            throw std::out_of_range("Port number out of range");
        }

        auto host = std::make_unique<Host>(hostname, device, port);
        auto client = std::make_unique<Client>(hostname, device, port);
        Connection connection(connectionType, port, std::move(host), std::move(client));

        std::cout << "Connection established.\nConnection details:\n";
        connection.GetConnectionType();
        connection.GetPort();

        std::cout << "\nHost details:\n";
        connection.GetHost();

        std::cout << "\nClient details:\n";
        connection.GetClient();

        // Example of changing connection type
        std::string newConnectionType = "SFTP";
        std::string oldConnectionType = connection.SetConnectionType(newConnectionType);
        std::cout << "\nChanged connection type from " << oldConnectionType << " to ";
        connection.GetConnectionType();

        // Example of changing the host
        auto newHost = std::make_unique<Host>("new-host", "server", 2222);
        auto oldHost = connection.SetHost(std::move(newHost));
        std::cout << "\nChanged host. New host details:\n";
        connection.GetHost();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
