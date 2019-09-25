FROM cm0x4d/ubuntu-qt5-cmake

USER root
RUN apt update
RUN apt install -y cmake git

RUN git clone https://github.com/ironexmaiden/sortserver.git
RUN cd sortserver
RUN mkdir build
RUN cd build
RUN cmake -DCMAKE_BUILD_TYPE=Release ../
RUN make -j
RUN make install

