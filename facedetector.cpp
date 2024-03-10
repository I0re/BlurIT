#include "facedetector.h"
#include <QFile>
#include <QIODevice>
#include <iostream>
#include <QCoreApplication>
#include <opencv2/core/ocl.hpp>

facedetector::facedetector() {
    // Get the path to the directory where the executable is located
    QString exeDir = QCoreApplication::applicationDirPath();

    // Construct the file paths relative to the executable directory
    QString modelPath = exeDir + "/res10_300x300_ssd_iter_140000.caffemodel";
    QString prototxtPath = exeDir + "/deploy.prototxt.txt";

    // Use OpenCL if available
    if (!cv::ocl::haveOpenCL()) {
        std::cerr << "OpenCL is not available." << std::endl;
    } else {
        std::cout << "OpenCL is available." << std::endl;
    }

    // Initialize the neural network model
    net = cv::dnn::readNetFromCaffe(prototxtPath.toStdString(), modelPath.toStdString());

    if (net.empty()) {
        std::cerr << "Error loading model from resource." << std::endl;
    } else {
        std::cout << "Model loaded successfully." << std::endl;
    }

    // Use OpenCL backend for DNN
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);
}

facedetector::~facedetector() {}

void facedetector::detectFaces(cv::Mat& frame, std::vector<cv::Rect>& faces) {
    if (net.empty()) {
        std::cerr << "Error: Network not initialized." << std::endl;
        return;
    }

    // Convert input frame to a blob for OpenCL acceleration
    cv::UMat frameUMat = frame.getUMat(cv::ACCESS_READ);
    cv::Mat inputBlob = cv::dnn::blobFromImage(frameUMat, 1.0, cv::Size(300, 300), cv::Scalar(104, 177, 123), false, false);

    // Use OpenCL backend for DNN
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);

    // Set input to the network
    net.setInput(inputBlob, "data");

    // Forward pass to get detection results
    cv::Mat detection = net.forward("detection_out");

    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    faces.clear();
    for (int i = 0; i < detectionMat.rows; ++i) {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > 0.5) {
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

            // Check if the coordinates are within the bounds of the frame
            if (x1 >= 0 && y1 >= 0 && x2 < frame.cols && y2 < frame.rows) {
                faces.emplace_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
            } else {
                std::cerr << "Error: Detected face coordinates out of bounds." << std::endl;
            }
        }
    }
}
