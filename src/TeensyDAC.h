#pragma once

#ifndef __TEENSYDAC_H__
    #define __TEENSYDAC_H__

    #define NOMINMAX
    #include "DeviceBase.h"
    #include "MMDevice.h"
    #include <rdlmm/DeviceError.h>
    #include <rdlmm/DeviceLog.h>
    #include <rdlmm/DeviceProp.h>
    #include <rdlmm/DevicePropHelpers.h>
    #include <rdlmm/LocalProp.h>
    #include <rdlmm/RemoteProp.h>
    #include <rdlmm/Stream_HubSerial.h>
    #include <string>

    #define MAX_NUM_GALVOS 8

constexpr char* g_FirmwareName    = "MM-TeensyDAC2";
constexpr char* g_deviceNameHub   = "TeensyDAC2-Hub";
constexpr char* g_deviceDescHub   = "Teensy Hub";
constexpr char* g_deviceNameGalvo = "TeensyDAC2-Gal";
constexpr char* g_deviceDescGalvo = "Galvo Axis";

const auto g_infoName      = rdlmm::PropInfo<std::string>::build(MM::g_Keyword_Name, g_deviceNameHub).readOnly();
const auto g_infoPort      = rdlmm::PropInfo<std::string>::build(MM::g_Keyword_Port, "").preInit(); // initial port must be empty string
const auto g_infoNumGalvos = rdlmm::PropInfo<int>::build("Number of Galvos", 0).preInit();

const auto g_infoVersion = rdlmm::PropInfo<int>::build("Firmware Version", 0).readOnly();
const auto g_infoIsAlive = rdlmm::PropInfo<bool>::build("Is Alive", false).readOnly();
const auto g_infoFoo     = rdlmm::PropInfo<int>::build("Foo", 0).withBrief("foo").withLimits(-1000, 1000).sequencable();
const auto g_infoDACPos  = rdlmm::PropInfo<double>::build("Position", 0).withBrief("pos").withLimits(-10.0, 10.0).sequencable();

//class TeensyInputMonitorThread;

class TeensyHub : public HubBase<TeensyHub> {
 public:
    using HubT          = TeensyHub;
    using LoggerT       = rdlmm::DeviceLog_Print<HubT>;
    using SerialStreamT = rdlmm::Stream_HubSerial<HubT>;
    //using KeysT         = rdl::jsonrpc_default_keys;
    //using ClientT       = rdl::json_client<KeysT>;

    TeensyHub();
    ~TeensyHub() {
        Shutdown();
    }

    int Initialize();
    int Shutdown();
    void GetName(char* name) const {
        CDeviceUtils::CopyLimitedString(name, g_deviceNameHub);
    }
    bool Busy() {
        return false;
    }

    bool SupportsDeviceDetection(void) {
        return true;
    }
    MM::DeviceDetectionStatus DetectDevice(void);
    int DetectInstalledDevices();

    bool port(const std::string& portname) {
        int ret = port_.SetProperty(portname);
        return ret;
    }

    const std::string port() {
        std::string res;
        port_.GetCachedProperty(res);
        return res;
    }

    rdl::json_client<rdl::jsonrpc_default_keys> client() {
        return client_;
    }

    bool IsPortAvailable() { return initialized_; }

    //// custom interface for child devices
    //bool IsPortAvailable() {return portAvailable_;}
    //bool IsLogicInverted() {return invertedLogic_;}
    //bool IsTimedOutputActive() {return timedOutputActive_;}
    //void SetTimedOutput(bool active) {timedOutputActive_ = active;}

    //int PurgeComPortH() {return PurgeComPort(port_.c_str());}
    //int WriteToComPortH(const unsigned char* command, unsigned len) {return WriteToComPort(port_.c_str(), command, len);}
    //int ReadFromComPortH(unsigned char* answer, unsigned maxLen, unsigned long& bytesRead)
    //{
    //   return ReadFromComPort(port_.c_str(), answer, maxLen, bytesRead);
    //}
    //static MMThreadLock& GetLock() {return lock_;}
    //void SetShutterState(unsigned state) {shutterState_ = state;}
    //void SetSwitchState(unsigned state) {switchState_ = state;}
    //unsigned GetShutterState() {return shutterState_;}
    //unsigned GetSwitchState() {return switchState_;}

