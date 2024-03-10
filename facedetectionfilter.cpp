// facedetectionfilter.cpp
#include "facedetectionfilter.h"

FaceDetectionSink::FaceDetectionSink(QObject *parent)
    : QObject(parent)
{
}

bool FaceDetectionSink::present(const QVideoFrame &frame)
{
    if (!frame.isValid())
        return false;

    // Convert QVideoFrame to OpenCV Mat
    cv::Mat cvFrame(frame.height(), frame.width(), CV_8UC4, frame.bits());

    // Perform face detection and tracking using your existing facedetector class
    facedetector faceDetector;
    std::vector<cv::Rect> faces;
    faceDetector.detectFaces(cvFrame, faces);

    // Blur the detected faces
    for (const auto &face : faces)
    {
        cv::Mat faceRegion = cvFrame(face);
        cv::GaussianBlur(faceRegion, faceRegion, cv::Size(25, 25), 30);
    }

    // Convert processed frame back to QImage
    QImage qImg(cvFrame.data, cvFrame.cols, cvFrame.rows, cvFrame.step, QImage::Format_RGB888);
    QPixmap qPixMap = QPixmap::fromImage(qImg.rgbSwapped());

    // Emit the processed frame
    emit outputImage(qPixMap);

    return true;
}
