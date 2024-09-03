#include "../include/HardwareInfo.hpp"
#include <string>

#ifdef _WIN32
    #include <Windows.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
#elif __linux__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/utsname.h>
#endif

namespace HardwareInfo {

#ifdef _WIN32

std::string getHardwareId() {
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD adapterInfoSize = sizeof(adapterInfo);
    
    if (GetAdaptersInfo(adapterInfo, &adapterInfoSize) == ERROR_SUCCESS) {
        char macAddress[18];
        snprintf(macAddress, sizeof(macAddress), 
                 "%02X:%02X:%02X:%02X:%02X:%02X",
                 adapterInfo[0].Address[0], adapterInfo[0].Address[1],
                 adapterInfo[0].Address[2], adapterInfo[0].Address[3],
                 adapterInfo[0].Address[4], adapterInfo[0].Address[5]);
        return std::string(macAddress);
    }
    
    return "Unknown";
}

std::string getHostname() {
    char hostname[256];
    DWORD size = sizeof(hostname);
    
    if (GetComputerNameA(hostname, &size)) {
        return std::string(hostname);
    }
    
    return "Unknown";
}

#elif __linux__

std::string getHardwareId() {
    struct utsname sysInfo;
    
    if (uname(&sysInfo) == 0) {
        return std::string(sysInfo.nodename) + "-" + sysInfo.machine;
    }
    
    return "Unknown";
}

std::string getHostname() {
    char hostname[256];
    
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
    
    return "Unknown";
}

#endif

} // namespace HardwareInfo
