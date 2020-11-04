#include "CLI.h"

CLI::CLI() {
  list = new SimpleList<String>;
  queue = new SimpleList<String>;
}

CLI::~CLI() {}

void CLI::load() {
  String defaultValue = str(CLI_DEFAULT_AUTOSTART);
  checkFile(execPath, defaultValue);
  execFile(execPath);
}

void CLI::load(String filepath) {
  execPath = filepath;
  load();
}

void CLI::enable() {
  enabled = true;
}

void CLI::disable() {
  enabled = true;
}

void CLI::update() {
  // when serial available, read input
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    exec(input);
  }

  // when queue is not empty, delay is off and no scan is active, run it
  else if ((queue->size() > 0) && !delayed && !scan.isScanning() &&
           !attack.isRunning()) {
    String s = queue->shift();
    exec(s);
  }
}

void CLI::stop() {
  queue->clear();
}

void CLI::enableDelay(uint32_t delayTime) {
  delayed = true;
  this->delayTime = delayTime;
  delayStartTime = millis();
}

void CLI::exec(String input) {
  // quick exit when input is empty
  if (input.length() == 0)
    return;

  // check delay
  if (delayed && (millis() - delayStartTime > delayTime)) {
    delayed = false;
  }

  // when delay is on, add it to queue, else run it
  if (delayed) {
    queue->add(input);
  } else {
    runLine(input);
  }
}

void CLI::execFile(String path) {
  String input;

  if (readFile(path, input)) {
    String tmpLine;
    char tmpChar;

    input += '\n';

    while (!queue->isEmpty()) {
      input += queue->shift();
      input += '\n';
    }

    for (int i = 0; i < input.length(); i++) {
      tmpChar = input.charAt(i);

      if (tmpChar == '\n') {
        queue->add(tmpLine);
        tmpLine = String();
      } else {
        tmpLine += tmpChar;
      }
    }

    queue->add(tmpLine);
  }
}

void CLI::parameterError(String parameter) {
  prnt(CLI_ERROR_PARAMETER);
  prnt(parameter);
  prntln(DOUBLEQUOTES);
}

bool CLI::isInt(String str) {
  if (eqls(str, STR_TRUE) || eqls(str, STR_FALSE))
    return true;

  for (uint32_t i = 0; i < str.length(); i++)
    if (!isDigit(str.charAt(i)))
      return false;

  return true;
}

int CLI::toInt(String str) {
  if (eqls(str, STR_TRUE))
    return 1;
  else if (eqls(str, STR_FALSE))
    return 0;
  else
    return str.toInt();
}

uint32_t CLI::getTime(String time) {
  int value = time.toInt();

  if (value < 0)
    value = -value;

  if (time.substring(time.length() - 1).equalsIgnoreCase(String(S)))
    value *= 1000;
  else if (time.substring(time.length() - 3).equalsIgnoreCase(str(STR_MIN)) ||
           (time.charAt(time.length() - 1) == M))
    value *= 60000;
  return value;
}

bool CLI::eqlsCMD(int i, const char *keyword) {
  return eqls(list->get(i).c_str(), keyword);
}

void CLI::runLine(String input) {
  String tmp;

  for (int i = 0; i < input.length(); i++) {
    // when 2 semicolons in a row without a backslash escaping the first
    if ((input.charAt(i) == SEMICOLON) && (input.charAt(i + 1) == SEMICOLON) &&
        (input.charAt(i - 1) != BACKSLASH)) {
      runCommand(tmp);
      tmp = String();
      i++;
    } else {
      tmp += input.charAt(i);
    }
  }

  tmp.replace(BACKSLASH + SEMICOLON + SEMICOLON, SEMICOLON + SEMICOLON);

  if (tmp.length() > 0)
    runCommand(tmp);
}