 protected:
    int GetControllerVersion(int&);
    rdlmm::LocalProp<HubT, std::string> port_;
    rdlmm::LocalProp<HubT, int> version_;
    rdlmm::RemoteSimpleProp<HubT, int> foo_;
    bool initialized_;
    LoggerT logger_;
    SerialStreamT serial_;
    rdl::json_client<rdl::jsonrpc_default_keys> client_;
};

#if 1
class TeensyDACGalvo : public CStageBase<TeensyDACGalvo> {
 public:
    using HubT = TeensyHub;
    TeensyDACGalvo(int __chan, HubT* hub);
    virtual ~TeensyDACGalvo();

    void GetName(char* __name) const override {
        std::string name = g_deviceNameGalvo;
        name += char('A' + chan_);
        CDeviceUtils::CopyLimitedString(__name, name.c_str());
    }
    bool Busy() override;
    int Shutdown() override;
    int Initialize() override;

    double toLocal(int16_t dacval);
    int16_t toRemote(double dacvolt);

    /////////////////////////////////////////////////////////////
    /// MM::Stage implementation
    /// NOTE: Although Stage Position function is in um,
    ///       we will use galvo position in terms of voltage
    ///		  as this fits better with MM's MD acquisition
    //////////////////////////////////////////////////////////
    ///@{
    int SetPositionUm(double pos) override;
    int GetPositionUm(double& pos) override;
    double GetStepSize() const;
    int SetPositionSteps(long steps) override;
    int GetPositionSteps(long& steps) override;
    int GetLimits(double& lower, double& upper) override;

    int SetOrigin() override { return DEVICE_OK; }
    int Move(double /*v*/) override { return DEVICE_OK; }
    bool IsContinuousFocusDrive() const override { return false; }

    // Sequence functions
    int IsStageSequenceable(bool&) const override {
        return true;
    };
    int GetStageSequenceMaxLength(long& nrEvents) const override;
    int StartStageSequence() override;
    int StopStageSequence() override;
    int ClearStageSequence() override;
    int AddToStageSequence(double pos_um) override;
    int SendStageSequence() override;
    ///@}

 protected:
    HubT* hub_;
    const double max_voltage_ = +10.0;
    const double min_voltage_ = -10.0;
    int chan_;
    std::vector<std::string> posSequence_;
    rdlmm::RemoteChannelProp<TeensyDACGalvo, double, int16_t> pos_;
    //rdlmm::RemoteChannelProp<HubT, double, long> off_;
    //rdlmm::RemoteChannelProp<HubT, double> freq_;
}; // TeensyDACGalvo

#endif

