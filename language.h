#ifndef language_h
#define language_h

#include "Arduino.h"

extern String str(const char *ptr);
extern String keyword(const char *keywordPtr);
extern bool eqls(const char *str, const char *keywordPtr);
extern bool eqls(String str, const char *keywordPtr);
extern String b2s(bool input);
extern String b2a(bool input);
extern bool s2b(String input);
extern void prnt(String s);
extern void prnt(bool b);
extern void prnt(char c);
extern void prnt(const char *ptr);
extern void prnt(int i);
extern void prntln();
extern void prntln(String s);
extern void prntln(bool b);
extern void prntln(char c);
extern void prntln(const char *ptr);
extern void prntln(int i);

// ===== GLOBAL STRINGS ===== //

// Often used characters, therefor in the RAM
const char CURSOR = '|';
const char SPACE = ' ';
const char DOUBLEPOINT = ':';
const char EQUALS = '=';
const char HASHSIGN = '#';
const char ASTERIX = '*';
const char PERCENT = '%';
const char DASH = '-';
const char QUESTIONMARK = '?';
const char ZERO = '0';
const char S = 's';
const char M = 'm';
const char D = 'd';
const char DOUBLEQUOTES = '\"';
const char SLASH = '/';
const char NEWLINE = '\n';
const char CARRIAGERETURN = '\r';
const char SEMICOLON = ';';
const char BACKSLASH = '\\';
const char POINT = '.';
const char VERTICALBAR = '|';
const char COMMA = ',';
const char ENDOFLINE = '\0';
const char OPEN_BRACKET = '[';
const char CLOSE_BRACKET = ']';
const char OPEN_CURLY_BRACKET = '{';
const char CLOSE_CURLY_BRACKET = '}';

const char STR_TRUE[] PROGMEM = "true";
const char STR_FALSE[] PROGMEM = "false";
const char STR_MIN[] PROGMEM = "min";

// ===== SETUP ===== //
const char SETUP_OK[] PROGMEM = "OK";
const char SETUP_ERROR[] PROGMEM = "ERROR";
const char SETUP_STARTED[] PROGMEM = "STARTED! \\o/";


