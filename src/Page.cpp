#include "Page.hpp"

Page::Page() {
    setViewportSize(QSize(640, 480));
    setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(this, SIGNAL(linkClicked(QUrl)),
            this, SLOT(loadInMainFrame(QUrl)));
}

void Page::renderPng() {
    QByteArray ba;
    QImage image(viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&image);
    mainFrame()->render(&painter);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    emit pngRendered(ba);
}

void Page::loadInMainFrame(QUrl url) {
    mainFrame()->load(url);
}
