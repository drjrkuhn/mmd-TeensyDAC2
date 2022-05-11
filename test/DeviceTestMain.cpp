#define NOMINMAX

#include "DeviceBase.h"
#include <MMCore.h>
#include <MMDevice.h>
#include <rdlmm/DevicePropHelpers.h>
#include <string>
#include <vector>

const std::string g_moduleName{"TeensyDAC2"};
const std::string g_deviceName{"TeensyDAC2-Hub"};
const std::string g_hubLabel{"Hub"};
const std::string g_portLabel{"HubSerial"};
const std::string g_port{"COM8"};

inline std::string getPropertyTypeVerbose(MM::PropertyType t) {
    switch (t) {
        case (MM::Float):
            return std::string("Float");
        case (MM::Integer):
            return std::string("Integer");
        case (MM::String):
            return std::string("String");
    }

    // we don't know this property so we'll just use the id
    std::ostringstream os;
    os << "Property_type_" << t;
    return os.str();
}

int main() {
    using namespace std;

    CMMCore core;
    core.enableStderrLog(true);
    core.enableDebugLog(true);
    try {
        // setup the serial port from the serial manager
        core.loadDevice(g_portLabel.c_str(), "SerialManager", g_port.c_str());
        core.setProperty(g_portLabel.c_str(), "Fast USB to Serial", "Enable");
        core.setProperty(g_portLabel.c_str(), "Verbose", "0");
        core.initializeDevice(g_portLabel.c_str());
        // Initialize the device and set the serial port
        core.loadDevice(g_hubLabel.c_str(), g_moduleName.c_str(), g_deviceName.c_str());
        core.setProperty(g_hubLabel.c_str(), "Port", g_portLabel.c_str());
        core.initializeDevice(g_hubLabel.c_str());

        cout << "==== " << g_hubLabel << " Properties ====" << endl;
        for (auto propName : core.getDevicePropertyNames(g_hubLabel.c_str())) {
            cout << rdlmm::ToString(core.getPropertyType(g_hubLabel.c_str(), propName.c_str())) << " " << propName;
            cout << " = " << core.getProperty(g_hubLabel.c_str(), propName.c_str()) << endl;
        }

        // Run unit tests
        //core.setProperty(hubLabel.c_str(), "Test", "Run");
        //cout << "Results: " << core.getProperty(hubLabel.c_str(), "Test") << endl
        //     << endl;

        // unload the device
        // -----------------
        core.unloadAllDevices();
    } catch (CMMError& err) {
        cout << err.getMsg();
        return 1;
    }
    return 0;
}
