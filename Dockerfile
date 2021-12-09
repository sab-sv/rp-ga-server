FROM ubuntu:16.04
RUN mkdir myprog
RUN apt update
WORKDIR /myprog
ENV DIRPATH /myprog
RUN apt install -y libxext6 libxext-dev libxtst6 libxtst-dev libfreetype6 libfreetype6-dev libogg-dev yasm nasm libx11-dev mesa-utils libpulse-dev libasound2-dev  libxtst-dev libswscale-dev libpostproc-dev libavformat-dev libsdl2-dev libsdl2-ttf-dev libopus-dev cmake  git libx265-dev libsdl-image1.2 vim wget
RUN apt-get install -y patch make cmake g++ pkg-config libx11-dev libxext-dev libxtst-dev libfreetype6-dev libgl1-mesa-dev libglu1-mesa-dev libpulse-dev libasound2-dev lib32z1
RUN apt-get install -y libsdl2-dev libsdl2-ttf-dev yasm \
                libmp3lame-dev libopus-dev libogg-dev \
                libvorbis-dev libtheora-dev \
                libvpx-dev libx264-dev libxvidcore-dev
#RUN ln -s /usr/include/locale.h /usr/include/xlocale.h
RUN pwd
RUN git clone "https://github.com/CloudMaster-Manoj/gaminganywhere.git"/ && \
ls -lrt
RUN /bin/bash -c "source /myprog/gaminganywhere/env-setup"
RUN apt install sudo
RUN cd /myprog/gaminganywhere/deps.src/ && pwd &&  make
RUN apt install -y libavdevice-dev libavfilter-dev
RUN chmod 777 /etc/ld.so.conf
RUN cd /myprog
RUN export LD_LIBRARY_PATH="/myprog/gaminganywhere/deps.posix/lib"
RUN echo "/myprog/gaminganywhere/deps.posix/lib" >> /etc/ld.so.conf
RUN ls -lrt /myprog/gaminganywhere/ga/module/server-ffmpeg/
RUN apt install -y libga-dev
RUN apt-get install -y patch make cmake g++ pkg-config libx11-dev libxext-dev libxtst-dev libfreetype6-dev libgl1-mesa-dev libglu1-mesa-dev libpulse-dev libasound2-dev lib32z1
RUN cd /myprog/gaminganywhere/ga/ && \
        make all && \
        make install
RUN git clone https://github.com/CloudMaster-Manoj/AC.git
RUN apt install -y libsdl1.2debian libsdl-image1.2 zlib1g libogg0 libvorbis0a libopenal1  libogg-dev libsdl-dev clang+ libsdl-image1.2-dev libopenal-dev
RUN cd /myprog/AC/source/src/ && make all && make install
RUN cd /myprog/gaminganywhere/bin && chmod 777 ga-server-event-driven && ls -lrt && chmod 777 ga-client
RUN apt-get install -y software-properties-common
RUN apt-get install -y mesa-utils

RUN apt-get install -y binutils

# Required for Steam
RUN dpkg --add-architecture i386
RUN add-apt-repository multiverse
RUN export DISPLAY=:0.0
RUN cat /etc/ssh/ss* | grep -i x11*
RUN cat /myprog/gaminganywhere/bin/config/server.assaultcube.linux.conf
RUN sed -i 's/native_server/native_client/g' /myprog/gaminganywhere/bin/config/server.assaultcube.linux.conf
RUN cat /myprog/gaminganywhere/bin/config/server.assaultcube.linux.conf
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y keyboard-configuration
RUN apt install -y debconf-utils
RUN apt install curl
RUN  apt install -y apt-transport-https
RUN apt install x11-xserver-utils
#RUN /myprog/gaminganywhere/bin/ga-server-event-driven /myprog/gaminganywhere/bin/config/server.assaultcube.linux.conf
#ADD NVIDIA-DRIVER.run /tmp/NVIDIA-DRIVER.run
RUN apt install libx11-6
RUN DEBIAN_FRONTEND=noninteractive apt install -y ubuntu-desktop
RUN apt install -y vnc4server
RUN apt install -y gnome-panel