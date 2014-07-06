#include <cctype>

#include "Pages.hpp"
#include "Page.hpp"

#include <QtGui>

Pages* Pages::globalInstance_ = 0;

Pages::Pages() {
    globalInstance_ = this;
}

Pages* Pages::globalInstance() {
    return globalInstance_;
}

void Pages::createPage(QString key) {
    Page* page = new Page;
    pages_.insert(key, page);
    QWebFrame* frame = page->mainFrame();
    connect(frame, SIGNAL(titleChanged(QString)),
            sender(), SLOT(titleChanged(QString)));
    connect(frame, SIGNAL(urlChanged(QUrl)),
            sender(), SLOT(urlChanged(QUrl)));
    connect(page, SIGNAL(pngRendered(QByteArray)),
            sender(), SLOT(pngRendered(QByteArray)));
}

void Pages::deletePage(QString key) {
    It it = pages_.find(key);
    if (it != pages_.end()) {
        Page* page = *it;
        page->deleteLater();
        pages_.erase(it);
    }
}

void Pages::loadInPage(QString key, QUrl url) {
    Page* page = pageOf(key);
    if (page) {
        page->mainFrame()->load(url);
    }
}

void Pages::renderPage(QString key) {
    It it = pages_.find(key);
    if (it != pages_.end()) {
        Page* page = *it;
        page->renderPng();
    }
}

void Pages::setSize(QString key, QSize size) {
    Page* page = pageOf(key);
    if (page) {
        page->setViewportSize(size);
    }
}

void Pages::moused(QString key, QEvent::Type type, MOUSE_ARGS) {
    Page* page = pageOf(key);
    if (page) {
        QMouseEvent e(type, pos, button, button, modifiers);
        QApplication::sendEvent(page, &e);
    }
}

void Pages::wheeled(QString key, int delta, MOUSE_ARGS) {
    Page* page = pageOf(key);
    if (page) {
        Qt::Orientation o = Qt::Vertical;
        QWebFrame* f = page->mainFrame();
        QRect r = f->scrollBarGeometry(Qt::Horizontal);
        int height = page->viewportSize().height();
        if (!r.isEmpty() && pos.y() > height - r.height()) {
            o = Qt::Horizontal;
        }
        QWheelEvent e(pos, delta, button, modifiers, o);
        QApplication::sendEvent(page, &e);
    }
}

void Pages::keyed(QString key, int k, QEvent::Type type,
        Qt::KeyboardModifiers modifiers, QString text) {
    if (k == Qt::Key_Control || k == Qt::Key_Shift ||
            k == Qt::Key_Alt) {
        return;
    }
    Page* page = pageOf(key);
    if (page) {
        if (text.isEmpty() && isalpha(k)) {
            bool shift = modifiers.testFlag(Qt::ShiftModifier);
            text = QChar(shift ? k : tolower(k));
        }
        QKeyEvent e(type, k, modifiers, text);
        QApplication::sendEvent(page, &e);
    }
}

Page* Pages::pageOf(QString key) const {
    CIt it = pages_.find(key);
    if (it != pages_.end()) {
        return *it;
    } else {
        return 0;
    }
}

