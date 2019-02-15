#! /bin/bash

local python=python3
local pip=pip

mkdir -p ~/Arduino/libraries/
mkdir -p ~/Arduino/hardware/

sudo usermod -a -G dialout $USER
sudo apt update &&
    sudo apt install -yy libncurses5 git flex bison gperf ffmpeg

git clone https://github.com/hardkernel/ODROID-GO.git ~/Arduino/libraries/ODROID-GO

local tmpdir=`mktemp -d`
cd $tmpdir &&
    wget https://bootstrap.pypa.io/get-pip.py &&
    sudo -H $python get-pip.py &&
    sudo -H $pip install pyserial
sudo rm -rf $tmpdir

mkdir -p ~/Arduino/hardware/espressif
cd ~/Arduino/hardware/espressif &&
    git clone https://github.com/espressif/arduino-esp32.git esp32

cd ~/Arduino/hardware/espressif/esp32 &&
    git submodule update --init --recursive

cd ~/Arduino/hardware/espressif/esp32/tools &&
    $python get.py

mkdir -p ~/esp/
cd ~/esp &&
    git clone https://github.com/OtherCrashOverride/esp-idf --recursive esp-idf-odroidgo &&
    $python -m pip install --user -r ~/esp/esp-idf-odroidgo/requirements.txt &&
    ln -s ~/esp/esp-idf-odroidgo/ ~/esp-idf/    
