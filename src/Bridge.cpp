#include <boost/bind.hpp>
#include <Wt/WServer>

#include "Bridge.hpp"
#include "Pages.hpp"
#include "Application.hpp"

Bridge::Bridge() {
    sessionId_ = wApp->sessionId();
    qSessionId_ = QString::fromUtf8(sessionId_.c_str());
    renderingPending_ = false;
    connect(this, SIGNAL(createPage(QString)),
            PAGES, SLOT(createPage(QString)));
    connect(this, SIGNAL(deletePage(QString)),
            PAGES, SLOT(deletePage(QString)));
    connect(this, SIGNAL(loadInPage(QString, QUrl)),
            PAGES, SLOT(loadInPage(QString, QUrl)));
    connect(this, SIGNAL(renderPage(QString)),
            PAGES, SLOT(renderPage(QString)));
    connect(this, SIGNAL(setSize(QString, QSize)),
            PAGES, SLOT(setSize(QString, QSize)));
    connect(this, SIGNAL(moused(QString, QEvent::Type, QPoint,
                Qt::MouseButton, Qt::KeyboardModifiers)),
            PAGES, SLOT(moused(QString, QEvent::Type, QPoint,
                Qt::MouseButton, Qt::KeyboardModifiers)));
    connect(this, SIGNAL(wheeled(QString, int, QPoint,
                Qt::MouseButton, Qt::KeyboardModifiers)),
            PAGES, SLOT(wheeled(QString, int, QPoint,
                Qt::MouseButton, Qt::KeyboardModifiers)));
    connect(this, SIGNAL(keyed(QString, int, QEvent::Type,
                Qt::KeyboardModifiers, QString)),
            PAGES, SLOT(keyed(QString, int, QEvent::Type,
                Qt::KeyboardModifiers, QString)));
}

void Bridge::createP() {
    emit createPage(qSessionId_);
}

void Bridge::deleteP() {
    emit deletePage(qSessionId_);
}

void Bridge::loadInP(QUrl url) {
    emit loadInPage(qSessionId_, url);
}

void Bridge::renderP(int timeout) {
    if (!renderingPending_) {
        renderingPending_ = true;
        QTimer::singleShot(timeout, this,
                SLOT(realRenderP()));
    }
}

void Bridge::realRenderP() {
    renderingPending_ = false;
    emit renderPage(qSessionId_);
}

void Bridge::setS(QSize size) {
    emit setSize(qSessionId_, size);
}

void Bridge::mouse(QEvent::Type type, MOUSE_ARGS) {
    emit moused(qSessionId_, type, MOUSE_NAMES);
}

void Bridge::wheel(int delta, MOUSE_ARGS) {
    emit wheeled(qSessionId_, delta, MOUSE_NAMES);
}

void Bridge::keye(int k, QEvent::Type type,
        Qt::KeyboardModifiers modifiers, QString text) {
    emit keyed(qSessionId_, k, type, modifiers, text);
}

void Bridge::post(const F& f) {
    WServer::instance()->post(sessionId_, f);
}

QByteArray Bridge::image() const {
    return image_;
}

void Bridge::titleChanged(QString title) {
    WString wtitle = toWString(title);
    post(boost::bind(&App::titleChanged, wtitle));
}

void Bridge::urlChanged(QUrl url) {
    WString wurl = toWString(url.toString());
    post(boost::bind(&App::urlChanged, wurl));
}

void Bridge::pngRendered(QByteArray image) {
    if (image != image_) {
        post(&App::imageChanged);
    }
    image_ = image;
}

