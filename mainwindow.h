#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>
#include <QPixmap>
#include <opencv2/videoio.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectVideo();
    void cancelUpload();
    void updateVideoFrame(QPixmap pixmap);
    void openSupportLink();

private:
    Ui::MainWindow *ui;
    QLabel *videoLabel;
    cv::VideoCapture cap;
    void DisableUpload();
    void DisableDownload();
    void DisableCancel();
    void EnableUpload();
    void EnableDownload();
    void EnableCancel();
    void downloadProcessedVideo();
};

#endif // MAINWINDOW_H
