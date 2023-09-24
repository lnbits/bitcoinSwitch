#!/bin/sh
git clone https://github.com/lnbits/lnbits.github.io mainpage
cp -r ./mainpage/assets ./installer/main_assets
rm -rf mainpage

mkdir -p ./installer/firmware/esp32

for version in $(jq -r '.versions[]' ./installer/versions.json); do
    mkdir -p ./installer/firmware/esp32/$version
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/manifest.json
    mv manifest.json ./installer/firmware/esp32/$version
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/bootloader.bin
    mv bootloader.bin ./installer/firmware/esp32/$version
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/boot_app0.bin
    mv boot_app0.bin ./installer/firmware/esp32/$version
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/bitcoinSwitch.ino.bin
    mv bitcoinSwitch.ino.bin ./installer/firmware/esp32/$version
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/bitcoinSwitch.ino.partitions.bin
    mv bitcoinSwitch.ino.partitions.bin ./installer/firmware/esp32/$version
done
