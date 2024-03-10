#include <QFile>
#include <QIODevice>
#include <QCoreApplication>
#include <opencv2/dnn.hpp>

class facedetector {
public:
    facedetector();
    ~facedetector();
    void detectFaces(cv::Mat& frame, std::vector<cv::Rect>& faces);

private:
    cv::dnn::Net net;
};
