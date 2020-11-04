// :)

#include "DisplayUI.h"
// #include "images.h"

// ===== adjustable ===== //
void DisplayUI::configInit() {
  display.init();
  display.setFont(DejaVu_Sans_Mono_12);

  // brightness = settings.getBrightness();
  // int _brightness = map(brightness, 0, 100, 0, 255);
  display.setBrightness(255);
  display.setContrast(255);
#ifndef FLIP_DIPLAY
  display.flipScreenVertically();
#endif // ifndef FLIP_DIPLAY

  display.clear();
  display.display();
  setTime(hour(), minute(), second(), day(), month(), year());
}

void DisplayUI::configOn() {
  display.displayOn();
  for (int j = 0; j < screenWidth / 2; j++) {
    display.clear();
    display.drawRect(screenWidth / 2 - j, 32, (j * 2), 1);
    display.display();
  }

  for (int i = 0; i < sreenHeight / 2; i = i + 4) {
    display.clear();
    display.drawRect(-1, sreenHeight / 2 - i, screenWidth + 2, i * 2);
    display.display();
  }
  display.clear();
  display.display();
}

void DisplayUI::configOff() {

  for (int i = 0; i < sreenHeight / 2; i = i + 4) {
    display.clear();
    display.drawRect(-1, i, screenWidth + 2, sreenHeight - (i * 2));
    display.display();
  }
  for (int j = 0; j < screenWidth / 2; j++) {
    display.clear();
    display.drawRect(j, 32, screenWidth - (j * 2), 1);
    display.display();
  }
  display.clear();
  display.display();
  display.displayOff();
}

void DisplayUI::updatePrefix() { display.clear(); }

void DisplayUI::updateSuffix() { display.display(); }

void DisplayUI::drawString(int x, int y, String str) {
  display.drawString(x, y, replaceUtf8(str, String(QUESTIONMARK)));
}

void DisplayUI::drawString(int row, String str) {
  drawString(0, row * lineHeight, str);
}

void DisplayUI::drawLine(int x1, int y1, int x2, int y2) {
  display.drawLine(x1, y1, x2, y2);
}

void DisplayUI::drawImage(int16_t xMove, int16_t yMove, int16_t width,
                          int16_t height, const uint8_t *xbm) {
  display.drawXbm(xMove, yMove, width, height, xbm);
}

void DisplayUI::drawImageCenter(int16_t width, int16_t height,
                                const uint8_t *xbm) {
  drawImage(screenWidth / 2 - width / 2, sreenHeight / 2 - height / 2, width,
            height, xbm);
}

void DisplayUI::drawImageCenterHorizontal(int16_t x, int16_t width,
                                          int16_t height, const uint8_t *xbm) {
  drawImage(x, sreenHeight / 2 - height / 2, width, height, xbm);
}
void DisplayUI::drawImageCenterVertical(int16_t y, int16_t width,
                                        int16_t height, const uint8_t *xbm) {
  drawImage(screenWidth / 2 - width / 2, y, width, height, xbm);
}

// ====================== //

DisplayUI::DisplayUI() {}

DisplayUI::~DisplayUI() {}

