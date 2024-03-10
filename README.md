# BlurIT

BlurIT is a video processing application developed in C++ and Qt, primarily designed to detect and blur faces in videos. It utilizes OpenCV for face detection and image processing.

## Features

- **Face Detection**: Utilizes OpenCV's deep neural network model to detect faces in video frames coupled with OpenCL for gpu acceleration.
- **Face Blurring**: Blurs detected faces in real-time using Gaussian blur at a hardcoded value of 10 (changeable).
- **Video Processing**: Processes videos in common formats such as MP4, AVI, and MKV.
- **User Interface**: Provides a user-friendly interface for uploading videos, processing, and downloading the processed video.

## Demo


## Prerequisites

Before you begin, ensure you have met the following requirements:

- CMake (version 3.5 or higher)
- OpenCV (version 4.9.0)
- Qt6

## Installation

To install BlurIT, follow these steps or download release:

1. Clone the repository:
```bash
git clone https://github.com/I0re/BlurIT.git
```
```bash
cd BlurIT
```

2. Build the application (Outside of Qt Creator/ still need Qt installed):
```bash
mkdir build
```
```bash
cd build
```
```bash
cmake -G "Visual Studio 17 2022" ..
```

3. Build the output .sln file in either debug or release versions.

5. If building from source in the directory of output exe file:
- Run windeployqt6.exe on the output executable file.
- Place "opencv_videoio_ffmpeg490_64.dll", "opencv_world490.dll", "opencv_world490.lib" and "opencv_world490.pdb" in the same directory as output executable file.

## Usage

To use BlurIT, follow these steps:

1. Launch the application.
2. Click on the "Upload" button to select a video file.
3. The application will detect and blur faces in the choosen video.
4. After processing, click on the "Download" button to save the processed video.

## Docker Support

BlurIT also provides Docker support for easy deployment on Unbuntu Linux:
```bash
docker build --build-arg OS_TYPE=Linux -t blurit .
```
```bash
docker run -it blurit
```

## What Could Improve
1. Capturing video audio and enabling output video to maintain the original video audio.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Special thanks to the developers of Qt, OpenCV and Docker for providing powerful libraries.
