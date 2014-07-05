#include "Page.hpp"

Page::Page() {
}

void Page::renderPng(QByteArray& ba) const {
    QImage image(viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&image);
    mainFrame()->render(&painter);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
}

