#include "customvideoplayer.h"
#include <QPainter>

CustomVideoPlayer::CustomVideoPlayer(QWidget *parent)
    : QVideoWidget(parent)
{
}

void CustomVideoPlayer::updateFrame(const QPixmap &pixmap)
{
    currentFrame = pixmap;
    update(); // Trigger a repaint
}

void CustomVideoPlayer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), currentFrame);
}
