#include "Page.hpp"

Page::Page() {
}

void Page::renderPng(QByteArray& ba) const {
    QSize size(640, 480);
    setViewportSize(size);
    QImage image(size, QImage::Format_ARGB32);
    QPainter painter(&image);
    mainFrame()->render(&painter);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
}

