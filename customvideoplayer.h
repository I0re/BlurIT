#ifndef CUSTOMVIDEOPLAYER_H
#define CUSTOMVIDEOPLAYER_H

#include <QVideoWidget>

class CustomVideoPlayer : public QVideoWidget
{
    Q_OBJECT
public:
    explicit CustomVideoPlayer(QWidget *parent = nullptr);
    void updateFrame(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap currentFrame;
};

#endif // CUSTOMVIDEOPLAYER_H
