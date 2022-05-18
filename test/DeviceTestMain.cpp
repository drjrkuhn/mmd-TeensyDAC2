#define NOMINMAX

#include "DeviceBase.h"
#include <MMCore.h>
#include <MMDevice.h>
#include <rdlmm/DevicePropHelpers.h>
#include <string>
#include <vector>

const std::string g_moduleName{"TeensyDAC2"};
const std::string g_hubName{"TeensyDAC2-Hub"};
const std::string g_port{"COM8"}; // we know the serial port
//const std::string g_port; // search for the serial port

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
    //core.enableStderrLog(true);
    //core.enableDebugLog(true);
    try {
        core.loadDevice(g_hubName.c_str(), g_moduleName.c_str(), g_hubName.c_str());

        string hubPort;
        if (g_port.empty()) {
            // search for the proper serial port

            // detectDevice takes non-const char * so create a copy
            vector<char> hubstr(g_hubName.begin(), g_hubName.end());
            hubstr.push_back('\0');

            vector<string> ports = core.getAvailableDevices("SerialManager");
            for (string testPort : ports) {
                cout << "TESTING PORT " << testPort << endl;
                core.loadDevice(testPort.c_str(), "SerialManager", testPort.c_str());
                core.setProperty(g_hubName.c_str(), "Port", testPort.c_str());

                MM::DeviceDetectionStatus status = core.detectDevice(&hubstr[0]);
                if (status == MM::DeviceDetectionStatus::CanCommunicate) {
                    cout << "FOUND ON PORT " << testPort << endl;
                    hubPort = testPort;
                    break;
                }
                core.unloadDevice(testPort.c_str());
            }
        } else {
            // Global port name already specified
            core.loadDevice(g_port.c_str(), "SerialManager", g_port.c_str());
            hubPort = g_port;
        }

        if (hubPort.empty()) {
            cerr << "Could not find " << g_hubName << endl;
            return -1;
        }

        cout << "==== Loaded Devices ====" << endl;
        for (auto dev : core.getLoadedDevices()) {
            cout << '\t' << dev << endl;
            
        }

        // setup the serial port from the serial manager
        core.setProperty(hubPort.c_str(), "Fast USB to Serial", "Enable");
        core.setProperty(hubPort.c_str(), "Verbose", "0");
        core.initializeDevice(hubPort.c_str());

        // Initialize the device and set the serial port
        core.setProperty(g_hubName.c_str(), "Port", hubPort.c_str());
        core.initializeDevice(g_hubName.c_str());

        cout << "==== " << g_hubName << " Properties ====" << endl;
        for (auto propName : core.getDevicePropertyNames(g_hubName.c_str())) {
            cout << rdlmm::ToString(core.getPropertyType(g_hubName.c_str(), propName.c_str())) << " " << propName;
            cout << " = " << core.getProperty(g_hubName.c_str(), propName.c_str()) << endl;
        }

        // Run unit tests
        //core.setProperty(hubLabel.c_str(), "Test", "Run");
        //cout << "Results: " << core.getProperty(hubLabel.c_str(), "Test") << endl
        //     << endl;

        core.unloadAllDevices();
    } catch (CMMError& err) {
        cout << err.getMsg();
        return 1;
    }
    return 0;
}
