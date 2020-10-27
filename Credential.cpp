#include "Credential.h"
// #include "ArduinoJson.h"

Credential::Credential() {
  listWifi = new SimpleList<SocialAccount>;
};

Credential::~Credential(){};

void Credential::init() {
  buf = String();
  readFile(FILE_PATH, buf);
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(buf);
  // listFacebook
  if (isExist(root, str(CLI_WIFI_CREDENTIAL))) {

    if (isExist(root, str(CLI_WIFI_CREDENTIAL))) {
      JsonArray &values = root[str(CLI_WIFI_CREDENTIAL)];
      for (unsigned int i = 0; i < values.size(); i++) {
        add(str(CLI_WIFI_CREDENTIAL), values[i][0], values[i][1]);
      }
    }
  } else {
    deleteAll();
  }
}

int Credential::count(String key) {
  if (key == str(CLI_WIFI_CREDENTIAL)) {
    return listWifi->size();
  }
  return 0;
}

void Credential::add(String key, String user, String pass) {
  if (key == str(CLI_WIFI_CREDENTIAL)) {
    listWifi->add(SocialAccount{user, pass});
  }
}

String Credential::get() {
  buf = String();
  readFile(FILE_PATH, buf);
  return buf;
}

void Credential::toSerial() {
  buf = String();
  readFile(FILE_PATH, buf);
  buf = String();
  readFile(FILE_WIFI_PATH, buf);
  buf = String();
}

bool Credential::isExist(JsonObject &root, String key) {
  return root.containsKey(key);
}

void Credential::save(String key, String name, String number) {

  buf = String();
  readFile(FILE_PATH, buf);
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(buf);

  if (!isExist(root, key)) {
    JsonArray &values = root.createNestedArray(key);
    JsonArray &array = jsonBuffer.createArray();
    array.add(name);
    array.add(number);
    values.add(array);
  } else {
    JsonArray &values = root[key];
    JsonArray &array = jsonBuffer.createArray();
    array.add(name);
    array.add(number);
    values.add(array);
  }

  String data;
  root.printTo(data);
  if (!writeFile(FILE_PATH, data)) {
    prnt(F_ERROR_SAVING);
    prntln(FILE_PATH);
  }
  add(key, name, number);
  buf = String();
  toSerial();
}

String Credential::getSocialUser(String key, int id) {
  if (key == str(CLI_WIFI_CREDENTIAL)) {
    return listWifi->get(id).user;
  }

  return "";
}
String Credential::getSocialPass(String key, int id) {
  if (key == str(CLI_WIFI_CREDENTIAL)) {
    return listWifi->get(id).pass;
  }
  return "";
}

String Credential::getPhone(int id) { return listPhone->get(id).phone; }
String Credential::getName(int id) { return listPhone->get(id).name; }

void Credential::deleteIndex(String key, int id) {
  buf = String();
  readFile(FILE_PATH, buf);
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(buf);
  JsonArray &values = root[key];
  values.removeAt(id);
  String data;
  root.printTo(data);

  if (key == str(CLI_WIFI_CREDENTIAL)) {
    listWifi->remove(id);
  }
  if (!writeFile(FILE_PATH, data)) {
    prnt(F_ERROR_SAVING);
    prntln(FILE_PATH);
  }
  buf = String();
  toSerial();
}

void Credential::setNameWifi(String name) {

  if (!writeFile(FILE_WIFI_PATH, name)) {
    prnt(F_ERROR_SAVING);
    prntln(FILE_WIFI_PATH);
  }
  buf = String();
}

void Credential::deleteAll() {
  String data =
      "{\"wifi\":[]}";
  if (!writeFile(FILE_PATH, data)) {
    prnt(F_ERROR_SAVING);
    prntln(FILE_PATH);
  }
}