// ===== SERIAL COMMAND LINE INTERFACE ===== //
const char CLI_SCAN[] PROGMEM = "scan";                // scan
const char CLI_STATUS[] PROGMEM = "status";            // status
const char CLI_SHOW[] PROGMEM = "show";                // show
const char CLI_REMOVE[] PROGMEM = "remove";            // remove
const char CLI_SET[] PROGMEM = "set";                  // set
const char CLI_STOP[] PROGMEM = "stop";                // stop
const char CLI_LOAD[] PROGMEM = "load";                // load
const char CLI_SAVE[] PROGMEM = "save";                // save
const char CLI_ADD[] PROGMEM = "add";                  // add
const char CLI_DESELECT[] PROGMEM = "deselect";        // deselect
const char CLI_CLEAR[] PROGMEM = "clear";              // clear
const char CLI_SYSINFO[] PROGMEM = "sysinfo";          // sysinfo
const char CLI_ON[] PROGMEM = "on";                    // on
const char CLI_OFF[] PROGMEM = "off";                  // off
const char CLI_RANDOM[] PROGMEM = "random";            // random
const char CLI_GET[] PROGMEM = "get";                  // get
const char CLI_INFO[] PROGMEM = "info";                // info
const char CLI_FORMAT[] PROGMEM = "format";            // format
const char CLI_DELETE[] PROGMEM = "Yes";            // delete
const char CLI_PRINT[] PROGMEM = "print";              // print
const char CLI_RUN[] PROGMEM = "run";                  // run
const char CLI_WRITE[] PROGMEM = "write";              // write
const char CLI_SEND[] PROGMEM = "send";                // send
const char CLI_CUSTOM[] PROGMEM = "custom";            // custom
const char CLI_DELAY[] PROGMEM = "delay";              // delay
const char CLI_REPLACE[] PROGMEM = "replace";          // replace
const char CLI_DRAW[] PROGMEM = "draw";                // draw
const char CLI_SCRIPT[] PROGMEM = "script";            // script
const char CLI_STARTAP[] PROGMEM = "startap";          // startap
const char CLI_STOPAP[] PROGMEM = "stopap";            // stopap
const char CLI_RENAME[] PROGMEM = "rename";            // rename
const char CLI_COPY[] PROGMEM = "copy";                // copy
const char CLI_ENABLE[] PROGMEM = "enable/d";          // enable, enabled
const char CLI_DISABLE[] PROGMEM = "disable/d";        // disable, disabled
const char CLI_WPA2[] PROGMEM = "wpa/2,-wpa/2";        // wpa, wpa2, -wpa, -wpa2
const char CLI_ATTACK[] PROGMEM = "attack/s";          // attack, attacks
const char CLI_SETTING[] PROGMEM = "setting/s";        // setting, settings
const char CLI_ID[] PROGMEM = "id,-i/d";               // id, -i, -id
const char CLI_ALL[] PROGMEM = "all,-a";               // all, -a
const char CLI_TIME[] PROGMEM = "time,-t";             // time, -t
const char CLI_CONTINUE[] PROGMEM = "continue,-c";     // continue, -c
const char CLI_CHANNEL[] PROGMEM = "channel,-ch";      // channel, -ch
const char CLI_MAC[] PROGMEM = "mac,-m";               // mac, -m
const char CLI_BSSID[] PROGMEM = "bssid,-b";           // bssid, -b
const char CLI_BEACON[] PROGMEM = "beacon,-b";         // bssid, -b
const char CLI_DEAUTH[] PROGMEM = "deauth,-d";         // deauth, -d
const char CLI_DEAUTHALL[] PROGMEM = "deauthall,-da";  // deauthall, -da
const char CLI_PROBE[] PROGMEM = "probe,-p";           // probe, -p
const char CLI_NOOUTPUT[] PROGMEM = "nooutput,-no";    // nooutput, -no
const char CLI_FORCE[] PROGMEM = "force,-f";           // force, -f
const char CLI_TIMEOUT[] PROGMEM = "timeout,-t";       // timeout, -t
const char CLI_WIFI[] PROGMEM = "wifi,-w";             // wifi, -w
const char CLI_CLONES[] PROGMEM = "clones,-cl";        // clones, -cl
const char CLI_PATH[] PROGMEM = "path,-p";             // path, -p
const char CLI_PASSWORD[] PROGMEM = "password,-ps/wd"; // password, -ps, -pswd
const char CLI_HIDDEN[] PROGMEM = "hidden,-h";         // hidden, -h
const char CLI_CAPTIVEPORTAL[] PROGMEM =
    "captiveportal,-cp";                            // captiveportal, -cp
const char CLI_SELECT[] PROGMEM = "select/ed,-s";   // select, selected, -s
const char CLI_SSID[] PROGMEM = "ssid/s,-s/s";      // ssid, ssids, -s, -ss
const char CLI_AP[] PROGMEM = "ap/s,-ap/s";         // ap, aps, -ap, -aps
const char CLI_STATION[] PROGMEM = "station/s,-st"; // station, stations, -st
const char CLI_NAME[] PROGMEM = "name/s,-n";        // name, names, -n
const char CLI_LINE[] PROGMEM = "line/s,-l";        // line, lines, -l
const char CLI_COMMENT[] PROGMEM = "//";            // //
const char CLI_SCREEN[] PROGMEM = "screen";         // screen
const char CLI_MODE[] PROGMEM = "mode,-m";          // mode
const char CLI_MODE_BUTTONTEST[] PROGMEM = "buttontest";       // buttontest
const char CLI_MODE_PACKETMONITOR[] PROGMEM = "packetmonitor"; // packetmonitor
const char CLI_MODE_LOADINGSCREEN[] PROGMEM = "loadingscreen"; // loading
const char CLI_MODE_MENU[] PROGMEM = "menu";                   // menu
const char CLI_SAVE_WIFI_CREDENTIAL[] PROGMEM = "Savewifi";
const char CLI_WIFI_CREDENTIAL[] PROGMEM = "Passwords";
const char CLI_CREDENTIAL_DELETE[] PROGMEM = "credentialdelete";
const char CLI_INPUT_PREFIX[] PROGMEM = "# ";
const char CLI_ERROR[] PROGMEM = "ERROR: ";
const char CLI_ERROR_PARAMETER[] PROGMEM = "Error Invalid parameter \"";


