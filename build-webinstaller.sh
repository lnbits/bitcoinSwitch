#!/bin/sh
git clone https://github.com/lnbits/lnbits.github.io mainpage
cp -r ./mainpage/assets ./installer/main_assets
rm -rf mainpage

mkdir -p ./installer/firmware/esp32

for version in $(jq -r '.versions[]' ./installer/versions.json); do
    rm -f firmware.zip
    mkdir -p ./installer/firmware/esp32/$version
    wget https://github.com/lnbits/bitcoinswitch/releases/download/$version/firmware.zip
    unzip firmware.zip -d ./installer/firmware/esp32
    rm firmware.zip
done
