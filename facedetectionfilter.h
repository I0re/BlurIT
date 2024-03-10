#ifndef FACEDETECTIONFILTER_H
#define FACEDETECTIONFILTER_H

#include <QObject>
#include <QVideoSink>
#include <QImage>
#include <QPixmap>

#include "facedetector.h" // Include your existing facedetector class header

class FaceDetectionSink : public QObject, public QVideoSink
{
    Q_OBJECT

public:
    explicit FaceDetectionSink(QObject *parent = nullptr);

    bool present(const QVideoFrame &frame) override;

signals:
    void outputImage(const QPixmap &pixmap);

private:
         // Define your member variables and methods here
};

#endif // FACEDETECTIONFILTER_H