//class CTeensyShutter : public CShutterBase<CTeensyShutter>
//{
//public:
//   CTeensyShutter();
//   ~CTeensyShutter();
//
//   // MMDevice API
//   // ------------
//   int Initialize();
//   int Shutdown();
//
//   void GetName(char* pszName) const;
//   bool Busy();
//
//   // Shutter API
//   int SetOpen(bool open = true);
//   int GetOpen(bool& open);
//   int Fire(double deltaT);
//
//   // action interface
//   // ----------------
//   int OnOnOff(MM::PropertyBase* pProp, MM::ActionType eAct);
//
//private:
//   int WriteToPort(long lnValue);
//   MM::MMTime changedTime_;
//   bool initialized_;
//   std::string name_;
//};
//
//class CTeensySwitch : public CStateDeviceBase<CTeensySwitch>
//{
//public:
//   CTeensySwitch();
//   ~CTeensySwitch();
//
//   // MMDevice API
//   // ------------
//   int Initialize();
//   int Shutdown();
//
//   void GetName(char* pszName) const;
//   bool Busy() {return busy_;}
//
//   unsigned long GetNumberOfPositions()const {return numPos_;}
//
//   // action interface
//   // ----------------
//   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnDelay(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnRepeatTimedPattern(MM::PropertyBase* pProp, MM::ActionType eAct);
//   /*
//   int OnSetPattern(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnGetPattern(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnPatternsUsed(MM::PropertyBase* pProp, MM::ActionType eAct);
//   */
//   int OnSkipTriggers(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnStartTrigger(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnStartTimedOutput(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnBlanking(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnBlankingTriggerDirection(MM::PropertyBase* pProp, MM::ActionType eAct);
//
//   int OnSequence(MM::PropertyBase* pProp, MM::ActionType eAct);
//
//private:
//   static const unsigned int NUMPATTERNS = 12;
//
//   int OpenPort(const char* pszName, long lnValue);
//   int WriteToPort(long lnValue);
//   int ClosePort();
//   int LoadSequence(unsigned size, unsigned char* seq);
//
//   unsigned pattern_[NUMPATTERNS];
//   unsigned delay_[NUMPATTERNS];
//   int nrPatternsUsed_;
//   unsigned currentDelay_;
//   bool sequenceOn_;
//   bool blanking_;
//   bool initialized_;
//   long numPos_;
//   bool busy_;
//};
//
//class CTeensyDA : public CSignalIOBase<CTeensyDA>
//{
//public:
//   CTeensyDA(int channel);
//   ~CTeensyDA();
//
//   // MMDevice API
//   // ------------
//   int Initialize();
//   int Shutdown();
//
//   void GetName(char* pszName) const;
//   bool Busy() {return busy_;}
//
//   // DA API
//   int SetGateOpen(bool open);
//   int GetGateOpen(bool& open) {open = gateOpen_; return DEVICE_OK;};
//   int SetSignal(double volts);
//   int GetSignal(double& volts) {volts_ = volts; return DEVICE_UNSUPPORTED_COMMAND;}
//   int GetLimits(double& minVolts, double& maxVolts) {minVolts = minV_; maxVolts = maxV_; return DEVICE_OK;}
//
//   int IsDASequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
//
//   // action interface
//   // ----------------
//   int OnVolts(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnMaxVolt(MM::PropertyBase* pProp, MM::ActionType eAct);
//   int OnChannel(MM::PropertyBase* pProp, MM::ActionType eAct);
//
//private:
//   int WriteToPort(unsigned long lnValue);
//   int WriteSignal(double volts);
//
//   bool initialized_;
//   bool busy_;
//   double minV_;
//   double maxV_;
//   double volts_;
//   double gatedVolts_;
//   unsigned channel_;
//   unsigned maxChannel_;
//   bool gateOpen_;
//   std::string name_;
//};
//
//class CTeensyInput : public CGenericBase<CTeensyInput>
//{
//public:
//   CTeensyInput();
//   ~CTeensyInput();
//
//   int Initialize();
//   int Shutdown();
//   void GetName(char* pszName) const;
//   bool Busy();
//
//   int OnDigitalInput(MM::PropertyBase* pPropt, MM::ActionType eAct);
//   int OnAnalogInput(MM::PropertyBase* pProp, MM::ActionType eAct, long channel);
//
//   int GetDigitalInput(long* state);
//   int ReportStateChange(long newState);
//
//private:
//   int ReadNBytes(CTeensyHub* h, unsigned int n, unsigned char* answer);
//   int SetPullUp(int pin, int state);
//
//   MMThreadLock lock_;
//   TeensyInputMonitorThread* mThread_;
//   char pins_[MM::MaxStrLength];
//   char pullUp_[MM::MaxStrLength];
//   int pin_;
//   bool initialized_;
//   std::string name_;
//};
//
//class TeensyInputMonitorThread : public MMDeviceThreadBase
//{
//   public:
//      TeensyInputMonitorThread(CTeensyInput& aInput);
//     ~TeensyInputMonitorThread();
//      int svc();
//      int open (void*) { return 0;}
//      int close(unsigned long) {return 0;}
//
//      void Start();
//      void Stop() {stop_ = true;}
//      TeensyInputMonitorThread & operator=( const TeensyInputMonitorThread & )
//      {
//         return *this;
//      }
//
//
//   private:
//      long state_;
//      CTeensyInput& aInput_;
//      bool stop_;
//};

#endif //__TEENSYDAC_H__