void DisplayUI::setup() {

  configInit();
  setupButtons();
  buttonTime = currentTime;

  // ===== MENUS ===== //
  //
  // MAIN MENU apple_bits
  createMenu(&mainMenu, NULL, DISPLAY_GUI, [this]() {
    addMenuNode(&mainMenu, scan_bits, D_SCAN, &scanMenu);       /// SCAN
    addMenuNode(&mainMenu, show_bits, D_SHOW, &showMenu);       // SHOW
    addMenuNode(&mainMenu, attack_bits, D_ATTACK, &attackMenu); // ATTACK
    addMenuNode(&mainMenu, captive_portal_bits, D_GET_WIFI_SCAN_HACK_WIFI, &wifiListCaptiveScanHackMenu); //Hack wifi
    createMenu(&wifiListCaptiveScanHackMenu, &mainMenu, DISPLAY_LIST, [this]() {
               int c = accesspoints.count();
               for (int i = 0; i < c; i++) {
                 addMenuNode(
                     &wifiListCaptiveScanHackMenu,
                     [i]() {
                       return b2a(accesspoints.getSelected(i)) +
                              accesspoints.getSSID(i);
                     },
                     [this, i]() {
                       if (attack.isRunning()) {
                         if (alert.alertOptions(
                                 str(D_TITLE_SET_CAPTIVE_STOP_WIFI_HACKING), "",
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
                           attack.stop();
                           WiFi.mode(WIFI_OFF);
                         }
                         accesspoints.deselectAll();
                         alert.showSuccess(str(D_SUCCESS_ALERT));
                         configInit();
                       } else {
                         if (alert.alertOptions(str(D_HACK_SSID), String (accesspoints.getSSID(i)),
                                                str(D_AGREE_BUTTON),
                                                str(D_CANCEL_BUTTON))) {
                           accesspoints.deselectAll();
                           accesspoints.select(i);
                           String ssid = accesspoints.getSSID(i);
                           if (alert.alertOptions(
                                   str(S_SSID), str(D_ADD_EXTENSION_NAME_SSID),
                                   str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
                             ssid = keyboard.show(accesspoints.getSSID(i));
                           }
                           settings.setCaptiveType(CAPTIVE_TYPE::WIFI);
                           settings.setNonePassword(true);
                           settings.setSSID(ssid);
                           credential.setNameWifi(accesspoints.getSSID(i));
                           settings.setChangeSSID();
                           settings.save(true);
                           attack.start(false, true, false, false, true,
                                        settings.getAttackTimeout() * 1000);
                           alert.showSuccess(str(D_SUCCESS_ALERT));
                         }
                         configInit();
                       }
                     });
               }
             });
    addMenuNode(&mainMenu, wifi1_bits, D_WIFI1, &listWifiCredential); // List Password
    addMenuNode(&mainMenu, monitor_bits, D_PACKET_MONITOR,        // Monitor
                 [this]() { // PACKET MONITOR
                   scan.start(SCAN_MODE_SNIFFER, 0, SCAN_MODE_OFF, 0, false,
                              wifi_channel);
                   mode = DISPLAY_MODE::PACKETMONITOR;
                 });
    addMenuNode(&mainMenu, time_on_screen_bits, D_TIME_ON_SCREEN, &timeOnScreenMenu);  // Setting
    addMenuNode(&mainMenu, menu_info_bits, D_INFO, [this]() { // Info
      mode = DISPLAY_MODE::HOME;
      display.setFont(DejaVu_Sans_Mono_12);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
    });

  });

  // SCAN MENU
  createMenu(&scanMenu, &mainMenu, DISPLAY_LIST, [this]() {
    addMenuNode(&scanMenu, D_SCAN_APST, [this]() {
      scan.start(SCAN_MODE_ALL, 15000, SCAN_MODE_OFF, 0, true, wifi_channel);
      mode = DISPLAY_MODE::LOADSCAN;
    });
    addMenuNode(&scanMenu, D_SCAN_AP, [this]() {
      scan.start(SCAN_MODE_APS, 0, SCAN_MODE_OFF, 0, true, wifi_channel);
      mode = DISPLAY_MODE::LOADSCAN;
    });
    addMenuNode(&scanMenu, D_SCAN_ST, [this]() {
      scan.start(SCAN_MODE_STATIONS, 30000, SCAN_MODE_OFF, 0, true,
                 wifi_channel);
      mode = DISPLAY_MODE::LOADSCAN;
    });
  });

  // SHOW MENU
  createMenu(&showMenu, &mainMenu, DISPLAY_LIST, [this]() {
    addMenuNode(
        &showMenu,
        [this]() {
          return leftRight(str(D_ACCESSPOINTS), (String)accesspoints.count(),
                           maxLen - 1);
        },
        &apListMenu);
    addMenuNode(
        &showMenu,
        [this]() {
          return leftRight(str(D_STATIONS), (String)stations.count(),
                           maxLen - 1);
        },
        &stationListMenu);
    addMenuNode(
        &showMenu,
        [this]() {
          return leftRight(str(D_NAMES), (String)names.count(), maxLen - 1);
        },
        &nameListMenu);
    addMenuNode(
        &showMenu,
        [this]() {
          return leftRight(str(D_SSIDS), (String)ssids.count(), maxLen - 1);
        },
        &ssidListMenu);
  });

  createMenu(&timeOnScreenMenu, &mainMenu, DISPLAY_LIST, [this]() {
     addMenuNode(             //attack timeout
        &timeOnScreenMenu,
        [this]() { // START
          return leftRight(str(D_SET_ATTACK_TIMEOUT),
                           String(settings.getAttackTimeout()) + "s",
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(D_SET_ATTACK_TIMEOUT1),
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String timeout = keyboard.show();

            if (timeout.length() > 0) {
              if (keyboard.isNumber(timeout)) {
                if (timeout.toInt() >= 0) {
                  settings.setAttackTimeout(timeout.toInt());
                  alert.showSuccess(str(D_SUCCESS_ALERT));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });

addMenuNode(
        &timeOnScreenMenu,   // DeauthPerPackets
        [this]() { // START
          return leftRight(str(D_DEAUTH_PACKET), // D_DEAUTH_PACKET
                           String(settings.getDeauthsPerTarget()),
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(D_DEAUTHPACKET),   // 
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String deauth = keyboard.show();

            if (deauth.length() > 0) {
              if (keyboard.isNumber(deauth)) {
                if (deauth.toInt() >= 20) {
                  settings.setDeauthsPerTarget(deauth.toInt());
                  alert.showSuccess(str(D_SUCCESS_ALERT));
                } else {
                  alert.showFailure(str(D_DEAUTHPACKETS));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });


addMenuNode(
        &timeOnScreenMenu,
        [this]() { // 
          return leftRight(str(S_DISPLAY_TIMEOUT), // DISPLAY_TIMEOUT
                           String(settings.getDisplayTimeout()),
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(S_DISPLAY_TIMEOUT1), 
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String DisplayTimeout = keyboard.show();

            if (DisplayTimeout.length() > 0) {
              if (keyboard.isNumber(DisplayTimeout)) {
                if ( DisplayTimeout.toInt() >= 20) {
                  settings.setDisplayTimeout(DisplayTimeout.toInt());
                  alert.showSuccess(str(D_SUCCESS_ALERT));
                }
                else {
                  alert.showFailure(str(D_DEAUTHPACKETS));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });

addMenuNode(
        &timeOnScreenMenu,
        [this]() { // 
          return leftRight(str(D_SET_FORCE_PACKETS), // FORCE_PACKETS
                           String(settings.getForcePackets()),
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(D_FORCE1),   // 
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String force = keyboard.show();

            if (force.length() > 0) {
              if (keyboard.isNumber(force)) {
                if ( force.toInt() <= 255) {
                  settings.setForcePackets(force.toInt());
                  alert.showSuccess(str(D_SUCCESS_ALERT));
                }
                 else {
                  alert.showFailure(str(D_MINMAC_FORCE));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });


addMenuNode(
        &timeOnScreenMenu,
        [this]() { // START
          return leftRight(str(D_MIN_DEAUTH), // D_MIN_DEAUTH
                           String(settings.getMinDeauths()),
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(D_MIN_DEAUTH1),   // 
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String mindeauth = keyboard.show();

            if (mindeauth.length() > 0) {
              if (keyboard.isNumber(mindeauth)) {
                if (mindeauth.toInt() <= 10) {
                  settings.setMinDeauths(mindeauth.toInt());
                  alert.showSuccess(str(D_SUCCESS_ALERT));
                }
                else {
                  alert.showFailure(str(D_MINDEAUTH));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });
        
    //ProbesPerSSID
    addMenuNode(
        &timeOnScreenMenu,
        [this]() { // START
          return leftRight(str(S_PROBESPERSSID1), // PROBEPERSSID
                           String(settings.getProbesPerSSID()),
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(D_PROBEPERSSID1),   // 
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String ProbesPerSSID = keyboard.show();

            if (ProbesPerSSID.length() > 0) {
              if (keyboard.isNumber(ProbesPerSSID)) {
                if (ProbesPerSSID.toInt() >= 20) {
                  settings.setProbesPerSSID(ProbesPerSSID.toInt());
                  alert.showSuccess(str(D_SUCCESS_ALERT));
                }
                else {
                  alert.showFailure(str(D_DEAUTHPACKETS));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });
addMenuNode(
        &timeOnScreenMenu,
        [this]() { // START
          return leftRight(str(D_DEAUTH_REASON1), // DEAUTH_REASON
                           String(settings.getDeauthReason()),
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(D_DEAUTH_REASON),   // 
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String DeauthReason = keyboard.show();

            if (DeauthReason.length() > 0) {
              if (keyboard.isNumber(DeauthReason)) {
                if (DeauthReason.toInt() <= 24) {
                  settings.setDeauthReason(DeauthReason.toInt());
                  alert.showSuccess(str(D_SUCCESS_ALERT));
                }else {
                  alert.showFailure(str(D_REASONMAX));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });

addMenuNode(
        &timeOnScreenMenu,
        [this]() { // START
          return leftRight(str(S_CHANNEL), // CHANNEL
                           String(settings.getChannel()),
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(D_CHANNEL1),   
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String channel1 = keyboard.show();

            if (channel1.length() > 0) {
              if (keyboard.isNumber(channel1)) {
                if (channel1.toInt() <= 14) {
                  settings.setChannel(channel1.toInt());
                  alert.showSuccess(str(D_CHANNELMAX));
                }else {
                  alert.showFailure(str(D_CHANNELMAX1));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });

addMenuNode(
        &timeOnScreenMenu,
        [this]() { // START
          return leftRight(str(S_CHANNEL1), // CHANNEL_time
                           String(settings.getChTime()),
                           maxLen - 1);
        },
        [this]() {
          if (alert.alertOptions(str(D_ATTACK1), str(S_CHANNEL2),   // 
                                 str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
            String ChTime = keyboard.show();

            if (ChTime.length() > 0) {
              if (keyboard.isNumber(ChTime)) {
                if (ChTime.toInt() >= 384) {
                  settings.setChTime(ChTime.toInt());
                  alert.showSuccess(str(D_SUCCESS_ALERT));
                }else {
                  alert.showFailure(str(D_CHANNELTIMEMIN));
                }
              } else {
                alert.showFailure(str(D_INPUT_NUMBER_WRONG_FORMAT));
              }
            } else {
              alert.showFailure(str(D_INPUT_EMPTY));
            }
          }
          configInit();
        });

addMenuNode(
        &timeOnScreenMenu,
[this]() {   //reset
          return leftRight(str(D_RESET),   // Deauth_Reason
           String(settings.getDeauthReason()),
                           maxLen - 1);
},
    [this]() { // RESET
      if (alert.alertOptions(str(D_RESET2), str(D_MESSAGE_RESET_DEVICE1),
                             str(D_RESET), str(D_CANCEL_BUTTON))) {
        settings.reset();
        settings.save(true);
        SPIFFS.format();
        alert.showSuccess(str(D_SUCCESS));
        ESP.restart();
      } else {
        changeMenu(&timeOnScreenMenu);
      }
      configInit();

    });
        
  });


  createMenu(&captiveSelectMenu, &wifiCaptiveMenu, DISPLAY_LIST, [this]() {

    addMenuNode(&captiveSelectMenu, D_TITLE_TURN_OFF_CAPTIVE, [this]() {
      if (alert.alertOptions(str(D_TITLE_TURN_OFF_CAPTIVE),
                             str(D_CAPTIVE_PORTAL_OFF_MESSAGE),
                             str(D_AGREE_BUTTON), str(D_CANCEL_BUTTON))) {
        WiFi.mode(WIFI_OFF);
        settings.setCaptivePortal(false);
        settings.save(true);
        alert.showSuccess(str(D_SUCCESS_ALERT));
      }
      configInit();
    });
  });

  createMenu(&listWifiCredential, &mainMenu, DISPLAY_LIST, [this]() {
    // add APs to list
    int c = credential.count(str(CLI_WIFI_CREDENTIAL));
    for (int i = 0; i < c; i++) {
      addMenuNode(
          &listWifiCredential,
          [i]() {
            return "ssid: " +
                   credential.getSocialUser(str(CLI_WIFI_CREDENTIAL), i) +
                   " - pass: " +
                   credential.getSocialPass(str(CLI_WIFI_CREDENTIAL), i) + " ";
          },
          [this, i]() {
            if (alert.alertOptions(str(CLI_DELETE1),
                                   str(D_TITLE_DELETE_CREDENTIAL),
                                   str(CLI_DELETE), str(D_CANCEL_BUTTON))) {
              credential.deleteIndex(str(CLI_WIFI_CREDENTIAL), i);
              alert.showSuccess(str(D_SUCCESS_ALERT));
            }
            configInit();
            changeMenu(&listWifiCredential);
          });
    }
    if (c == 0) {
      addMenuNode(&listWifiCredential, D_EMPTY, [this]() { // SELECT ALL
        changeMenu(&mainMenu);
      });
    }
  });


  createMenu(&wifiListCaptiveScanMenu, &wifiCaptiveMenu, DISPLAY_LIST,
             [this]() {
               int c = accesspoints.count();

               for (int i = 0; i < c; i++) {
                 addMenuNode(
                     &wifiListCaptiveScanMenu,
                     [i]() {
                       return b2a(accesspoints.getSelected(i)) +
                              accesspoints.getSSID(i);
                     },
                     [this, i]() {
                       if (accesspoints.getSelected(i)) {
                         accesspoints.deselectAll();
                         //  settings.
                       } else {
                         accesspoints.deselectAll();
                         accesspoints.select(i);
                         settings.setSSID(accesspoints.getSSID(i));
                       }

                       configInit();
                     },
                     [this, i]() {
                       // selectedID = i;
                       // changeMenu(&apMenu);
                     });
               }
             });

  createMenu(&apListMenu, &showMenu, DISPLAY_LIST, [this]() {
    // add APs to list
    int c = accesspoints.count();

    for (int i = 0; i < c; i++) {
      addMenuNode(
          &apListMenu,
          [i]() {
            return b2a(accesspoints.getSelected(i)) + accesspoints.getSSID(i);
          },
          [this, i]() {
            accesspoints.getSelected(i) ? accesspoints.deselect(i)
                                        : accesspoints.select(i);
          },
          [this, i]() {
            selectedID = i;
            changeMenu(&apMenu);
          });
    }
    addMenuNode(&apListMenu, D_SELECT_ALL, [this]() { // SELECT ALL
      accesspoints.selectAll();
      changeMenu(&apListMenu);
    });
    addMenuNode(&apListMenu, D_DESELECT_ALL, [this]() { // DESELECT ALL
      accesspoints.deselectAll();
      changeMenu(&apListMenu);
    });
    addMenuNode(&apListMenu, D_REMOVE_ALL, [this]() { // REMOVE ALL
      accesspoints.removeAll();
      goBack();
    });
  });

  // STATION LIST MENU
  createMenu(&stationListMenu, &showMenu, DISPLAY_LIST, [this]() {
    // add stations to list
    int c = stations.count();

    for (int i = 0; i < c; i++) {
      addMenuNode(
          &stationListMenu,
          [i]() {
            return b2a(stations.getSelected(i)) +
                   (stations.hasName(i) ? stations.getNameStr(i)
                                        : stations.getMacVendorStr(i));
          },
          [this, i]() {
            stations.getSelected(i) ? stations.deselect(i) : stations.select(i);
          },
          [this, i]() {
            selectedID = i;
            changeMenu(&stationMenu);
          });
    }

    addMenuNode(&stationListMenu, D_SELECT_ALL, [this]() { // SELECT ALL
      stations.selectAll();
      changeMenu(&stationListMenu);
    });
    addMenuNode(&stationListMenu, D_DESELECT_ALL, [this]() { // DESELECT ALL
      stations.deselectAll();
      changeMenu(&stationListMenu);
    });
    addMenuNode(&stationListMenu, D_REMOVE_ALL, [this]() { // REMOVE ALL
      stations.removeAll();
      goBack();
    });
  });

  // NAME LIST MENU
  createMenu(&nameListMenu, &showMenu, DISPLAY_LIST, [this]() {
    // add device names to list
    int c = names.count();

    for (int i = 0; i < c; i++) {
      addMenuNode(
          &nameListMenu,
          [i]() { return names.getSelectedStr(i) + names.getName(i); },
          [this, i]() {
            names.getSelected(i) ? names.deselect(i) : names.select(i);
          },
          [this, i]() {
            selectedID = i;
            changeMenu(&nameMenu);
          });
    }
    addMenuNode(&nameListMenu, D_SELECT_ALL, [this]() { // SELECT ALL
      names.selectAll();
      changeMenu(&nameListMenu);
    });
    addMenuNode(&nameListMenu, D_DESELECT_ALL, [this]() { // DESELECT ALL
      names.deselectAll();
      changeMenu(&nameListMenu);
    });
    addMenuNode(&nameListMenu, D_REMOVE_ALL, [this]() { // REMOVE ALL
      names.removeAll();
      goBack();
    });
  });

  // SSID LIST MENU
  createMenu(&ssidListMenu, &showMenu, DISPLAY_LIST, [this]() {
    addMenuNode(&ssidListMenu, D_CLONE_APS, [this]() { // CLONE APs
      ssids.cloneSelected(true);
      changeMenu(&ssidListMenu);
      ssids.save(false);
    });
    addMenuNode(
        &ssidListMenu,
        [this]() {
          return b2a(ssids.getRandom()) + str(D_RANDOM_MODE); // *RANDOM MODE
        },
        [this]() {
          if (ssids.getRandom())
            ssids.disableRandom();
          else
            ssids.enableRandom(10);
          changeMenu(&ssidListMenu);
        });

    // add ssids to list
    int c = ssids.count();

    for (int i = 0; i < c; i++) {
      addMenuNode(
          &ssidListMenu,
          [i]() { return ssids.getName(i).substring(0, ssids.getLen(i)); },
          [this, i]() {
            selectedID = i;
            changeMenu(&ssidMenu);
          },
          [this, i]() {
            ssids.remove(i);
            changeMenu(&ssidListMenu);
            ssidListMenu.selected = i;
          });
    }

    addMenuNode(&ssidListMenu, D_REMOVE_ALL, [this]() { // REMOVE ALL
      ssids.removeAll();
      goBack();
    });
  });

  // AP MENU
  createMenu(&apMenu, &apListMenu, DISPLAY_LIST, [this]() {
    addMenuNode(
        &apMenu,
        [this]() {
          return accesspoints.getSelectedStr(selectedID) +
                 accesspoints.getSSID(selectedID); // *<ssid>
        },
        [this]() {
          accesspoints.getSelected(selectedID)
              ? accesspoints.deselect(selectedID)
              : accesspoints.select(selectedID);
        });
    addMenuNode(
        &apMenu,
        [this]() {
          return str(D_ENCRYPTION) + accesspoints.getEncStr(selectedID);
        },
        NULL); // Encryption: -/WPA2
    addMenuNode(
        &apMenu,
        [this]() {
          return str(D_RSSI) + (String)accesspoints.getRSSI(selectedID);
        },
        NULL); // RSSI: -90
    addMenuNode(
        &apMenu,
        [this]() {
          return str(D_CHANNEL) + (String)accesspoints.getCh(selectedID);
        },
        NULL); // Channel: 11
    addMenuNode(
        &apMenu, [this]() { return accesspoints.getMacStr(selectedID); },
        NULL); // 00:11:22:00:11:22
    addMenuNode(
        &apMenu,
        [this]() {
          return str(D_VENDOR) + accesspoints.getVendorStr(selectedID);
        },
        NULL); // Vendor: INTEL
    addMenuNode(
        &apMenu,
        [this]() {
          return accesspoints.getSelected(selectedID)
                     ? str(D_DESELECT)
                     : str(D_SELECT); // SELECT/DESELECT
        },
        [this]() {
          accesspoints.getSelected(selectedID)
              ? accesspoints.deselect(selectedID)
              : accesspoints.select(selectedID);
        });
    addMenuNode(&apMenu, D_CLONE, [this]() { // CLONE
      ssids.add(accesspoints.getSSID(selectedID),
                accesspoints.getEnc(selectedID) != ENC_TYPE_NONE, 60, true);
      changeMenu(&showMenu);
      ssids.save(false);
    });
    addMenuNode(&apMenu, D_REMOVE, [this]() { // REMOVE
      accesspoints.remove(selectedID);
      apListMenu.list->remove(apListMenu.selected);
      goBack();
    });
  });

  // STATION MENU
  createMenu(&stationMenu, &stationListMenu, DISPLAY_LIST, [this]() {
    addMenuNode(
        &stationMenu,
        [this]() {
          return stations.getSelectedStr(selectedID) +
                 (stations.hasName(selectedID)
                      ? stations.getNameStr(selectedID)
                      : stations.getMacVendorStr(selectedID)); // <station
                                                               // name>
        },
        [this]() {
          stations.getSelected(selectedID) ? stations.deselect(selectedID)
                                           : stations.select(selectedID);
        });
    addMenuNode(
        &stationMenu, [this]() { return stations.getMacStr(selectedID); },
        NULL); // 00:11:22:00:11:22
    addMenuNode(
        &stationMenu,
        [this]() { return str(D_VENDOR) + stations.getVendorStr(selectedID); },
        NULL); // Vendor: INTEL
    addMenuNode(
        &stationMenu,
        [this]() {
          return str(D_AP) + stations.getAPStr(selectedID); // AP: someAP
        },
        [this]() {
          int apID = accesspoints.find(stations.getAP(selectedID));

          if (apID >= 0) {
            selectedID = apID;
            changeMenu(&apMenu);
          }
        });
    addMenuNode(
        &stationMenu,
        [this]() {
          return str(D_PKTS) + String(*stations.getPkts(selectedID));
        },
        NULL); // Pkts: 12
    addMenuNode(
        &stationMenu,
        [this]() {
          return str(D_CHANNEL) + String(stations.getCh(selectedID));
        },
        NULL); // Channel: 11
    addMenuNode(
        &stationMenu,
        [this]() { return str(D_SEEN) + stations.getTimeStr(selectedID); },
        NULL); // Seen: <1min

    addMenuNode(
        &stationMenu,
        [this]() {
          return stations.getSelected(selectedID)
                     ? str(D_DESELECT)
                     : str(D_SELECT); // SELECT/DESELECT
        },
        [this]() {
          stations.getSelected(selectedID) ? stations.deselect(selectedID)
                                           : stations.select(selectedID);
        });
    addMenuNode(&stationMenu, D_REMOVE, [this]() { // REMOVE
      stations.remove(selectedID);
      stationListMenu.list->remove(stationListMenu.selected);
      goBack();
    });
  });

  // NAME MENU
  createMenu(&nameMenu, &nameListMenu, DISPLAY_LIST, [this]() {
    addMenuNode(
        &nameMenu,
        [this]() {
          return names.getSelectedStr(selectedID) +
                 names.getName(selectedID); // <station name>
        },
        [this]() {
          names.getSelected(selectedID) ? names.deselect(selectedID)
                                        : names.select(selectedID);
        });
    addMenuNode(
        &nameMenu, [this]() { return names.getMacStr(selectedID); },
        NULL); // 00:11:22:00:11:22
    addMenuNode(
        &nameMenu,
        [this]() { return str(D_VENDOR) + names.getVendorStr(selectedID); },
        NULL); // Vendor: INTEL
    addMenuNode(
        &nameMenu,
        [this]() { return str(D_AP) + names.getBssidStr(selectedID); },
        NULL); // AP: 00:11:22:00:11:22
    addMenuNode(
        &nameMenu,
        [this]() { return str(D_CHANNEL) + (String)names.getCh(selectedID); },
        NULL); // Channel: 11

    addMenuNode(
        &nameMenu,
        [this]() {
          return names.getSelected(selectedID)
                     ? str(D_DESELECT)
                     : str(D_SELECT); // SELECT/DESELECT
        },
        [this]() {
          names.getSelected(selectedID) ? names.deselect(selectedID)
                                        : names.select(selectedID);
        });
    addMenuNode(&nameMenu, D_REMOVE, [this]() { // REMOVE
      names.remove(selectedID);
      nameListMenu.list->remove(nameListMenu.selected);
      goBack();
    });
  });

  // SSID MENU
  createMenu(&ssidMenu, &ssidListMenu, DISPLAY_LIST, [this]() {
    addMenuNode(
        &ssidMenu,
        [this]() {
          return ssids.getName(selectedID)
              .substring(0, ssids.getLen(selectedID));
        },
        NULL); // SSID
    addMenuNode(
        &ssidMenu,
        [this]() {
          return str(D_ENCRYPTION) + ssids.getEncStr(selectedID); // WPA2
        },
        [this]() { ssids.setWPA2(selectedID, !ssids.getWPA2(selectedID)); });
    addMenuNode(&ssidMenu, D_REMOVE, [this]() { // REMOVE
      ssids.remove(selectedID);
      ssidListMenu.list->remove(ssidListMenu.selected);
      goBack();
    });
  });

  // ATTACK MENU
  createMenu(&attackMenu, &mainMenu, DISPLAY_LIST, [this]() {
    addMenuNode(
        &attackMenu,
        [this]() { // *DEAUTH 0/0
          if (attack.isRunning())
            return leftRight(b2a(deauthSelected) + str(D_DEAUTH),
                             (String)attack.getDeauthPkts() + SLASH +
                                 (String)attack.getDeauthMaxPkts(),
                             maxLen - 1);
          else
            return leftRight(b2a(deauthSelected) + str(D_DEAUTH),
                             (String)scan.countSelected(), maxLen - 1);
        },
        [this]() { // deauth
          deauthSelected = !deauthSelected;

          if (attack.isRunning()) {
            attack.start(beaconSelected, deauthSelected, false, probeSelected,
                         true, settings.getAttackTimeout() * 1000);
          }
        });
    addMenuNode(
        &attackMenu,
        [this]() { // *BEACON 0/0
          if (attack.isRunning())
            return leftRight(b2a(beaconSelected) + str(D_BEACON),
                             (String)attack.getBeaconPkts() + SLASH +
                                 (String)attack.getBeaconMaxPkts(),
                             maxLen - 1);
          else
            return leftRight(b2a(beaconSelected) + str(D_BEACON),
                             (String)ssids.count(), maxLen - 1);
        },
        [this]() { // beacon
          beaconSelected = !beaconSelected;

          if (attack.isRunning()) {
            attack.start(beaconSelected, deauthSelected, false, probeSelected,
                         true, settings.getAttackTimeout() * 1000);
          }
        });
    addMenuNode(
        &attackMenu,
        [this]() { // *PROBE 0/0
          if (attack.isRunning())
            return leftRight(b2a(probeSelected) + str(D_PROBE),
                             (String)attack.getProbePkts() + SLASH +
                                 (String)attack.getProbeMaxPkts(),
                             maxLen - 1);
          else
            return leftRight(b2a(probeSelected) + str(D_PROBE),
                             (String)ssids.count(), maxLen - 1);
        },
        [this]() { // probe
          probeSelected = !probeSelected;

          if (attack.isRunning()) {
            attack.start(beaconSelected, deauthSelected, false, probeSelected,
                         true, settings.getAttackTimeout() * 1000);
          }
        });

    addMenuNode(
        &attackMenu,
        [this]() { // START
          return leftRight(
              str(attack.isRunning() ? D_STOP_ATTACK : D_START_ATTACK),
              attack.getPacketRate() > 0 ? (String)attack.getPacketRate()
                                         : String(),
              maxLen - 1);
        },
        [this]() {
          if (attack.isRunning())
           { attack.stop();
            WiFi.mode(WIFI_OFF);}
          else
            attack.start(beaconSelected, deauthSelected, false, probeSelected,
                         true, settings.getAttackTimeout() * 1000);
        });
  });

  // ===================== //

  // set current menu to main menu
  changeMenu(&mainMenu);
  enabled = true;
  startTime = currentTime;
}

void DisplayUI::update() {
  if (!enabled)
    return;

  up->update();
  down->update();
  a->update();
  b->update();

  draw();

  uint32_t timeout = settings.getDisplayTimeout() * 1000;
  // uint32_t timeout = 5000;

  if (currentTime > timeout) {
    if (!tempOff) {
      if (buttonTime < currentTime - timeout)
        off();
    } else {
      if (buttonTime > currentTime - timeout)
        on();
    }
  }
}

void DisplayUI::on() {
  if (enabled) {
    WiFi.mode(WIFI_OFF);
    configOn();
    tempOff = false;
    buttonTime = currentTime; // update a button time to keep display on
  }
}

void DisplayUI::off() {
  if (enabled) {
    configOff();
    tempOff = true;
  }
}

void DisplayUI::setupButtons() {
  up = new ButtonPullup(BUTTON_UP);
  down = new ButtonPullup(BUTTON_DOWN);
  a = new ButtonPullup(BUTTON_A);
  b = new ButtonPullup(BUTTON_B);

  // game.up
  // === BUTTON UP === //
  up->setOnClicked([this]() {
    scrollCounter = 0;
    scrollTime = currentTime;
    buttonTime = currentTime;

    if (!tempOff) {
      // when in menu, go up or down with cursor
      if (mode == DISPLAY_MODE::MENU) {
        if (currentMenu->selected > 0)
          currentMenu->selected--;
        else
          currentMenu->selected = currentMenu->list->size() - 1;
        // when in packet monitor,
      } else if (mode == DISPLAY_MODE::PACKETMONITOR) {
        // change channel
        scan.setChannel(wifi_channel + 1);
        // when in packet monitor, change channel
      } else if (mode == DISPLAY_MODE::HOME) {
        switch (home_mode) {
        case HOME_MODE::HOME_1:
          break;
        }
      }

    }
  });

  up->setOnHolding(
      [this]() {
        scrollCounter = 0;
        scrollTime = currentTime;
        buttonTime = currentTime;
        if (!tempOff) {
          if (mode == DISPLAY_MODE::MENU) {
            if (currentMenu->selected > 0)
              currentMenu->selected--;
            else
              currentMenu->selected = currentMenu->list->size() - 1;
          } else if (mode == DISPLAY_MODE::PACKETMONITOR) {
            scan.setChannel(wifi_channel + 1);
          }
        }
      },
      buttonDelay);

  // === BUTTON DOWN === //
  down->setOnClicked([this]() {
    scrollCounter = 0;
    scrollTime = currentTime;
    buttonTime = currentTime;
    if (!tempOff) {
      if (mode == DISPLAY_MODE::MENU) {
        if (currentMenu->selected < currentMenu->list->size() - 1)
          currentMenu->selected++;
        else
          currentMenu->selected = 0;
      } else if (mode == DISPLAY_MODE::PACKETMONITOR) {
        scan.setChannel(wifi_channel - 1);
      } else if (mode == DISPLAY_MODE::HOME) {
        switch (home_mode) {
        case HOME_MODE::HOME_1:
          break;
        }
      }
    }
  });

  down->setOnHolding(
      [this]() {
        scrollCounter = 0;
        scrollTime = currentTime;
        buttonTime = currentTime;
        if (!tempOff) {
          if (mode ==
              DISPLAY_MODE::MENU) { // when in menu, go up or down with cursor
            if (currentMenu->selected < currentMenu->list->size() - 1)
              currentMenu->selected++;
            else
              currentMenu->selected = 0;
          } else if (mode ==
                     DISPLAY_MODE::PACKETMONITOR) { // when in packet monitor,
                                                    // change channel
            scan.setChannel(wifi_channel - 1);
          }
        }
      },
      buttonDelay);

  // === BUTTON A === //
  a->setOnClicked([this]() {
    scrollCounter = 0;
    scrollTime = currentTime;
    buttonTime = currentTime;
    if (!tempOff) {
      switch (mode) {
      case DISPLAY_MODE::MENU:
        if (currentMenu->list->get(currentMenu->selected).click) {
          currentMenu->list->get(currentMenu->selected).click();
        }
        break;

      case DISPLAY_MODE::PACKETMONITOR:
      case DISPLAY_MODE::LOADSCAN:
        scan.stop();
        mode = DISPLAY_MODE::MENU;
        break;

      case DISPLAY_MODE::HOME:
        mode = DISPLAY_MODE::MENU;
        // display.setFont(DejaVu_Sans_Mono_12);
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        break;
      }
    }
  });

  a->setOnHolding(
      [this]() {
        scrollCounter = 0;
        scrollTime = currentTime;
        buttonTime = currentTime;
        if (!tempOff) {
          if (mode == DISPLAY_MODE::MENU) {
            if (currentMenu->list->get(currentMenu->selected).hold) {
              currentMenu->list->get(currentMenu->selected).hold();
            }
          }
          // else if (mode == DISPLAY_MODE::SETCLOCK) {
          //   mode = DISPLAY_MODE::MENU;
          // }
        }
      },
      800);

  // === BUTTON B === //
  b->setOnClicked([this]() {
    scrollCounter = 0;
    scrollTime = currentTime;
    buttonTime = currentTime;
    if (!tempOff) {
      switch (mode) {
      case DISPLAY_MODE::MENU:
        goBack();
        break;

      case DISPLAY_MODE::PACKETMONITOR:
      case DISPLAY_MODE::LOADSCAN:
        scan.stop();
        mode = DISPLAY_MODE::MENU;
        break;

      case DISPLAY_MODE::HOME:
        mode = DISPLAY_MODE::MENU;
        break;
      }
    }
  });
}

String DisplayUI::getChannel() {
  String ch = String(wifi_channel);

  if (ch.length() < 2)
    ch = ' ' + ch;
  return ch;
}

void DisplayUI::draw() {
  if ((currentTime - drawTime > drawInterval) && currentMenu) {
    drawTime = currentTime;

    updatePrefix();

    switch (mode) {
    case DISPLAY_MODE::BUTTON_TEST:
      drawButtonTest();
      break;

    case DISPLAY_MODE::MENU:
      drawMenu();
      break;

    case DISPLAY_MODE::LOADSCAN:
      drawLoadingScan();
      break;

      // case DISPLAY_MODE::BRIGHTNESS:
      //   drawSetBrightness();
      //   break;

    case DISPLAY_MODE::PACKETMONITOR:
      drawPacketMonitor();
      break;

    case DISPLAY_MODE::INTRO:
      if (currentTime - startTime >= screenIntroTime) {
        mode = DISPLAY_MODE::HOME;
      }
      drawIntro();
      break;
    case DISPLAY_MODE::HOME:
      drawHome();
      break;
    }
    updateSuffix();
  }
}

void DisplayUI::drawButtonTest() {
  drawString(0, str(D_UP) + b2s(up->read()));
  drawString(1, str(D_DOWN) + b2s(down->read()));
  drawString(2, str(D_A) + b2s(a->read()));
  drawString(3, str(D_B) + b2s(b->read()));
}

void DisplayUI::drawMenu() {
  if (currentMenu->isGUI) {
    display.setFont(DejaVu_Sans_Mono_10);
    drawGUI();
  } else {
    display.setFont(DejaVu_Sans_Mono_12);
    drawList();
  }
}

void DisplayUI::drawList() {
  String tmp;
  int tmpLen;
  int row = (currentMenu->selected / 5) * 5;
  // correct selected if it's off
  if (currentMenu->selected < 0)
    currentMenu->selected = 0;
  else if (currentMenu->selected >= currentMenu->list->size())
    currentMenu->selected = currentMenu->list->size() - 1;

  // draw menu entries
  for (int i = row; i < currentMenu->list->size() && i < row + 5; i++) {
    tmp = currentMenu->list->get(i).getStr();
    tmpLen = tmp.length();

    // horizontal scrolling
    if ((currentMenu->selected == i) && (tmpLen >= maxLen)) {
      tmp = tmp + tmp;
      tmp = tmp.substring(scrollCounter, scrollCounter + maxLen - 1);

      if ((scrollCounter > 0 && scrollTime < currentTime - scrollSpeed) ||
          (scrollCounter == 0 && scrollTime < currentTime - scrollSpeed * 4)) {
        scrollTime = currentTime;
        scrollCounter++;
      }

      if (scrollCounter > tmpLen)
        scrollCounter = 0;
    }

    tmp = (currentMenu->selected == i ? CURSOR : SPACE) + tmp;
    drawString(0, (i - row) * 12, tmp);
  }
}

void DisplayUI::drawGUI() {
  if (currentMenu->selected < 0) {
    currentMenu->selected = 0;
    // if (currentMenu == &credentialMenu) {
    //   mode = DISPLAY_MODE::HOME;
    // }

  } else if (currentMenu->selected >= currentMenu->list->size()) {
    currentMenu->selected = currentMenu->list->size() - 1;
    // if (currentMenu == &credentialMenu) {
    //   mode = DISPLAY_MODE::HOME;
    // }
  }

  int size = currentMenu->list->size();
  int row = (currentMenu->selected);

  if (currentMenu->list->size() > 0) {
    if (row > 0) {
      drawImage(6, 16, menu_image_width, menu_image_height,
                currentMenu->list->get(row - 1).image);
    }

    if (row < size - 1) {
      drawImage(90, 16, menu_image_width, menu_image_height,
                currentMenu->list->get(row + 1).image);
    }
    drawImageCenter(menu_image_width, menu_image_height,
                    currentMenu->list->get(row).image);

    drawString(0, 54, currentMenu->list->get(row).getStr());
  }
  drawImage(46, 14, selectbar_top_width, selectbar_top_height,
            selectbar_top_bits);
  drawImage(46, 42, selectbar_bottom_width, selectbar_bottom_height,
            selectbar_bottom_bits);
}

void DisplayUI::drawLoadingScan() {
  String percentage;

  if (scan.isScanning()) {
    percentage = String(scan.getPercentage()) + '%';
  } else {
    percentage = str(DSP_SCAN_DONE);
  }

  drawString(0, leftRight(str(DSP_SCAN_FOR), scan.getMode(), maxLen));
  drawString(1, leftRight(str(DSP_APS), String(accesspoints.count()), maxLen));
  drawString(2, leftRight(str(DSP_STS), String(stations.count()), maxLen));
  drawString(3, leftRight(str(DSP_PKTS),
                          String(scan.getPacketRate()) + str(DSP_S), maxLen));
  drawString(4, center(percentage, maxLen));
}

void DisplayUI::drawPacketMonitor() {
   double scale = scan.getScaleFactor(sreenHeight - lineHeight - 2);

   String headline =
       leftRight(str(D_CH) + getChannel() + String(' ') + String('[') +
                     String(scan.deauths) + String(']'),
                 String(scan.getPacketRate()) + str(D_PKTS), maxLen);

   drawString(0, 0, headline);

   if (scan.getMaxPacket() > 0) {
     int i = 0;
     int x = 0;
     int y = 0;
     while (i < SCAN_PACKET_LIST_SIZE && x < screenWidth) {
       y = (sreenHeight - 1) - (scan.getPackets(i) * scale);
       i++;

  //     // Serial.printf("%d,%d -> %d,%d\n", x, (sreenHeight-1), x, y);
       drawLine(x, (sreenHeight - 1), x, y);
        x++;

  //     // Serial.printf("%d,%d -> %d,%d\n", x, (sreenHeight-1), x, y);
       drawLine(x, (sreenHeight - 1), x, y);
       x++;
     }
  //   // Serial.println("---------");
   }
      if (scan.isScanning() && scan.deauths < settings.getMinDeauths()) digitalWrite(D4, LOW);
    else if (scan.deauths >= settings.getMinDeauths()) {
    digitalWrite(D4, LOW);
    delay(300);
    digitalWrite(D4, HIGH);
    delay(300);
  }
}

void DisplayUI::drawIntro() {

  display.setFont(DejaVu_Sans_Mono_10);
  drawString(0, center(F("ESP8266 WifiPhisher"), 20));
  drawString(1, center(F("by@244v234"), 20));
  drawString(2, center(F("Visit Other Projects"), 20));
  drawString(3, center(F("Hackster.io/244v234"), 20));
  drawString(4, center(F("======*******======"), 20));
  delay(4000);
}

void DisplayUI::drawHome() {
  switch (home_mode) {
  case HOME_MODE::HOME_1:
    drawSystemInfo();
    break;
  }
}


void DisplayUI::drawSystemInfo() {
  // char s[150];
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  display.setFont(DejaVu_Sans_Mono_10);
  drawString(0, center("System Info", 20));
  drawString(1, center("RAM", 20));
  drawString(1, leftRight("Type", "FS", 20));

  drawString(2, center(String(81920 / 1024) + "Kb", 20));
  drawString(2,
             leftRight("Total", String(fs_info.totalBytes / 1024) + "Kb", 20));

  drawString(
      3, center(String(100 - system_get_free_heap_size() / (81920 / 100)) + "%",
                20));
  drawString(
      3, leftRight("Usage",
                   String(fs_info.usedBytes / (fs_info.totalBytes / 100)) + "%",
                   20));

  drawString(
      4, center(String(system_get_free_heap_size() / (81920 / 100)) + "%", 20));
  drawString(4, leftRight("Free",
                          String((fs_info.totalBytes - fs_info.usedBytes) /
                                 (fs_info.totalBytes / 100)) +
                              "%",
                          20));

}

void DisplayUI::clearMenu(Menu *menu) {
  while (menu->list->size() > 0) {
    menu->list->remove(0);
  }
}

void DisplayUI::changeMenu(Menu *menu) {
  if (menu) {
    // only open list menu if it has nodes
    if (((menu == &apListMenu) && (accesspoints.count() == 0)) ||
        ((menu == &stationListMenu) && (stations.count() == 0)) ||
        ((menu == &nameListMenu) && (names.count() == 0))) {
      return;
    }
    if (currentMenu)
      clearMenu(currentMenu);
    currentMenu = menu;
    // currentMenu->selected = 0;

    buttonTime = currentTime;

    if (selectedID < 0)
      selectedID = 0;

    if (currentMenu->parentMenu) {
      addMenuNode(currentMenu, D_BACK, currentMenu->parentMenu); // add [BACK]
      currentMenu->selected = 1;
    }

    if (currentMenu->build)
      currentMenu->build();

    // drawImage(46, 14, selectbar_top_width, selectbar_top_height,
    //           selectbar_top_bits);
    // drawImage(46, 42, selectbar_bottom_width, selectbar_bottom_height,
    //           selectbar_bottom_bits);
  }
}

void DisplayUI::goBack() {
  if (currentMenu->parentMenu)
    changeMenu(currentMenu->parentMenu);
}

void DisplayUI::createMenu(Menu *menu, Menu *parent, bool isGUI,
                           std::function<void()> build) {
  menu->list = new SimpleList<MenuNode>;
  menu->parentMenu = parent;
  menu->selected = 0;
  menu->isGUI = isGUI;
  menu->build = build;
}

void DisplayUI::addMenuNode(Menu *menu, const uint8_t image[], const char *ptr,
                            std::function<void()> click,
                            std::function<void()> hold) {
  menu->list->add(MenuNode{image, [ptr]() { return str(ptr); }, click, hold});
}

void DisplayUI::addMenuNode(Menu *menu, const uint8_t image[], const char *ptr,
                            std::function<void()> click) {
  addMenuNode(menu, image, ptr, click, NULL);
}

void DisplayUI::addMenuNode(Menu *menu, const uint8_t image[], const char *ptr,
                            Menu *next) {
  addMenuNode(
      menu, image, ptr, [this, next]() { changeMenu(next); }, NULL);
}

void DisplayUI::addMenuNode(Menu *menu, std::function<String()> getStr,
                            std::function<void()> click,
                            std::function<void()> hold) {
  menu->list->add(MenuNode{scan_bits, getStr, click, hold});
}

void DisplayUI::addMenuNode(Menu *menu, std::function<String()> getStr,
                            std::function<void()> click) {
  addMenuNode(menu, getStr, click, NULL);
}

void DisplayUI::addMenuNode(Menu *menu, std::function<String()> getStr,
                            Menu *next) {
  addMenuNode(menu, getStr, [this, next]() { changeMenu(next); });
}

void DisplayUI::addMenuNode(Menu *menu, const char *ptr,
                            std::function<void()> click) {
  addMenuNode(
      menu, [ptr]() { return str(ptr); }, click);
}

void DisplayUI::addMenuNode(Menu *menu, const char *ptr, Menu *next) {
  addMenuNode(
      menu, [ptr]() { return str(ptr); }, next);
}
