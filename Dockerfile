FROM ubuntu:bionic

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    	    git \
	    wget \
	    libncurses5 \
	    libncurses-dev \
	    flex \
	    bison \
	    gperf \
	    ffmpeg \
	    python-pip && \
    apt-get clean && \
    pip install pyserial

RUN mkdir -p /opt/bin
RUN mkdir -p /opt/Arduino/libraries 
RUN mkdir -p /opt/Arduino/hardware/espressif
RUN mkdir -p /opt/esp

WORKDIR /opt

COPY mkfw /opt/mkfw
RUN cd mkfw && make && cp /opt/mkfw/mkfw /opt/bin && rm -rf /opt/mkfw

RUN git clone https://github.com/dleslie/ODROID-GO.git /opt/Arduino/libraries/ODROID-GO && \
    git clone https://github.com/dleslie/arduino-esp32.git /opt/Arduino/hardware/espressif/esp32 && \
    cd /opt/Arduino/hardware/espressif/esp32 && \
    git submodule update --init --recursive && \
    cd /opt/Arduino/hardware/espressif/esp32/tools && \
    python get.py

RUN cd /opt/esp && \
    git clone https://github.com/OtherCrashOverride/esp-idf --recursive esp-idf-odroidgo && \
    python -m pip install --user -r /opt/esp/esp-idf-odroidgo/docs/requirements.txt && \
    ln -s /opt/esp/esp-idf-odroidgo /opt/esp/esp-idf

ENV PATH="/opt/Arduino/hardware/espressif/esp32/tools/xtensa-esp32-elf/bin":/opt/bin:$PATH
ENV IDF_PATH="/opt/esp/esp-idf"
ENV IDF_ADD_PATHS_EXTRAS="${IDF_ADD_PATHS_EXTRAS}:${IDF_PATH}/components/esptool_py/esptool"
ENV IDF_ADD_PATHS_EXTRAS="${IDF_ADD_PATHS_EXTRAS}:${IDF_PATH}/components/espcoredump"
ENV IDF_ADD_PATHS_EXTRAS="${IDF_ADD_PATHS_EXTRAS}:${IDF_PATH}/components/partition_table/"
ENV IDF_ADD_PATHS_EXTRAS="${IDF_ADD_PATHS_EXTRAS}:${IDF_PATH}/tools/"
ENV export PATH="${IDF_ADD_PATHS_EXTRAS}:${PATH}"
