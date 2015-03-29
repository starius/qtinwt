/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#include <Wt/WServer>

#include "Application.hpp"
#include "Page.hpp"
#include "SafeHtml.hpp"

#include <QtCore>

Page::Page(QString session_id):
    sessionId_(session_id.toStdString()),
    renderingPending_(false) {
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
    if (!renderingPending_) {
        renderingPending_ = true;
        QTimer::singleShot(REFRESH_MSEC / 2, this,
                SLOT(realRenderPng()));
    }
}

void Page::realRenderPng() {
    renderingPending_ = false;
    QByteArray ba;
    QImage image(viewportSize(),
                 QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    mainFrame()->render(&painter);
    painter.end();
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    post(boost::bind(&App::imageChanged, ba));
}

void Page::onHtmlPage() {
    QWebElement d = mainFrame()->documentElement();
    post(boost::bind(&App::htmlChanged, safeHtml(d)));
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
