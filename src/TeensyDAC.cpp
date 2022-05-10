///////////////////////////////////////////////////////////////////////////////
// FILE:          Arduino.cpp
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   Arduino adapter.  Needs accompanying firmware
// COPYRIGHT:     University of California, San Francisco, 2008
// LICENSE:       LGPL
//
// AUTHOR:        Nico Stuurman, nico@cmp.ucsf.edu 11/09/2008
//                automatic device detection by Karl Hoover
//
//

#include "TeensyDAC.h"
#include "ModuleInterface.h"
#include <rdl/sys_timing.h>

const char* g_DeviceNameArduinoHub     = "Arduino-Hub";
const char* g_DeviceNameArduinoSwitch  = "Arduino-Switch";
const char* g_DeviceNameArduinoShutter = "Arduino-Shutter";
const char* g_DeviceNameArduinoDA1     = "Arduino-DAC1";
const char* g_DeviceNameArduinoDA2     = "Arduino-DAC2";
const char* g_DeviceNameArduinoInput   = "Arduino-Input";

// Global info about the state of the Arduino.  This should be folded into a class
const int g_Min_MMVersion         = 1;
//const int g_Max_MMVersion         = 2;
//const char* g_versionProp         = "Version";
//const char* g_normalLogicString   = "Normal";
//const char* g_invertedLogicString = "Inverted";
//
//const char* g_On  = "On";
//const char* g_Off = "Off";

// static lock
//MMThreadLock CTeensyHub::lock_;

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData() {
    RegisterDevice(g_deviceNameHub, MM::HubDevice, g_deviceDescHub);
    for (int i = 0; i < MAX_NUM_GALVOS; i++) {
        std::string name = g_deviceNameGalvo;
        std::string desc = g_deviceDescGalvo;
        name += char('A' + i);
        desc += " ";
        desc += char('A' + i);
        RegisterDevice(name.c_str(), MM::StageDevice, desc.c_str());
    }
}

MODULE_API MM::Device* CreateDevice(const char* deviceName) {
    if (deviceName == 0) {
        return nullptr;
    }
    if (strcmp(deviceName, g_deviceNameHub) == 0) {
        return new CTeensyHub;
    }

    #if 0
    for (int i = 0; i < MAX_NUM_GALVOS; i++) {
        std::string name = g_deviceNameGalvo;
        name += char('A' + i);
        if (strcmp(deviceName, name.c_str()) == 0) {
            return new TeensyDACGalvo(i);
        }
    }
    #endif
    return nullptr;
}

MODULE_API void DeleteDevice(MM::Device* pDevice) {
    delete pDevice;
}

///////////////////////////////////////////////////////////////////////////////
// CArduinoHUb implementation
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
CTeensyHub::CTeensyHub()
    : initialized_(false), portAvailable_(false), serial_(this), client_(serial_, serial_) {
    InitializeDefaultErrorMessages();
    rdlmm::InitCommonErrors(this, "Teensy", g_Min_MMVersion);
    serial_.setTimeout(5000);

    logger_ = LoggerT(this, true);
    client_.logger(&logger_);
    port_.create(this, g_infoPort);
}

int CTeensyHub::GetControllerVersion(int& version) {
    version = 0;
    try {
        int tempversion  = 0;
        ASSERT_OK((client_.call_get<int, std::string>("?fver", tempversion, g_FirmwareName)));
        version = tempversion;
        return DEVICE_OK;
    } catch (rdlmm::DeviceResultException dre) {
        LogMessage(dre.format(this));
    }
    return rdlmm::ERR_FIRMWARE_NOT_FOUND;
}

