# Use a base image with the desired Linux distribution and necessary tools
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# Install necessary dependencies
RUN apt-get update \
    && apt-get install -y \
    wget \
    build-essential \
    libssl-dev \
    freeglut3-dev \
    python3

# Install Cmake
RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1.tar.gz && \
    tar -zxvf cmake-3.28.1.tar.gz && \
    cd cmake-3.28.1 && \
    ./bootstrap && \
    make -j4 && \
    make install

# Install Qt6.6.2
RUN cd tmp/ && \
    wget https://download.qt.io/official_releases/qt/6.6/6.6.2/single/qt-everywhere-src-6.6.2.tar.xz && \
    tar xf qt-everywhere-src-6.6.2.tar.xz && \
    mkdir -p ~/dev/qt-build && \
    cd ~/dev/qt-build && \
    /tmp/qt-everywhere-src-6.6.2/configure && \
    cmake --build . -j4 && \
    cmake --install .

ENV PATH="/usr/local/Qt-6.6.2/bin:${PATH}"

# Copy the source code of the client into the container
COPY . .

# Build the client using CMake
RUN cmake -Bbuild -H. \
    && cmake --build build --target client --parallel

# Set the default command to run the client when the container starts
CMD ["./bin/client"]
