# Use a base image with the desired Linux distribution
FROM ubuntu:latest

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    gnupg \
    libqt6gui6 \
    libopencv-dev \
    qt6-base-dev \
    libglx-dev \
    libx11-dev \
    libxcb-* \
    libxcb-cursor0 \
    libx11-xcb1 \
    libxkbcommon-x11-0 \
    libxcb-xkb1 \
    libxcb-render-util0 \
    libxcb-keysyms1 \
    libxcb-image0 \
    libgl1-mesa-dev \
    mesa-common-dev \
    libxcb-xinerama0 \
    qt6-multimedia-dev \
    pcmanfm \
    featherpad \
    lxtask \
    xterm \
    && rm -rf /var/lib/apt/lists/*

# Copy the source code into the container
COPY ./ /app/

# Set the working directory
WORKDIR /app

# Set operating system
ARG OS_TYPE=Linux

# Make the opencv binaries before proceeding to build the application and cd back to app directory to proceed with execution (takes long time because its building opencv from sources)
RUN mkdir -p /app/opencv/build-linux-opencl-GCC && \
    cd /app/opencv/build-linux-opencl-GCC && \
    cmake -D CMAKE_BUILD_TYPE=Release /app/opencv/sources && \
    make -j$(nproc) && \
    make install

# Set OpenCV_DIR environment variable based on the operating system
ENV OpenCV_DIR=/app/opencv/opencv/build-linux-opencl-GCC

# Set display to see ubuntu GUI using vxcsrv or another tool
ENV DISPLAY=host.docker.internal:0.0

# Build the application
RUN mkdir build && cd build && \
    cmake -DOS_TYPE=Linux .. && \
    make

# Set the entry point
ENTRYPOINT ["./build/BlurIT"]

CMD pcmanfm