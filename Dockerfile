FROM ubuntu:bionic

ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /work
COPY bash_helpers.sh /work
RUN bash_helpers.sh && install_esp32

WORKDIR /src
RUN source /work/bash_helpers.sh && setup_esp32 && make