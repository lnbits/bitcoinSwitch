# Status LED Addon for Bitcoin Switch

[![connected](images/ledaddon_green.png)](https://youtube.com/shorts/giTMPoKyzK4)[![connected](images/ledaddon_blue.png)](https://youtube.com/shorts/giTMPoKyzK4)

I wanted a status indicator, to "see" without the serial connection to 
check the status of a bitcoinSwitch project. [See the demo](https://youtube.com/shorts/N0I8sqNY8uo?feature=share).

## HowTo

- Maybe: rename the project directory to `bitcoinSwitch` for the Arduino IDE
- Add a Status LED like in this project [here](https://github.com/thespielplatz/hardware-playground/tree/main/MultiLED)
- I am using a LED which has one anode and three cathodes for the color, this means `LOW` is `ON` and `HIGH` is `OFF`.
- RED to pin#32, GREEN to pin#33 and BLUE to pin#25 with an  
- Follow the awesome install instructions: "_Complicated install instructions not using browser flashing_" further down  this page

### Status LED explanation

| State                                  | LED                               |
|----------------------------------------|-----------------------------------|
| Startup                                | Solid Red                         |
| Waiting, if config mode gets triggered | Blinking Red (2sec)               |
| Config Mode                            | 3x Blink Green/Red --> Solid Blue |
| Connecting 2 Wifi                      | Blinking Blue                     |
| Connected 2 Wifi                       | Solid Blue                        |
| WebSocket Connect & Ready for Payment  | Solid Green                       |
| WebSocket Disconnect                   | Solid Red                         |
| WebSocket Data for Payment received    | Solid Blue                        |

# bitcoinSwitch (orignal repo)

<img style="max-width:100%;" src="https://user-images.githubusercontent.com/33088785/166832680-600ed270-cbc9-4749-82f1-c1853b242329.png">

<img style="max-width:100%;" src="https://user-images.githubusercontent.com/33088785/166829474-a28ca2b7-dd3e-46d4-89d3-8a10bf1d3fad.png">

### Flash and configure from browser! https://lnbits.github.io/bitcoinswitch/installer/

> Once flashed, press GPIO4 in few seconds of ESP32 booting up to be able to config

   <table>
  <tr>
    <th><img src="https://user-images.githubusercontent.com/33088785/204107016-bc9473e0-2843-4873-af71-cd934e07f444.gif" alt="Snow" style="width:100%"></th>
    <th><img src="https://user-images.githubusercontent.com/33088785/204107029-cc4ad95b-b130-4b48-9091-86d7be7d4f16.gif" alt="Forest" style="width:100%"></th>
    <th><img src="https://user-images.githubusercontent.com/33088785/204107037-870571f8-b860-4019-93d4-bbdbeaf1091f.gif" alt="Mountains" style="width:100%"></th>
    <th><img src="https://user-images.githubusercontent.com/33088785/204107044-b8a7d94f-6908-40dd-bb82-974e08f077f4.gif" alt="Mountains" style="width:100%"></th>
  </tr>
</table> 

### 👉 An absolutely incredible ,<a href="https://ereignishorizont.xyz/bitcoinswitch/en/">bitcoinSwitch guide</a> by Axel 👈

## Compatible with any ESP32 microcontroller.

For use with LNURLDevice in LNbits.

### ✅ $5 worth parts/5min setup

### ✅ Websockets for blazingly fast turning on the things

### ✅ Web-installer/config for easy setup

### ✅ Support for MULTIPLE GPIOS/LNURLs!!!


> <a href="https://twitter.com/arcbtc/status/1585627498510831616">Video tutorial</a>

> Join us! <a href="https://t.me/makerbits">t.me/makerbits</a>

### Complicated install instructions not using browser flashing
- Install <a href="https://www.arduino.cc/en/software">Arduino IDE 1.8.19</a>
- Install ESP32 boards, using <a href="https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-boards-manager">boards manager</a>
![image](https://user-images.githubusercontent.com/33088785/161862832-1269a12e-16ce-427c-9a92-df3ee573a1fb.png)

- Download this repo
- Copy these <a href="libraries">libraries</a> into your Arduino install "libraries" folder
- Open this <a href="bitcoinSwitch.ino">bitcoinSwitch.ino</a> file in the Arduino IDE
- Select the correct ESP32 board from tools>board
- Upload to device

![trigger](https://user-images.githubusercontent.com/33088785/166829947-d0194b32-19fc-4a16-83d3-dc6f9af9337c.gif)
