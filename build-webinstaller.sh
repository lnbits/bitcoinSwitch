#!/bin/sh
git clone https://github.com/lnbits/lnbits.github.io mainpage
cp -r ./mainpage/assets ./installer/main_assets
rm -rf mainpage

mkdir -p ./installer/firmware/esp32

for version in $(jq -r '.versions[]' ./installer/versions.json); do
    mkdir -p ./installer/firmware/esp32/$version
    cd ./installer/firmware/esp32/$version
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/manifest.json
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/bootloader.bin
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/boot_app0.bin
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/bitcoinSwitch.ino.bin
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/bitcoinSwitch.ino.partitions.bin
done
