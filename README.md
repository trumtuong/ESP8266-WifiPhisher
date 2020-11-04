<p align="center">
  👍<a href="https://facebook.com/244v234">Facebook</a>
| 🐦 <a href="https://twitter.com/244v234">Twitter</a>
| 📺 <a href="https://www.youtube.com/channel/UC5yaB0VU_u4sY-DiE0BGuSw/featured?view_as=subscriber">YouTube</a>
| 🌍 <a href="https://wifiphisher.tk">wifiphisher.tk</a><br>
<br></p>

## ESP8266 WifiPhisher
This software allows you to easily perform a variety of actions to test 802.11 wireless networks by using an inexpensive ESP8266 WiFi SoC (System On A Chip).
## Main feature
- The deauthentication attack, is used to disconnect devices from their WiFi network.	
- It is capable of performing a social engineering attack to get a secret WPA / WPA2 password
## Video demo
- [Youtube](https://youtu.be/9W9xHPFy9rc)
## [Flashing the firmware bin file](https://www.hackster.io/234v244/esp8266-wifiphisher-4ed3d3)
## Compiling using Arduino IDE
First you have to install and open the Arduino IDE.
- Add the required [libraries](https://drive.google.com/drive/folders/1QKlt-UVW6BszD1YrvQ47xcYCMHJUfeuS?usp=sharing)
- In Arduino go to File -> Preferences add both URLs in Additional Boards Manager URLs https://raw.githubusercontent.com/SpacehuhnTech/arduino/main/package_spacehuhn_index.json
- Go to Tools -> Board -> Boards Manager, search "deauther" and install Deauther ESP8266 Boards
- Select your board at Tools -> Board and be sure it is at Deauther ESP8266 Boards (and not at ESP8266 Modules)!
- Download the source code for this project from the releases page. You can also clone the project to get the latest changes, but you will also get the latest bugs ;)
- Extract the whole .zip file, and open WifiPhisher.ino with Arduino.
- Check your upload settings and press upload!
[Libraries](https://drive.google.com/drive/folders/1QKlt-UVW6BszD1YrvQ47xcYCMHJUfeuS?usp=sharing)
## Disclaimer
Usage of Wifiphisher for attacking infrastructures without prior mutual consistency can be considered as an illegal activity. It is the final user's responsibility to obey all applicable local, state and federal laws. Authors assume no liability and are not responsible for any misuse or damage caused by this program.
## This source code is built on [Deauther](https://github.com/SpacehuhnTech/esp8266_deauther)