const char CLI_SYSTEM_OUTPUT[] PROGMEM =
    "RAM usage: %u bytes used [%d%%], %u bytes free [%d%%], %u bytes in "
    "total\r\n";
const char CLI_SYSTEM_RAM_OUT[] PROGMEM =
    "SPIFFS: %u bytes used [%d%%], %u bytes free [%d%%], %u bytes in total\r\n";
const char CLI_SYSTEM_SPIFFS_OUT[] PROGMEM =
    "        block size %u bytes, page size %u bytes\r\n";

const char CLI_FILES[] PROGMEM = "Files: ";
const char CLI_BYTES[] PROGMEM = " bytes";
const char CLI_WRITTEN[] PROGMEM = "Written \"";
const char CLI_TO[] PROGMEM = "\" to ";
const char CLI_WITH[] PROGMEM = " with ";

const char CLI_DRAW_OUTPUT[] PROGMEM = "%+4u";
const char CLI_ERROR_NOT_FOUND_A[] PROGMEM = "ERROR: command \"";
const char CLI_ERROR_NOT_FOUND_B[] PROGMEM = "\" not found :(";
const char CLI_DEFAULT_AUTOSTART[] PROGMEM = "scan -t 5s\nsysinfo\n";


// LOADING SCREEN
const char DSP_SCAN_FOR[] PROGMEM = "Scan for";
const char DSP_APS[] PROGMEM = "APs";
const char DSP_STS[] PROGMEM = "STs";
const char DSP_PKTS[] PROGMEM = "Pkts";
const char DSP_S[] PROGMEM = "/s";
const char DSP_SCAN_DONE[] PROGMEM = "Done";

// ALL MENUS
const char D_BACK[] PROGMEM = "[BACK]";
const char D_REMOVE_ALL[] PROGMEM = "REMOVE ALL";
const char D_SELECT[] PROGMEM = "SELECT";
const char D_DESELECT[] PROGMEM = "DESELECT";
const char D_REMOVE[] PROGMEM = "REMOVE";
const char D_SELECT_ALL[] PROGMEM = "SELECT ALL";
const char D_DESELECT_ALL[] PROGMEM = "DESELECT ALL";
const char D_CLONE[] PROGMEM = "CLONE SSID";
const char D_SET_WIFI[] PROGMEM = "SET NETWORK";
const char D_EMPTY[] PROGMEM = "---EMPTY---";

// const char KEY_ENTER[] PROGMEM = "en";
// const char KEY_DEL[] PROGMEM = "del";
// const char KEY_MODE[] PROGMEM = "Aa";

// BUTTON TEST
const char D_UP[] PROGMEM = "UP:";
const char D_DOWN[] PROGMEM = "DOWN:";
const char D_LEFT[] PROGMEM = "LEFT:";
const char D_RIGHT[] PROGMEM = "RIGHT:";
const char D_B[] PROGMEM = "A:";
const char D_A[] PROGMEM = "B:";

