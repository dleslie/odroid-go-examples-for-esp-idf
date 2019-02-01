#! /bin/bash

sudo usermod -a -G dialout $USER &&							\
sudo apt update &&									\
sudo apt install -yy libncurses5 git flex bison gperf ffmpeg &&				\
wget https://bootstrap.pypa.io/get-pip.py &&						\
sudo -H python get-pip.py &&								\
sudo -H pip install pyserial &&								\
rm get-pip.py &&									\
mkdir -p ~/Arduino/hardware/espressif &&						\
cd ~/Arduino/hardware/espressif &&							\
git clone https://github.com/espressif/arduino-esp32.git esp32 &&			\
cd esp32 &&										\
git submodule update --init --recursive &&						\
cd tools &&										\
python3 get.py &&									\
cd ~ &&											\
git clone https://github.com/hardkernel/ODROID-GO.git ~/Arduino/libraries/ODROID-GO &&	\    
mkdir -p ~/esp/esp-idf-odroidgo &&							\
git clone https://github.com/OtherCrashOverride/esp-idf  --recursive ~/esp/esp-idf &&	\
python2 -m pip install --user -r ~/esp/esp-idf-odroidgo/requirements.txt &&             \
echo "Now you'll want to point IDF_PATH at ~/esp/esp-idf-odroidgo/"    
