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
#include <iostream>

// Global info about the state of the Arduino.  This should be folded into a class
const int g_Min_MMVersion         = 1;

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData() {
    std::cout << "###### InitializeModuleData()" << std::endl;
    RegisterDevice(g_deviceNameHub, MM::HubDevice, g_deviceDescHub);
#if 0
    for (int i = 0; i < MAX_NUM_GALVOS; i++) {
        std::string name = g_deviceNameGalvo;
        std::string desc = g_deviceDescGalvo;
        name += char('A' + i);
        desc += " ";
        desc += char('A' + i);
        RegisterDevice(name.c_str(), MM::StageDevice, desc.c_str());
    }
#endif
}

MODULE_API MM::Device* CreateDevice(const char* deviceName) {
    std::cout << "###### CreateDevice " << deviceName << std::endl;
    if (deviceName == 0) { 
        return nullptr;
    }
    if (strcmp(deviceName, g_deviceNameHub) == 0) {
        return new TeensyHub;
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
    std::cout << "###### DeleteDevice()" << std::endl;
    delete pDevice;
}

///////////////////////////////////////////////////////////////////////////////
// CArduinoHUb implementation
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
TeensyHub::TeensyHub()
    : initialized_(false), serial_(this), client_(serial_, serial_) {
    //std::cout << "###### TeensyHub() constructor" << std::endl;
    InitializeDefaultErrorMessages();
    rdlmm::InitCommonErrors(this, "Teensy", g_Min_MMVersion);
    serial_.setTimeout(5000);

    logger_ = LoggerT(this, true);
    client_.logger(&logger_);
    port_.create(this, g_infoPort);
}

int TeensyHub::GetControllerVersion(int& version) {
    std::cout << "###### GetControllerVersion()" << std::endl;
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

MM::DeviceDetectionStatus TeensyHub::DetectDevice(void) {
    std::cout << "###### DetectDevice()" << std::endl;
    if (initialized_)
        return MM::CanCommunicate;

    return rdlmm::DetectRemote(this, port(), 9600, g_FirmwareName, g_Min_MMVersion);
}

int TeensyHub::Initialize() {
    std::cout << "###### Initialize()" << std::endl;
    try {
        // The first second or so after opening the serial port, the Arduino is waiting for firmwareupgrades.  Simply sleep 1 second.
        sys::delay(1000);
        PurgeComPort(port().c_str());
        int tempversion;
        ASSERT_OK(GetControllerVersion(tempversion));
        if (tempversion < g_Min_MMVersion)
            THROW_DEVICE_ERROR(rdlmm::ERR_VERSION_MISMATCH);
        ASSERT_OK(version_.create(this, g_infoVersion));
        ASSERT_OK(version_.SetProperty(tempversion));
        ASSERT_OK(foo_.create(this, client_, g_infoFoo));
        ASSERT_OK(UpdateStatus());
        initialized_ = true;
        return DEVICE_OK;

    } catch (rdlmm::DeviceResultException dre) {
        LogMessage(dre.format(this));
        return dre.error;
    }
}

int TeensyHub::DetectInstalledDevices() {
    std::cout << "###### DetectInstalledDevices()" << std::endl;
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

int TeensyHub::Shutdown() {
    std::cout << "###### Shutdown()" << std::endl;
    initialized_ = false;
    return DEVICE_OK;
}


//#############################################################################
//### class TeensyDACGalvo
//#############################################################################

////////////////////////////////////////////////////////////////
/// \name constructors and destructors
////////////////////////////////////////////////////////////////
///@{
TeensyDACGalvo::TeensyDACGalvo(int __chan) : chan_(__chan) {
    InitializeDefaultErrorMessages();
    rdlmm::InitCommonErrors(this, "Teensy", g_Min_MMVersion);
#if defined(GALVOS_SEQUENCABLE)
    posSequence_.clear();
#endif
}

TeensyDACGalvo::~TeensyDACGalvo() {
    TeensyDACGalvo::Shutdown();
}
///@}

//////////////////////////////////////////////////////////
/// \name MM::Device implementation
//////////////////////////////////////////////////////////
///@{
int TeensyDACGalvo::Shutdown() {
    return DEVICE_OK;
}

int TeensyDACGalvo::Initialize() {
    TeensyHub* hub = dynamic_cast<TeensyHub*>(GetParentHub());
    if (!hub || !hub->IsPortAvailable()) {
        return rdlmm::ERR_NO_PORT_SET;
    }
    try {
        CommandSet cmds = CommandSet::build(); // dummy to initialize

#if defined(GALVOS_SEQUENCABLE)
        cmds = CommandSet::build().withChan(chan_).withSet(GAL_SET_POS).withGet(GAL_GET_POS).withSetSeq(GAL_SETSEQ_POS).withGetSeq(GAL_GETSEQ_POS).withStartSeq(GAL_STARTSEQ_POS).withStopSeq(GAL_STOPSEQ_POS);
        assertOK(pos_.createRemoteProp(this, hub, g_infoGalvoPosition, cmds));
        cmds = CommandSet::build().withGet(GAL_GET_ARRAYSEQ_POS);
        assertOK(posSeqRead_.createRemoteProp(this, hub, g_infoGalvoPosSeq, cmds));

#else
        cmds = CommandSet::build().withChan(chan_).withSet(GAL_SET_POS).withGet(GAL_GET_POS);
        assertOK(pos_.createRemoteProp(this, hub, g_infoGalvoPosition, cmds));
#endif

        cmds = CommandSet::build().withChan(chan_).withSet(GAL_SET_OFF).withGet(GAL_GET_OFF);
        assertOK(off_.createRemoteProp(this, hub, g_infoGalvoOffset, cmds));

        cmds = CommandSet::build().withChan(chan_).withSet(GAL_SET_FREQ).withGet(GAL_GET_FREQ);
        assertOK(freq_.createRemoteProp(this, hub, g_infoGalvoFrequency, cmds));

    } catch (DeviceResultException deviceError) {
        LogMessage(deviceError.format(this));
        std::string lastTrans = hub->getLastLog();
        if (!lastTrans.empty()) {
            LogMessage(std::string("Last Transaction: ") + lastTrans);
        }
        return deviceError.error;
    }
    return DEVICE_OK;
}
///@}

//////////////////////////////////////////////////////////
/// \name MM::Device implementation
//////////////////////////////////////////////////////////
///@{

bool TeensyDACGalvo::Busy() {
    //hprot::prot_bool_t moving;
    //TeensyDACHub* hub = static_cast<TeensyDACHub*>(GetParentHub());

    //if (!hub->dispatchGet(CHAN_GET_IS_MOVING, moving)) {
    //	// not busy if error received
    //	return false;
    //}
    //return moving != 0;
    return false;
}

///@}

//////////////////////////////////////////////////////////
/// \name MM::Stage implementation
//////////////////////////////////////////////////////////
///@{

int TeensyDACGalvo::SetPositionUm(double pos) {
    // Treat position in um as wavelength in nm
    return pos_.setProperty(static_cast<position_t>(pos));
}

int TeensyDACGalvo::GetPositionUm(double& pos) {
    // Treat position in um as wavelength in nm
    pos = pos_.getCachedValue();
    return DEVICE_OK;
}

double TeensyDACGalvo::GetStepSize() const {
    // Treat position in um as wavelength in nm
    return POSITION_PRECISION;
}

int TeensyDACGalvo::SetPositionSteps(long steps) {
    // Treat position in um as wavelength in nm
    double pos_nm = steps * POSITION_PRECISION + MIN_POSITION;
    return OnStagePositionChanged(pos_nm);
}

int TeensyDACGalvo::GetPositionSteps(long& steps) {
    position_t pos_voltage = pos_.getCachedValue();
    steps                  = static_cast<long>((pos_voltage - MIN_POSITION) / POSITION_PRECISION);
    return DEVICE_OK;
}

int TeensyDACGalvo::GetLimits(double& lower, double& upper) {
    // Treat position in um as wavelength in nm
    lower = MIN_POSITION;
    upper = MAX_POSITION;
    return DEVICE_OK;
}

#if defined(GALVOS_SEQUENCABLE)
int TeensyDACGalvo::GetStageSequenceMaxLength(long& nrEvents) const {
    hprot::prot_size_t size;
    int ret;
    if ((ret = pos_.getRemoteSequenceSize(size)) == DEVICE_OK) {
        nrEvents = size;
    }
    return ret;
};

int TeensyDACGalvo::StartStageSequence() {
    int ret = pos_.startRemoteSequence();
    return ret;
}

int TeensyDACGalvo::StopStageSequence() {
    int ret = pos_.stopRemoteSequence();
    return ret;
};

int TeensyDACGalvo::ClearStageSequence() {
    posSequence_.clear();
    return DEVICE_OK;
};

int TeensyDACGalvo::AddToStageSequence(double pos_um) {
    posSequence_.push_back(std::to_string(pos_um));
    return DEVICE_OK;
}

int TeensyDACGalvo::SendStageSequence() {
    if (posSequence_.size() > 0) {
        return pos_.setRemoteSequence(posSequence_);
    }
    return DEVICE_OK;
}
#endif

///@}