// MAIN MENU
const char D_SCAN[] PROGMEM = "Scan";
const char D_SHOW[] PROGMEM = "Select";
const char D_ATTACK[] PROGMEM = "Attack";
const char D_ATTACK1[] PROGMEM = "Do you want to change";
const char D_DEAUTH_REASON[] PROGMEM = "the DeauthReason?";
const char D_CHANNEL1[] PROGMEM = "the Channel";
const char D_DEAUTH_REASON1[] PROGMEM = "DeauthReason";
const char D_DEAUTH_PACKET[] PROGMEM = "DeauthPacket";
const char D_MIN_DEAUTH[] PROGMEM = "MinDeauths";
const char D_MINMAC_FORCE[] PROGMEM = "0 < ForcePackets < 255";
const char D_DEAUTHPACKETS[] PROGMEM = "Min = 20";
const char D_MINDEAUTH[] PROGMEM = "Max = 10";
const char D_REASONMAX[] PROGMEM = "RS < 24, default = 1 ";
const char D_CHANNELTIMEMIN[] PROGMEM = "Min = 384";
const char D_CHANNELMAX[] PROGMEM = "US = 11,EU = 13,Japan = 14";
const char D_CHANNELMAX1[] PROGMEM = "Max = 14";
const char D_MIN_DEAUTH1[] PROGMEM = "the MinDeauths?";
const char D_PROBEPERSSID1[] PROGMEM = "the ProbePerSSID?";
const char D_FORCE1[] PROGMEM = "the ForcePackets?"; 
const char D_SET_FORCE_PACKETS[] PROGMEM = "ForcePackets";
const char D_DEAUTHPACKET[] PROGMEM = "the DeauthPerPacket?";
const char D_SET_ATTACK_TIMEOUT1[] PROGMEM = "the AttackTimeout?";
const char D_PACKET_MONITOR[] PROGMEM = "Packet Monitor";
const char D_SET_CLOCK[] PROGMEM = "Clock";
const char D_HACK_AND_CAPTIVE_PORTAL[] PROGMEM = "Hack Wifi Password";
const char D_CREDENTIAL[] PROGMEM = "Passwords";
const char D_WIFI1[] PROGMEM = "Passwords";
const char D_HACK_WIFI[] PROGMEM = "Hack WiFi";
const char D_BRIGHTNESS[] PROGMEM = "Brightness";
const char D_TIME_ON_SCREEN[] PROGMEM = "Setting";
const char D_EXIT[] PROGMEM = "Exit";
const char D_INFO[] PROGMEM = "Info";

// SCAN MENU
const char D_SCAN_APST[] PROGMEM = "SCAN AP + ST";
const char D_SCAN_AP[] PROGMEM = "SCAN APs";
const char D_SCAN_ST[] PROGMEM = "SCAN Stations";

// SHOW MENU
const char D_ACCESSPOINTS[] PROGMEM = "APs ";
const char D_STATIONS[] PROGMEM = "Stations ";
const char D_NAMES[] PROGMEM = "Names ";
const char D_SSIDS[] PROGMEM = "SSIDs ";

// SSID LIST MENU
const char D_CLONE_APS[] PROGMEM = "CLONE APs";
const char D_RANDOM_MODE[] PROGMEM = "RANDOM MODE";

// ATTACK MENU
const char D_DEAUTH[] PROGMEM = "DEAUTH";
const char D_BEACON[] PROGMEM = "BEACON";
const char D_PROBE[] PROGMEM = "PROBE";
const char D_START_ATTACK[] PROGMEM = "START";
const char D_STOP_ATTACK[] PROGMEM = "STOP";

// SUB MENUS
const char D_ENCRYPTION[] PROGMEM = "Encryption:";
const char D_RSSI[] PROGMEM = "RSSI:";
const char D_CHANNEL[] PROGMEM = "Channel:";
const char D_CH[] PROGMEM = "Ch";
const char D_VENDOR[] PROGMEM = "Vendor:";
const char D_AP[] PROGMEM = "AP:";
const char D_PKTS[] PROGMEM = "pkts";
const char D_SEEN[] PROGMEM = "Seen:";

const char D_CAPTIVE_PORTAL_OFF_MESSAGE[] PROGMEM = "Turn off";
const char D_RESET[] PROGMEM = "Reset";
const char D_RESET2[] PROGMEM = "Do you want to";
const char D_MESSAGE_RESET_DEVICE1[] PROGMEM = "factory reset?";

// STATUS INDICATOR
const char D_SUCCESS[] PROGMEM = "Success";