MM::DeviceDetectionStatus CTeensyHub::DetectDevice(void) {
    if (initialized_)
        return MM::CanCommunicate;

    return rdlmm::DetectRemote(this, port(), 9600, g_FirmwareName, g_Min_MMVersion);

    //// all conditions must be satisfied...
    //MM::DeviceDetectionStatus result = MM::Misconfigured;
    //char answerTO[MM::MaxStrLength];

    //try {
    //    std::string portLowerCase = port_;
    //    for (std::string::iterator its = portLowerCase.begin(); its != portLowerCase.end(); ++its) {
    //        *its = (char)tolower(*its);
    //    }
    //    if (0 < portLowerCase.length() && 0 != portLowerCase.compare("undefined") && 0 != portLowerCase.compare("unknown")) {
    //        result = MM::CanNotCommunicate;
    //        // record the default answer time out
    //        GetCoreCallback()->GetDeviceProperty(port_.c_str(), "AnswerTimeout", answerTO);

    //        // device specific default communication parameters
    //        // for Arduino Duemilanova
    //        GetCoreCallback()->SetDeviceProperty(port_.c_str(), MM::g_Keyword_Handshaking, g_Off);
    //        GetCoreCallback()->SetDeviceProperty(port_.c_str(), MM::g_Keyword_BaudRate, "57600");
    //        GetCoreCallback()->SetDeviceProperty(port_.c_str(), MM::g_Keyword_StopBits, "1");
    //        // Arduino timed out in GetControllerVersion even if AnswerTimeout  = 300 ms
    //        GetCoreCallback()->SetDeviceProperty(port_.c_str(), "AnswerTimeout", "500.0");
    //        GetCoreCallback()->SetDeviceProperty(port_.c_str(), "DelayBetweenCharsMs", "0");
    //        MM::Device* pS = GetCoreCallback()->GetDevice(this, port_.c_str());
    //        pS->Initialize();
    //        // The first second or so after opening the serial port, the Arduino is waiting for firmwareupgrades.  Simply sleep 2 seconds.
    //        CDeviceUtils::SleepMs(2000);
    //        MMThreadGuard myLock(lock_);
    //        PurgeComPort(port_.c_str());
    //        int v   = 0;
    //        int ret = GetControllerVersion(v);
    //        // later, Initialize will explicitly check the version #
    //        if (DEVICE_OK != ret) {
    //            LogMessageCode(ret, true);
    //        } else {
    //            // to succeed must reach here....
    //            result = MM::CanCommunicate;
    //        }
    //        pS->Shutdown();
    //        // always restore the AnswerTimeout to the default
    //        GetCoreCallback()->SetDeviceProperty(port_.c_str(), "AnswerTimeout", answerTO);
    //    }
    //} catch (...) {
    //    LogMessage("Exception in DetectDevice!", false);
    //}
    //return result;
}

int CTeensyHub::Initialize() {
    try {
        // The first second or so after opening the serial port, the Arduino is waiting for firmwareupgrades.  Simply sleep 1 second.
        sys::delay(1000);
        PurgeComPort(port().c_str());
        ASSERT_OK(name_.create(this, g_infoName));
        ASSERT_OK(version_.create(this, g_infoVersion));
        int tempversion;
        ASSERT_OK(GetControllerVersion(tempversion));
        if (tempversion < g_Min_MMVersion)
            THROW_DEVICE_ERROR(rdlmm::ERR_VERSION_MISMATCH);
        version_.SetProperty(tempversion);

        ASSERT_OK(UpdateStatus());
        initialized_ = true;
        return DEVICE_OK;

    } catch (rdlmm::DeviceResultException dre) {
        LogMessage(dre.format(this));
        return dre.error;
    }
}

int CTeensyHub::DetectInstalledDevices() {
    //if (MM::CanCommunicate == DetectDevice()) {
    //    std::vector<std::string> peripherals;
    //    peripherals.clear();
    //    peripherals.push_back(g_DeviceNameArduinoSwitch);
    //    peripherals.push_back(g_DeviceNameArduinoShutter);
    //    peripherals.push_back(g_DeviceNameArduinoInput);
    //    peripherals.push_back(g_DeviceNameArduinoDA1);
    //    peripherals.push_back(g_DeviceNameArduinoDA2);
    //    for (size_t i = 0; i < peripherals.size(); i++) {
    //        MM::Device* pDev = ::CreateDevice(peripherals[i].c_str());
    //        if (pDev) {
    //            AddInstalledDevice(pDev);
    //        }
    //    }
    //}

    return DEVICE_OK;
}

int CTeensyHub::Shutdown() {
    initialized_ = false;
    return DEVICE_OK;
}

