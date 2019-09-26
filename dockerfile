FROM cm0x4d/ubuntu-qt5-cmake

USER root
RUN apt update
RUN apt install -y cmake git libqt5websockets5-dev

RUN git clone https://github.com/ironexmaiden/sortserver.git \
&& cd sortserver \
&& mkdir build \
&& cd build \
&& cmake -DCMAKE_BUILD_TYPE=Release ../ \
&& make -j \
&& make install