const char D_TITLE_TURN_ON_CAPTIVE[] PROGMEM = "Turn on";
const char D_TITLE_TURN_OFF_CAPTIVE[] PROGMEM = "Turn off";
const char D_TITLE_DELETE_CREDENTIAL[] PROGMEM = "delete this password?";
const char D_TITLE_SET_CAPTIVE_STOP_WIFI_HACKING[] PROGMEM = "Do you want to stop?";
const char D_AGREE_ERASE_CREDENTIAL[] PROGMEM = "Erase";
const char CLI_DELETE1[] PROGMEM = "Do you want to";
// WFI SCREEN
const char D_GET_WIFI_SCAN_HACK_WIFI[] PROGMEM = "Hack wifi password";

// WIFI Mode
const char D_WIFI_MODE_AP[] PROGMEM = "AP";
const char D_WIFI_MODE_AP_STA[] PROGMEM = "AP_STA";
const char D_WIFI_MODE_STA[] PROGMEM = "STA";
const char D_WIFI_MODE_OFF[] PROGMEM = "OFF";
const char D_CANCEL_BUTTON[] PROGMEM = "No";
const char D_YES_BUTTON[] PROGMEM = "Yes";
const char D_AGREE_BUTTON[] PROGMEM = "Yes";
const char D_SUCCESS_ALERT[] PROGMEM = "Success!";
const char D_ATTACKALL_ALERT[] PROGMEM = "Attack all running";
const char D_STOPATTACK_ALERT[] PROGMEM = "Attack has stopped";
const char D_FAILURE_ALERT[] PROGMEM = "Failure";
const char D_SET_NEW_SSID[] PROGMEM = "Set new ssid";
const char D_SET_PASS_FOR_SSID[] PROGMEM = "ssid with password";
const char D_HACK_SSID[] PROGMEM = "Hack this ssid?";
const char D_SET_ATTACK_TIMEOUT[] PROGMEM = "AttackTimeout";
const char D_INPUT_NUMBER_WRONG_FORMAT[] PROGMEM = "Just accept number";
const char D_INPUT_EMPTY[] PROGMEM = "Empty number";
const char D_ADD_EXTENSION_NAME_SSID[] PROGMEM = "this ssid name?";


// ===== SETTINGS ===== //
const char S_SETTINGS[] PROGMEM = "settings";
const char S_BEACONCHANNEL[] PROGMEM = "beaconchannel";
const char S_FORCEPACKETS[] PROGMEM = "forcepackets";
const char S_AUTOSAVE[] PROGMEM = "autosave";
const char S_LANG[] PROGMEM = "lang";
const char S_SERIALINTERFACE[] PROGMEM = "serial";
const char S_DISPLAYINTERFACE[] PROGMEM = "display";
const char S_WEBINTERFACE[] PROGMEM = "web/interface";
const char S_AUTOSAVETIME[] PROGMEM = "autosavetime";
const char S_DEAUTHSPERTARGET[] PROGMEM = "deauthspertarget";
const char S_CHTIME[] PROGMEM = "Chtime";
const char S_DEAUTHREASON[] PROGMEM = "deauthReason";
const char S_MACST[] PROGMEM = "macSt";
const char S_MACAP[] PROGMEM = "macAP";
const char S_RANDOMTX[] PROGMEM = "randomTX";
const char S_ATTACKTIMEOUT[] PROGMEM = "Attacktimeout";
const char S_PROBESPERSSID[] PROGMEM = "ProbesPerSSID";
const char S_PROBESPERSSID1[] PROGMEM = "ProbesSSID";
const char S_BEACONINTERVAL[] PROGMEM = "BeaconInterval";
const char S_CHANNEL[] PROGMEM = "Channel";
const char S_CHANNEL1[] PROGMEM = "ChannelTime";
const char S_CHANNEL2[] PROGMEM = "the ChannelTime?";
const char S_CAPTIVEPORTAL[] PROGMEM = "captivePortal";
const char S_HIDDEN[] PROGMEM = "hidden";
const char S_PASSWORD[] PROGMEM = "password";
const char S_NON_PASSWORD[] PROGMEM = "nonepassword";
const char S_SSID[] PROGMEM = "Do you want to edit";
const char S_MAC[] PROGMEM = "mac";
const char S_MIN_DEAUTHS[] PROGMEM = "MinDeauths";
const char S_DISPLAY_TIMEOUT[] PROGMEM = "DisplayTimeout";
const char S_DISPLAY_TIMEOUT1[] PROGMEM = "the DisplayTimeout";
const char S_SERIAL_ECHO[] PROGMEM = "serialEcho";
const char S_TYPE_CAPTIVE_PORTAL[] PROGMEM = "captiveType";
const char S_WEB_SPIFFS[] PROGMEM = "webSpiffs";

