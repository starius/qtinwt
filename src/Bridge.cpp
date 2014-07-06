#include <boost/bind.hpp>
#include <Wt/WServer>

#include "Bridge.hpp"
#include "Pages.hpp"
#include "Application.hpp"
#include "util.hpp"

Bridge::Bridge() {
    sessionId_ = wApp->sessionId();
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
    emit createPage(qsessionId());
}

void Bridge::deleteP() {
    emit deletePage(qsessionId());
}

void Bridge::loadInP(QUrl url) {
    emit loadInPage(qsessionId(), url);
}

void Bridge::renderP() {
    emit renderPage(qsessionId());
}

void Bridge::setS(QSize size) {
    emit setSize(qsessionId(), size);
}

void Bridge::mouse(QEvent::Type type, MOUSE_ARGS) {
    emit moused(qsessionId(), type, MOUSE_NAMES);
}

void Bridge::wheel(int delta, MOUSE_ARGS) {
    emit wheeled(qsessionId(), delta, MOUSE_NAMES);
}

void Bridge::keye(int k, QEvent::Type type,
        Qt::KeyboardModifiers modifiers, QString text) {
    emit keyed(qsessionId(), k, type, modifiers, text);
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
    image_ = image;
}