void CLI::runCommand(String input) {
  input.replace(String(NEWLINE), String());
  input.replace(String(CARRIAGERETURN), String());

  list->clear();

  // parse/split input in list
  String tmp;
  bool withinQuotes = false;
  bool escaped = false;
  char c;

  for (uint32_t i = 0; i < input.length() && i < 512; i++) {
    c = input.charAt(i);

    // when char is an unescaped
    if (!escaped && (c == BACKSLASH)) {
      escaped = true;
    }

    // (when char is a unescaped space AND it's not within quotes) OR char is \r
    // or \n
    else if (((c == SPACE) && !escaped && !withinQuotes) ||
             (c == CARRIAGERETURN) || (c == NEWLINE)) {
      // when tmp string isn't empty, add it to the list
      if (tmp.length() > 0) {
        list->add(tmp);
        tmp = String(); // reset tmp string
      }
    }

    // when char is an unescaped "
    else if ((c == DOUBLEQUOTES) && !escaped) {
      // update wheter or not the following chars are within quotes or not
      withinQuotes = !withinQuotes;

      if ((tmp.length() == 0) && !withinQuotes)
        tmp += SPACE; // when exiting quotes and tmp string is empty, add
                      // a space
    }

    // add character to tmp string
    else {
      tmp += c;
      escaped = false;
    }
  }

  // add string if something is left from the loop above
  if (tmp.length() > 0)
    list->add(tmp);

  // stop when input is empty/invalid
  if (list->size() == 0)
    return;

  // print comments
  if (list->get(0) == str(CLI_COMMENT)) {
    prntln(input);
    return;
  }

  if (settings.getSerialEcho()) {
    // print command
    prnt(CLI_INPUT_PREFIX);
    prntln(input);
  }

  if (list->size() == 0)
    return;

  // ===== SCAN ===== //
  // scan [<mode>] [-t <time>] [-c <continue-time>] [-ch <channel>]
  else if (eqlsCMD(0, CLI_SCAN)) {
    uint8_t scanMode = SCAN_MODE_ALL;
    uint8_t nextmode = SCAN_MODE_OFF;
    uint8_t channel = wifi_channel;
    bool channelHop = true;
    uint32_t time = 15000;
    uint32_t continueTime = 10000;

    for (int i = 1; i < list->size(); i++) {
      if (eqlsCMD(i, CLI_AP))
        scanMode = SCAN_MODE_APS;
      else if (eqlsCMD(i, CLI_STATION))
        scanMode = SCAN_MODE_STATIONS;
      else if (eqlsCMD(i, CLI_ALL))
        scanMode = SCAN_MODE_ALL;
      else if (eqlsCMD(i, CLI_WIFI))
        scanMode = SCAN_MODE_SNIFFER;
      else if (eqlsCMD(i, CLI_TIME)) {
        i++;
        time = getTime(list->get(i));
      } else if (eqlsCMD(i, CLI_CONTINUE)) {
        i++;
        nextmode = scanMode;
        continueTime = getTime(list->get(i));
      } else if (eqlsCMD(i, CLI_CHANNEL)) {
        i++;

        if (!eqlsCMD(i, CLI_ALL)) {
          channelHop = false;
          channel = list->get(i).toInt();
        }
      } else {
        parameterError(list->get(i));
      }
    }

    scan.start(scanMode, time, nextmode, continueTime, channelHop, channel);
  }


  // ===== (DE)SELECT ===== //
  // select [<type>] [<id>]
  // deselect [<type>] [<id>]
  else if (eqlsCMD(0, CLI_SELECT) || eqlsCMD(0, CLI_DESELECT)) {
    bool select = eqlsCMD(0, CLI_SELECT);
    int mode = 0; // aps = 0, stations = 1, names = 2
    int id = -1;  // -1 = all, -2 name string

    if ((list->size() == 1) || eqlsCMD(1, CLI_ALL)) {
      select ? scan.selectAll() : scan.deselectAll();
      return;
    }

    if ((list->size() == 2) || eqlsCMD(2, CLI_ALL))
      id = -1;
    else if (!isInt(list->get(2)))
      id = -2;
    else
      id = list->get(2).toInt();

    if (eqlsCMD(1, CLI_AP))
      mode = 0;
    else if (eqlsCMD(1, CLI_STATION))
      mode = 1;
    else if (eqlsCMD(1, CLI_NAME))
      mode = 2;
    else
      parameterError(list->get(1));

    if (id >= 0) {
      if (mode == 0)
        select ? accesspoints.select(id) : accesspoints.deselect(id);
      else if (mode == 1)
        select ? stations.select(id) : stations.deselect(id);
      else if (mode == 2)
        select ? names.select(id) : names.deselect(id);
    } else if (id == -1) {
      if (mode == 0)
        select ? accesspoints.selectAll() : accesspoints.deselectAll();
      else if (mode == 1)
        select ? stations.selectAll() : stations.deselectAll();
      else if (mode == 2)
        select ? names.selectAll() : names.deselectAll();
    } else if ((id == -2)) {
      String name = list->get(2);
      if (mode == 0)
        select ? accesspoints.select(name) : accesspoints.deselect(name);
      else if (mode == 1)
        select ? stations.select(name) : stations.deselect(name);
      else if (mode == 2)
        select ? names.select(name) : names.deselect(name);
    } else {
      parameterError(list->get(1) + SPACE + list->get(2));
    }
  }

  // ===== ADD ===== //
  else if ((list->size() >= 3) && eqlsCMD(0, CLI_ADD) && eqlsCMD(1, CLI_SSID)) {
    // add ssid -s [-f]
    if (eqlsCMD(2, CLI_SELECT)) {
      bool force = eqlsCMD(3, CLI_FORCE);
      ssids.cloneSelected(force);
    }

    // add ssid <ssid> [-wpa2] [-cl <clones>] [-f]
    // add ssid -ap <id> [-cl <clones>] [-f]
    else {
      String ssid = list->get(2);
      bool wpa2 = false;
      bool force = false;
      int clones = 1;
      int i = 3;

      if (eqlsCMD(2, CLI_AP)) {
        ssid = accesspoints.getSSID(list->get(3).toInt());
        wpa2 = accesspoints.getEncStr(list->get(3).toInt()) != " - ";
        i = 4;
      }

      while (i < list->size()) {
        if (eqlsCMD(i, CLI_WPA2))
          wpa2 = true;
        else if (eqlsCMD(i, CLI_FORCE))
          force = true;
        else if (eqlsCMD(i, CLI_CLONES)) {
          clones = list->get(i + 1).toInt();
          i++;
        } else
          parameterError(list->get(i));
        i++;
      }

      ssids.add(ssid, wpa2, clones, force);
    }
  }

  // add name <name> [-ap <id>] [-s] [-f]
  // add name <name> [-st <id>] [-s] [-f]
  // add name <name> [-m <mac>] [-ch <channel>] [-b <bssid>] [-s] [-f]
  else if ((list->size() >= 3) && eqlsCMD(0, CLI_ADD) && eqlsCMD(1, CLI_NAME)) {
    String name = list->get(2);
    String mac;
    uint8_t channel = wifi_channel;
    String bssid;
    bool selected = false;
    bool force = false;

    for (int i = 3; i < list->size(); i++) {
      if (eqlsCMD(i, CLI_MAC))
        mac = list->get(i + 1);
      else if (eqlsCMD(i, CLI_AP))
        mac = accesspoints.getMacStr(list->get(i + 1).toInt());
      else if (eqlsCMD(i, CLI_STATION)) {
        mac = stations.getMacStr(list->get(i + 1).toInt());
        bssid = stations.getAPMacStr(list->get(i + 1).toInt());
      } else if (eqlsCMD(i, CLI_CHANNEL))
        channel = (uint8_t)list->get(i + 1).toInt();
      else if (eqlsCMD(i, CLI_BSSID))
        bssid = list->get(i + 1);
      else if (eqlsCMD(i, CLI_SELECT)) {
        selected = true;
        i--;
      } else if (eqlsCMD(i, CLI_FORCE)) {
        force = true;
        i--;
      } else {
        parameterError(list->get(i));
        i--;
      }
      i++;
    }

    if (name.length() == 0);
    else if (mac.length() == 0);
    else
      names.add(mac, name, bssid, channel, selected, force);
  }

  // ===== SET NAME ==== //
  // set name <id> <newname>
  else if ((list->size() == 4) && eqlsCMD(0, CLI_SET) && eqlsCMD(1, CLI_NAME)) {
    names.setName(list->get(2).toInt(), list->get(3));
  }

  // ===== REPLACE ===== //
  // replace name <id> [-n <name>} [-m <mac>] [-ch <channel>] [-b <bssid>] [-s]
  else if ((list->size() >= 4) && eqlsCMD(0, CLI_REPLACE) &&
           eqlsCMD(1, CLI_NAME)) {
    int id = list->get(2).toInt();
    String name = names.getName(id);
    String mac = names.getMacStr(id);
    uint8_t channel = names.getCh(id);
    String bssid = names.getBssidStr(id);
    bool selected = names.getSelected(id);

    for (int i = 3; i < list->size(); i++) {
      if (eqlsCMD(i, CLI_NAME))
        name = list->get(i + 1);
      else if (eqlsCMD(i, CLI_MAC))
        mac = list->get(i + 1);
      else if (eqlsCMD(i, CLI_CHANNEL))
        channel = (uint8_t)list->get(i + 1).toInt();
      else if (eqlsCMD(i, CLI_BSSID))
        bssid = list->get(i + 1);
      else if (eqlsCMD(i, CLI_SELECT)) {
        selected = true;
        i--;
      } else {
        parameterError(list->get(i));
        i--;
      }
      i++;
    }

    names.replace(id, mac, name, bssid, channel, selected);
  }
  // replace ssid 1 -n "-wpa2"
  // replace ssid <id> [-n <name>} [-wpa2]
  else if ((list->size() >= 3) && eqlsCMD(0, CLI_REPLACE) &&
           eqlsCMD(1, CLI_SSID)) {
    int id = list->get(2).toInt();
    String name = ssids.getName(id);
    bool wpa2 = false;

    for (int i = 3; i < list->size(); i++) {
      if (eqlsCMD(i, CLI_NAME)) {
        name = list->get(i + 1);
        i++;
      } else if (eqlsCMD(i, CLI_WPA2)) {
        wpa2 = true;
      }
    }

    ssids.replace(id, name, wpa2);
  }

  // ===== REMOVE ===== //
  // remove <type> [-a]
  // remove <type> <id>
  else if ((list->size() >= 2) && eqlsCMD(0, CLI_REMOVE)) {
    if ((list->size() == 2) || (eqlsCMD(2, CLI_ALL))) {
      if (eqlsCMD(1, CLI_SSID))
        ssids.removeAll();
      else if (eqlsCMD(1, CLI_NAME))
        names.removeAll();
      else if (eqlsCMD(1, CLI_AP))
        accesspoints.removeAll();
      else if (eqlsCMD(1, CLI_STATION))
        stations.removeAll();
      else
        parameterError(list->get(1));
    } else {
      if (eqlsCMD(1, CLI_SSID))
        ssids.remove(list->get(2).toInt());
      else if (eqlsCMD(1, CLI_NAME))
        names.remove(list->get(2).toInt());
      else if (eqlsCMD(1, CLI_AP))
        accesspoints.remove(list->get(2).toInt());
      else if (eqlsCMD(1, CLI_STATION))
        stations.remove(list->get(2).toInt());
      else
        parameterError(list->get(1));
    }
  }

  // ===== RANDOM ===== //
  // enable random <interval>
  else if (eqlsCMD(0, CLI_ENABLE) && eqlsCMD(1, CLI_RANDOM) &&
           (list->size() == 3)) {
    ssids.enableRandom(getTime(list->get(2)));
  }

  // disable random
  else if (eqlsCMD(0, CLI_DISABLE) && eqlsCMD(1, CLI_RANDOM)) {
    ssids.disableRandom();
  }

  // ===== LOAD/SAVE ===== //
  // save [<type>] [<file>]
  // load [<type>] [<file>]
  else if ((eqlsCMD(0, CLI_LOAD) || eqlsCMD(0, CLI_SAVE)) &&
           (list->size() >= 1) && (list->size() <= 3)) {
    bool load = eqlsCMD(0, CLI_LOAD);

    if ((list->size() == 1) || eqlsCMD(1, CLI_ALL)) {
      load ? ssids.load() : ssids.save(false);
      load ? names.load() : names.save(false);
      load ? settings.load() : settings.save(false);

      if (!load)
        scan.save(false);
      return;
    }

    if (list->size() == 3) { // Todo: check if -f or filename
      if (eqlsCMD(1, CLI_SSID))
        load ? ssids.load(list->get(2)) : ssids.save(true, list->get(2));
      else if (eqlsCMD(1, CLI_NAME))
        load ? names.load(list->get(2)) : names.save(true, list->get(2));
      else if (eqlsCMD(1, CLI_SETTING))
        load ? settings.load(list->get(2)) : settings.save(true, list->get(2));
      else
        parameterError(list->get(1));
    } else {
      if (eqlsCMD(1, CLI_SSID))
        load ? ssids.load() : ssids.save(false);
      else if (eqlsCMD(1, CLI_NAME))
        load ? names.load() : names.save(false);
      else if (eqlsCMD(1, CLI_SETTING))
        load ? settings.load() : settings.save(false);
      else if ((eqlsCMD(1, CLI_SCAN) || eqlsCMD(1, CLI_AP) ||
                eqlsCMD(1, CLI_STATION)) &&
               !load)
        scan.save(false);
      else
        parameterError(list->get(1));
    }
  }

  // ===== ATTACK ===== //
  // attack [-b] [-d] [-da] [p] [-t <timeout>]
  // attack status [<on/off>]
  else if (eqlsCMD(0, CLI_ATTACK)) {
    if (eqlsCMD(1, CLI_STATUS)) {
      if (list->size() == 2) {
        attack.status();
      } else {
        if (eqlsCMD(2, CLI_ON))
          attack.enableOutput();
        else if (eqlsCMD(2, CLI_OFF))
          attack.disableOutput();
        else
          parameterError(list->get(2));
      }
      return;
    }

    bool beacon = false;
    bool deauth = false;
    bool deauthAll = false;
    bool probe = false;
    bool output = true;
    uint32_t timeout = settings.getAttackTimeout() * 1000;

    for (int i = 1; i < list->size(); i++) {
      if (eqlsCMD(i, CLI_BEACON))
        beacon = true;
      else if (eqlsCMD(i, CLI_DEAUTH))
        deauth = true;
      else if (eqlsCMD(i, CLI_DEAUTHALL))
        deauthAll = true;
      else if (eqlsCMD(i, CLI_PROBE))
        probe = true;
      else if (eqlsCMD(i, CLI_NOOUTPUT))
        output = false;
      else if (eqlsCMD(i, CLI_TIMEOUT)) {
        timeout = getTime(list->get(i + 1));
        i++;
      } else
        parameterError(list->get(i));
    }

    attack.start(beacon, deauth, deauthAll, probe, output, timeout);
  }

  // ===== GET/SET ===== //
  // get <setting>
  else if (eqlsCMD(0, CLI_GET) && (list->size() == 2)) {
    prntln(settings.get(list->get(1).c_str()));
  }

  // set <setting> <value>
  else if (eqlsCMD(0, CLI_SET) && (list->size() == 3)) {
    settings.set(list->get(1).c_str(), list->get(2));
  }

  // ===== STOP ===== //
  // stop [<mode>]
  else if (eqlsCMD(0, CLI_STOP)) {
    if ((list->size() >= 2) && !(eqlsCMD(1, CLI_ALL))) {
      for (int i = 1; i < list->size(); i++) {
        if (eqlsCMD(i, CLI_SCAN))
          scan.stop();
        else if (eqlsCMD(i, CLI_ATTACK))
          attack.stop();
        else if (eqlsCMD(i, CLI_SCRIPT))
          this->stop();
        else
          parameterError(list->get(i));
      }
    } else {
      scan.stop();
      attack.stop();
      this->stop();
    }
  }

  // ===== DELETE ==== //
  // delete <file> [<lineFrom>] [<lineTo>]
  else if ((list->size() >= 2) && eqlsCMD(0, CLI_DELETE)) {
    if (list->size() == 2) {
      // remove whole file
      if (removeFile(list->get(1))) {
        prntln(list->get(1));
      } else {
        prntln(list->get(1));
      }
    } else {
      // remove certain lines
      int beginLine = list->get(2).toInt();
      int endLine = list->size() == 4 ? list->get(3).toInt() : beginLine;

      if (removeLines(list->get(1), beginLine, endLine)) {
        prnt(beginLine);
        prnt(String(SPACE) + String(DASH) + String(SPACE));
        prnt(endLine);
        prntln(String(SPACE) + list->get(1));
      } else {
        prntln(list->get(1));
      }
    }
  }

  // ===== SEND ===== //
  // send deauth <apMac> <stMac> <rason> <channel>
  else if (eqlsCMD(0, CLI_SEND) && (list->size() == 6) &&
           eqlsCMD(1, CLI_DEAUTH)) {
    uint8_t apMac[6];
    uint8_t stMac[6];
    strToMac(list->get(2), apMac);
    strToMac(list->get(3), stMac);
    uint8_t reason = list->get(4).toInt();
    uint8_t channel = list->get(5).toInt();
    prnt(macToStr(apMac));
    prntln(macToStr(stMac));
    attack.deauthDevice(apMac, stMac, reason, channel);
  }

  // send beacon <mac> <ssid> <ch> [wpa2]
  else if (eqlsCMD(0, CLI_SEND) && (list->size() >= 5) &&
           eqlsCMD(1, CLI_BEACON)) {
    uint8_t mac[6];
    strToMac(list->get(2), mac);
    uint8_t channel = list->get(4).toInt();
    String ssid = list->get(3);

    for (int i = ssid.length(); i < 32; i++)
      ssid += SPACE;
    prnt(list->get(3));
    prntln(DOUBLEQUOTES);
    attack.sendBeacon(mac, ssid.c_str(), channel, eqlsCMD(5, CLI_WPA2));
  }

  // send probe <mac> <ssid> <ch>
  else if (eqlsCMD(0, CLI_SEND) && (list->size() == 5) &&
           eqlsCMD(1, CLI_PROBE)) {
    uint8_t mac[6];
    strToMac(list->get(2), mac);
    uint8_t channel = list->get(4).toInt();
    String ssid = list->get(3);

    for (int i = ssid.length(); i < 32; i++)
      ssid += SPACE;
    prnt(list->get(3));
    prntln(DOUBLEQUOTES);
    attack.sendProbe(mac, ssid.c_str(), channel);
  }

  // send custom <packet>
  else if (eqlsCMD(0, CLI_SEND) && eqlsCMD(1, CLI_CUSTOM)) {
    String packetStr = list->get(2);
    packetStr.replace(String(DOUBLEQUOTES), String());
    uint16_t counter = 0;
    uint16_t packetSize = packetStr.length() / 2;
    uint8_t packet[packetSize];

    for (int i = 0; i < packetSize; i++)
      packet[i] =
          strtoul((packetStr.substring(i * 2, i * 2 + 2)).c_str(), NULL, 16);

    if (attack.sendPacket(packet, packetSize, wifi_channel, 10)) {
      counter++;
    }
  }

  // ===== DELAY ===== //
  else if ((list->size() == 2) && eqlsCMD(0, CLI_DELAY)) {
    uint32_t endTime = currentTime + getTime(list->get(1));

    while (currentTime < endTime) {
      // ------- loop function ----- //
      currentTime = millis();

      wifiUpdate();    // manage access point
      scan.update();   // run scan
      attack.update(); // run attacks
      ssids.update();  // run random mode, if enabled

      // auto-save
      if (settings.getAutosave() &&
          (currentTime - autosaveTime > settings.getAutosaveTime())) {
        autosaveTime = currentTime;
        names.save(false);
        ssids.save(false);
        settings.save(false);
      }
      // ------- loop function end ----- //
      yield();
    }
  }

  // ===== DRAW ===== //
  else if (eqlsCMD(0, CLI_DRAW)) {
    int height = 25;
    int width = 2;

    if (list->size() >= 2)
      height = list->get(1).toInt();

    if (list->size() >= 3)
      width = list->get(2).toInt();
    double scale = scan.getScaleFactor(height);

    prnt(String(DASH) + String(DASH) + String(DASH) + String(DASH) +
         String(VERTICALBAR)); // ----|

    for (int j = 0; j < SCAN_PACKET_LIST_SIZE; j++) {
      for (int k = 0; k < width; k++)
        prnt(EQUALS);
    }
    prntln(VERTICALBAR);

    for (int i = height; i >= 0; i--) {
      char s[200];

      if (i == height)
        sprintf(s, str(CLI_DRAW_OUTPUT).c_str(),
                scan.getMaxPacket() > (uint32_t)height ? scan.getMaxPacket()
                                                       : (uint32_t)height);
      else if (i == height / 2)
        sprintf(s, str(CLI_DRAW_OUTPUT).c_str(),
                scan.getMaxPacket() > (uint32_t)height ? scan.getMaxPacket() / 2
                                                       : (uint32_t)height / 2);
      else if (i == 0)
        sprintf(s, str(CLI_DRAW_OUTPUT).c_str(), 0);
      else {
        s[0] = SPACE;
        s[1] = SPACE;
        s[2] = SPACE;
        s[3] = SPACE;
        s[4] = ENDOFLINE;
      }
      prnt(String(s));

      prnt(VERTICALBAR);

      for (int j = 0; j < SCAN_PACKET_LIST_SIZE; j++) {
        if (scan.getPackets(j) * scale > i) {
          for (int k = 0; k < width; k++)
            prnt(HASHSIGN);
        } else {
          for (int k = 0; k < width; k++)
            prnt(SPACE);
        }
      }
      prntln(VERTICALBAR);
    }

    prnt(String(DASH) + String(DASH) + String(DASH) + String(DASH) +
         String(VERTICALBAR)); // ----|

    for (int j = 0; j < SCAN_PACKET_LIST_SIZE; j++) {
      for (int k = 0; k < width; k++)
        prnt(EQUALS);
    }
    prntln(VERTICALBAR);

    prnt(String(SPACE) + String(SPACE) + String(SPACE) + String(SPACE) +
         String(VERTICALBAR));

    for (int j = 0; j < SCAN_PACKET_LIST_SIZE; j++) {
      char s[6];
      String helper = String(PERCENT) + DASH + (String)width + D;

      if (j == 0)
        sprintf(s, helper.c_str(), SCAN_PACKET_LIST_SIZE - 1);
      else if (j == SCAN_PACKET_LIST_SIZE / 2)
        sprintf(s, helper.c_str(), SCAN_PACKET_LIST_SIZE / 2);
      else if (j == SCAN_PACKET_LIST_SIZE - 1)
        sprintf(s, helper.c_str(), 0);
      else {
        int k;

        for (k = 0; k < width; k++)
          s[k] = SPACE;
        s[k] = ENDOFLINE;
      }
      prnt(s);
    }
    prntln(VERTICALBAR);
  }

  // ===== START/STOP AP ===== //
  // startap [-p <path][-s <ssid>] [-pswd <password>] [-ch <channel>] [-h] [-cp]
  else if (eqlsCMD(0, CLI_STARTAP)) {
    String path = String(F("/web"));
    String ssid = settings.getSSID();
    String password = settings.getPassword();
    int ch = wifi_channel;
      bool hidden = settings.getHidden();
    bool captivePortal = settings.getCaptivePortal();
    bool isNonePass = settings.getNonePassword();

    for (int i = 1; i < list->size(); i++) {
      if (eqlsCMD(i, CLI_PATH)) {
        i++;
        path = list->get(i);
      } else if (eqlsCMD(i, CLI_SSID)) {
        i++;
        ssid = list->get(i);
      } else if (eqlsCMD(i, CLI_PASSWORD)) {
        i++;
        password = list->get(i);
      } else if (eqlsCMD(i, CLI_CHANNEL)) {
        i++;
        ch = list->get(i).toInt();
        } else if (eqlsCMD(i, CLI_HIDDEN)) {
        hidden = true;
      } else if (eqlsCMD(i, CLI_CAPTIVEPORTAL)) {
        captivePortal = true;
      } else {
        parameterError(list->get(1));
      }
    }

    startAP(path, ssid, password, ch, hidden, captivePortal, isNonePass);
  }

  // stopap
  else if (eqlsCMD(0, CLI_STOPAP)) {
    stopAP();
  }


  // screen <on/off>
  else if (eqlsCMD(0, CLI_SCREEN) &&
           (eqlsCMD(1, CLI_ON) || eqlsCMD(1, CLI_OFF))) {
    if (eqlsCMD(1, CLI_ON)) {
      displayUI.on();
    } else if (eqlsCMD(1, CLI_OFF)) {
      displayUI.off();
    }
  }
  else if (eqlsCMD(0, CLI_SAVE_WIFI_CREDENTIAL)){
    bool isWifi = eqlsCMD(0, CLI_SAVE_WIFI_CREDENTIAL);
    String username = list->get(1);
    String password = list->get(2);
    if (isWifi) {
      credential.save(str(CLI_WIFI_CREDENTIAL), username, password);
    }
  }

  else if (eqlsCMD(0, CLI_CREDENTIAL_DELETE)) {
    bool isWifi = eqlsCMD(1, CLI_WIFI_CREDENTIAL);
    if (isWifi) {
      int index = list->get(2).toInt();
      credential.deleteIndex(str(CLI_WIFI_CREDENTIAL), index);
    } else {
      credential.deleteAll();
    }
  }
 else {
    prnt(input);
    // some debug stuff
 }
}
