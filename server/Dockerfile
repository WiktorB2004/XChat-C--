# Use a base image with the desired Linux distribution and necessary tools
FROM ubuntu:20.04


# Set up the working directory inside the container
WORKDIR /app

# Ubuntu configuration
ARG DEBIAN_FRONTEND=noninteractive

# Install necessary dependencies
RUN apt-get update \
    && apt-get install -y \
    wget \
    build-essential \
    libssl-dev

# Install Cmake
RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1.tar.gz && \
    tar -zxvf cmake-3.28.1.tar.gz && \
    cd cmake-3.28.1 && \
    ./bootstrap && \
    make && \
    make install

# Copy the source code into the container
COPY . /app

# Build the server using CMake
RUN cmake -Bbuild -H. \
    && cmake --build build --target server -- -j$(nproc)

# Expose the necessary ports for the server
EXPOSE 8080

# Set the default command to run the server when the container starts
CMD ["./bin/server"]
