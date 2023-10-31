#!/bin/sh
PROJECT_NAME=bitcoinSwitch
REPO=https://github.com/lnbits/hardware-installer
RELEASE=$REPO/releases/download
INSTALLER_PATH=./hardware-installer/public/firmware

git clone $REPO

cp README.md ./hardware-installer/public/README.md
cp versions.json ./hardware-installer/src/versions.json
cp config.js ./hardware-installer/src/config.js

sed -i "s/%title%/$PROJECT_NAME/g" ./hardware-installer/index.html

mkdir -p $INSTALLER_PATH
for device in $(jq -r '.devices[]' ./hardware-installer/src/versions.json); do
    for version in $(jq -r '.versions[]' ./hardware-installer/src/versions.json); do
        mkdir -p $INSTALLER_PATH/$device/$version
        wget $RELEASE/$version/$PROJECT_NAME.ino.bin
        wget $RELEASE/$version/$PROJECT_NAME.ino.partitions.bin
        wget $RELEASE/$version/$PROJECT_NAME.ino.bootloader.bin
        mv $PROJECT_NAME.ino.bin $INSTALLER_PATH/$device/$version
        mv $PROJECT_NAME.ino.partitions.bin $INSTALLER_PATH/$device/$version
        mv $PROJECT_NAME.ino.bootloader.bin $INSTALLER_PATH/$device/$version
    done
done
