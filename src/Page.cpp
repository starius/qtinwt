#include <Wt/WServer>

#include "Application.hpp"
#include "Page.hpp"

Page::Page(QString session_id):
    sessionId_(session_id.toStdString()) {
    setViewportSize(QSize(640, 480));
    setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(this, SIGNAL(linkClicked(QUrl)),
            this, SLOT(loadInMainFrame(QUrl)));
    QWebFrame* frame = mainFrame();
    connect(frame, SIGNAL(titleChanged(QString)),
            this, SLOT(onTitleChanged(QString)));
    connect(frame, SIGNAL(urlChanged(QUrl)),
            this, SLOT(onUrlChanged(QUrl)));
}

void Page::post(const F& f) {
    WServer::instance()->post(sessionId_, f);
}

void Page::renderPng() {
    QByteArray ba;
    QImage image(viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&image);
    mainFrame()->render(&painter);
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    post(boost::bind(&App::imageChanged, ba));
}

void Page::loadInMainFrame(QUrl url) {
    mainFrame()->load(url);
}

void Page::onTitleChanged(QString title) {
    post(boost::bind(&App::titleChanged, title));
}

void Page::onUrlChanged(QUrl url) {
    post(boost::bind(&App::urlChanged, url));
}

