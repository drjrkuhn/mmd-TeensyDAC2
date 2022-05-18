#include <Arduino.h>

#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1

// #define JSONRPC_DEBUG_CLIENTSERVER 1
#define JSONRPC_DEBUG_SERVER_DISPATCH 1

#include <Ardulingua.h>
#include <unordered_map>

using namespace rdl;

using StreamT = decltype(Serial);
using MapT    = std::unordered_map<sys::StringT, json_stub, sys::string_hash>;
using KeysT   = jsonrpc_default_keys;

#if defined(USB_DUAL_SERIAL) || defined(USB_TRIPLE_SERIAL)
auto logger = SerialUSB1;
void logger_begin()
{
    SerialUSB1.begin(9600);
    while (!SerialUSB1) /*noop*/
        ;
}
#else
auto logger = Null_Print;
void logger_begin() {}
#endif

sys::StringT g_firmware_name("MM-TeensyDAC2");
const int g_firmware_version = 1;
const int MAX_SEQ_SIZE = 512;
const int MAX_DAC_CHAN = 8;
const int g_num_dac_chan = 4;

/** 
 * Firmware double check. 
 * Caller has to pass the correct firmware name to get a positive firmware version number.
 * Allows for two-way firmware check with a single RPC call.
 * 
 * Drivers can also call "?fname" to get the name first, then call get_firmware_version.
 */
int get_firmware_version(sys::StringT name)
{
    return (g_firmware_name == name) ? g_firmware_version : -1;
}

// Start the dispatch map with some simple properties.
// Other properties will be added in setup() below
MapT dispatch_map{
    {"?fname", json_delegate<RetT<sys::StringT>>::create([]() { return g_firmware_name; }).stub()},
    {"?fver", json_delegate<RetT<int>, sys::StringT>::create<get_firmware_version>().stub()},
};

rdl::simple_prop<int> foo("foo", 1, 32);

rdl::simple_prop<double> bar0("bar0", 1.1, 32);
rdl::simple_prop<double> bar1("bar1", 2.2, 32);
rdl::simple_prop<double> bar2("bar2", 3.3, 32);
rdl::simple_prop<double> bar3("bar3", 4.4, 32);
decltype(bar0)::RootT* all_bars[] = {&bar0, &bar1, &bar2, &bar3};
rdl::channel_prop<double> bars("bar", all_bars, 4);

// The server
using ServerT = json_server<MapT, KeysT, 512>;
ServerT server(Serial, Serial, dispatch_map);

rdl::channel_prop<int16_t> dac_vals("dv", MAX_DAC_CHAN);

void setup_dispatch()
{
    for (int i=0; i<g_num_dac_chan; i++) {
        sys::StringT name = "dv";
        name += ('A'+i);
        rdl::simple_prop<int16_t> dval(name, 0, MAX_SEQ_SIZE);
        dac_vals.add(&dval);
    }
    add_to<MapT, decltype(dac_vals)::RootT>(dispatch_map, dac_vals, dac_vals.sequencable(-1), dac_vals.read_only(-1));

    add_to<MapT, decltype(foo)::RootT>(dispatch_map, foo, foo.sequencable(), foo.read_only());
    add_to<MapT, decltype(bars)::RooT>(dispatch_map, bars, bars.sequencable(-1), bars.read_only(-1));
}

void setup()
{

    Serial.begin(9600);
    Serial.setTimeout(2000); // longer timeout on virtual machine
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    logger_begin();
    server.logger(&logger);
    logger.println("log started for ArduinoCoreTestFirmware");

    foo.logger(&logger);
    bars.logger(&logger);
    bar0.logger(&logger);
    bar1.logger(&logger);
    bar2.logger(&logger);
    bar3.logger(&logger);

    setup_dispatch();
    logger.println("Map methods:");
    for (auto p : dispatch_map)
    {
        logger.println(p.first.c_str());
    }
}

void loop()
{
    server.check_messages();
    sys::delay(1);
}
