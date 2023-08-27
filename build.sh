#!/bin/sh
git clone https://github.com/lnbits/lnbits.github.io mainpage
cp -r ./mainpage/assets ./installer/main_assets
rm -rf mainpage