const char S_RANDOM[] PROGMEM = "random";
const char S_SSID_DEFAULT[] PROGMEM = "244v234";
const char AP_HIDDEN[] PROGMEM = "*HIDDEN*";
const char AP_WEP[] PROGMEM = "WEP";
const char AP_WPA[] PROGMEM = "WPA";
const char AP_WPA2[] PROGMEM = "WPA2";
const char AP_AUTO[] PROGMEM = "WPA*";


// ===== ATTACKS ===== //
const char A_TIMEOUT[] PROGMEM = "Timeout - ";
const char A_STATUS[] PROGMEM = "[Pkt/s] All: %+4u | Deauths: %+3u/%-3u | "
                                "Beacons: %+3u/%-3u | Probes: %+3u/%-3u\r\n";

// ===== SSIDs ===== //
const char SS_TABLE_HEADER[] PROGMEM = "ID Enc. SSID";
const char SS_JSON_SSIDS[] PROGMEM = "ssids";
const char SS_JSON_RANDOM[] PROGMEM = "random";
const char SS_RANDOM_INFO[] PROGMEM =
    "Generating new SSIDs... Type \"disable random\" to stop the random mode";

// ===== SCAN ==== //

const char SC_OUTPUT_A[] PROGMEM =
    "Scanning WiFi [%+2u%%]: %+3u packets/s | %+2u devices | %+2u deauths\r\n";
const char SC_OUTPUT_B[] PROGMEM =
    "Scanning WiFi: %+3u packets/s | %+2u devices | %+2u deauths\r\n";
const char SC_JSON_APS[] PROGMEM = "aps";
const char SC_JSON_STATIONS[] PROGMEM = "stations";
const char SC_JSON_NAMES[] PROGMEM = "names";
const char SC_MODE_OFF[] PROGMEM = "-";
const char SC_MODE_AP[] PROGMEM = "APs";
const char SC_MODE_ST[] PROGMEM = "STs";
const char SC_MODE_ALL[] PROGMEM = "AP+ST";

// ===== FUNCTIONS ===== //
const char F_LINE[] PROGMEM = "[%d] ";
const char F_TMP[] PROGMEM = "/tmp";
const char F_COPY[] PROGMEM = "_copy";

// ===== WIFI ===== //
const char W_STOPPED_AP[] PROGMEM = "Stopped Access Point";
const char W_AP_REQUEST[] PROGMEM = "[AP] request: ";
const char W_AP[] PROGMEM = "AP";
const char W_STATION[] PROGMEM = "Station";
const char W_MODE_OFF[] PROGMEM = "OFF";
const char W_MODE_AP[] PROGMEM = "AP";
const char W_MODE_ST[] PROGMEM = "STATION";
const char W_OK[] PROGMEM = " OK";
const char W_NOT_FOUND[] PROGMEM = " NOT FOUND";
const char W_BAD_ARGS[] PROGMEM = "BAD ARGS";
const char W_BAD_PATH[] PROGMEM = "BAD PATH";
const char W_FILE_NOT_FOUND[] PROGMEM = "ERROR 404 File Not Found";
const char W_STARTED_AP[] PROGMEM = "Started AP";
#endif // ifndef language_h
