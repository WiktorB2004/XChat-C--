# Use a base image with the desired Linux distribution and necessary tools
FROM ubuntu:20.04

# Install necessary dependencies
RUN apt-get update \
    && apt-get install -y \
    wget \
    build-essential \
    libssl-dev \
    qt5-default \
    qtbase5-dev \
    qtdeclarative5-dev \
    qml-module-qtquick-controls2


# Install Cmake
RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1.tar.gz && \
    tar -zxvf cmake-3.28.1.tar.gz && \
    cd cmake-3.28.1 && \
    ./bootstrap && \
    make && \
    make install

# Set up the working directory inside the container
WORKDIR /app/client

# Copy the source code of the client into the container
COPY . .

# Build the client using CMake
RUN cmake -Bbuild -H. \
    && cmake --build build --target client -- -j$(nproc)

# Set the default command to run the client when the container starts
CMD ["./bin/client"]
