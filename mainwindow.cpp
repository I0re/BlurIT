#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <opencv2/opencv.hpp>
#include <QMediaPlayer>
#include <QUrl>
#include "facedetector.h"
#include <QFile>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoLabel(nullptr)
{
    ui->setupUi(this);

    // Order of what happens when "Upload" button is pressed
    connect(ui->Upload, &QPushButton::clicked, this, &MainWindow::selectVideo);

    // Order of what happens when "Download" button is pressed
    connect(ui->Download, &QPushButton::clicked, this, &MainWindow::downloadProcessedVideo);

    // Order of what happens when "Cancel" button is pressed
    connect(ui->Cancel, &QPushButton::clicked, this, &MainWindow::cancelUpload);

    // Order of what happens when "Buy Me A Coffee!" button is pressed
    connect(ui->coffee, &QPushButton::clicked, this, &MainWindow::openSupportLink);

    DisableCancel();
    DisableDownload();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectVideo()
{
    // Checks if the videoplayer remains on screen and if does deletes it
    if (videoLabel) {
        delete videoLabel;
        videoLabel = nullptr;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"), "", tr("Video Files (*.mp4 *.avi *.mkv);;All Files (*)"));

    if (!fileName.isEmpty())
    {
        // Check if the selected file has a valid video file extension
        QFileInfo fileInfo(fileName);
        QString fileExtension = fileInfo.suffix().toLower();
        QStringList allowedExtensions = {"mp4", "avi", "mkv", "mov", "wmv", "flv", "3gp", "mpeg", "webm", "ogg", "asf", "vob", "m4v", "swf", "ts"};

        if (allowedExtensions.contains(fileExtension))
        {
            DisableUpload();
            DisableDownload();
            EnableCancel();

            // Open the selected video file
            cap.open(fileName.toStdString());
            if (!cap.isOpened()) {
                QMessageBox::warning(this, tr("Error"), tr("Error opening video file."));
                EnableUpload();
                return;
            }

            // Create a video writer object to write the video
            QString outputFile = QDir::tempPath() + "/output_video.avi";
            cv::VideoWriter videoWriter(outputFile.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), cap.get(cv::CAP_PROP_FPS), cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

            // Get total number of frames in the video
            int totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);

            // Dynamically create a QLabel for displaying video frames
            videoLabel = new QLabel(this);
            videoLabel->setFixedSize(ui->VideoPlayerFrame->size());
            videoLabel->setAlignment(Qt::AlignCenter);
            ui->verticalLayout_5->addWidget(videoLabel);

            // Create facedetector object
            facedetector detector;

            // Read and process each frame of the video
            cv::Mat frame;
            int currentFrame = 0;
            while (cap.read(frame)) {
                // Detect faces in the frame
                std::vector<cv::Rect> faces;
                detector.detectFaces(frame, faces);

                // Blur detected faces
                for (const auto& face : faces) {
                    cv::Mat faceROI = frame(face);
                    cv::GaussianBlur(faceROI, faceROI, cv::Size(0, 0), 10);
                }

                // Write the frame to the video writer object
                videoWriter.write(frame);

                // Convert OpenCV frame to QImage
                QImage qImg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
                QPixmap qPixMap = QPixmap::fromImage(qImg.rgbSwapped());

                // Update the video label with the current frame
                if(videoLabel) {
                    videoLabel->setPixmap(qPixMap.scaled(videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                }

                // Calculate progress and update progress bar
                currentFrame++;
                int progress = static_cast<int>((static_cast<float>(currentFrame) / static_cast<float>(totalFrames)) * 100);
                ui->progressBar->setValue(progress);
                if (progress == 100) {
                    EnableDownload();
                    EnableUpload();
                    DisableCancel();
                }

                // Handle events and update UI
                QApplication::processEvents();
            }

            // Enable/disable buttons after video playback
            EnableUpload();
            DisableCancel();
        }
        else {
            QMessageBox::warning(this, tr("Invalid File"), tr("Please select a valid video file."));
            EnableUpload();
        }
    }
    else {
        EnableUpload();
    }
}

void MainWindow::cancelUpload()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Cancel Confirmation"),
        tr("Are you sure you want to cancel the upload?"),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes)
    {
        // Stop both video and audio playback and reset UI
        if (videoLabel) {
            delete videoLabel;
            videoLabel = nullptr;
        }

        // Release the video capture object
        if (cap.isOpened()) {
            cap.release();
        }

        ui->progressBar->setValue(0);
        EnableUpload();
        DisableCancel();
        DisableDownload();
    }
}

void MainWindow::updateVideoFrame(QPixmap pixmap)
{
    if(videoLabel) {
        videoLabel->setPixmap(pixmap);
    }
}

void MainWindow::downloadProcessedVideo()
{
    // Check if the processed video file exists
    QString outputFile = QDir::tempPath() + "/output_video.avi";
    QFile videoFile(outputFile);
    if (!videoFile.exists()) {
        QMessageBox::warning(this, tr("Error"), tr("Processed video file not found."));
        return;
    }

    // Get the directory for saving downloaded files
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    // Prompt the user to choose the download location
    QString downloadPath = QFileDialog::getSaveFileName(this, tr("Save Processed Video"), defaultPath, tr("AVI Files (*.avi)"));

    // Check if the user cancelled the dialog
    if(downloadPath.isEmpty())
        return;

    // Copy the processed video file to the selected directory
    if (!QFile::copy(outputFile, downloadPath)) {
        QMessageBox::warning(this, tr("Error"), tr("Error copying processed video file."));
        return;
    }

    // Notify the user that the video has been downloaded
    QMessageBox::information(this, tr("Download Complete"), tr("Proccessed video has been downloaded to the selected location."));
}

void MainWindow::openSupportLink()
{
    QDesktopServices::openUrl(QUrl("https://www.buymeacoffee.com/andresonlyz"));
}

void MainWindow::DisableUpload()
{
    ui->Upload->setEnabled(false);
}

void MainWindow::DisableDownload()
{
    ui->Download->setEnabled(false);
}

void MainWindow::DisableCancel()
{
    ui->Cancel->setEnabled(false);
}

void MainWindow::EnableUpload()
{
    ui->Upload->setEnabled(true);
}

void MainWindow::EnableDownload()
{
    ui->Download->setEnabled(true);
}

void MainWindow::EnableCancel()
{
    ui->Cancel->setEnabled(true);
}
