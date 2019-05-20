#!/bin/bash

function install_esp32() {
    python=python2
    pip=pip

    mkdir -p ~/Arduino/libraries/
    mkdir -p ~/Arduino/hardware/
    
    sudo usermod -a -G dialout $USER
    sudo apt update &&
	sudo apt install -yy libncurses5 git flex bison gperf ffmpeg python-pyserial python-pip

    git clone https://github.com/hardkernel/ODROID-GO.git ~/Arduino/libraries/ODROID-GO

    tmpdir=`mktemp -d`
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
	$python -m pip install --user -r ~/esp/esp-idf-odroidgo/docs/requirements.txt &&
	git clone -b v3.2 --recursive https://github.com/espressif/esp-idf.git esp-idf-official &&
	$python -m pip install --user -r ~/esp/esp-idf-official/requirements.txt &&
	ln -s ~/esp/esp-idf-official ~/esp/esp-idf

    setup_esp32
}

function setup_esp32() {
    export IDF_PATH="$HOME/esp/esp-idf"

    if [ -f "$HOME/esp/esp-idf/add_path.sh" ]; then
	source "$HOME/esp/esp-idf/add_path.sh" > /dev/null 2> /dev/null || true
    fi

    if [ -d "$HOME/Arduino/hardware/espressif/esp32/tools/xtensa-esp32-elf/bin" ]; then
	export PATH="$HOME/Arduino/hardware/espressif/esp32/tools/xtensa-esp32-elf/bin":$PATH
    fi
}

export setup_esp32
export install_esp32

install_esp32
