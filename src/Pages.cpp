/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#include <cctype>

#include "Pages.hpp"
#include "Page.hpp"

#include <QtGui>

Pages* Pages::globalInstance_ = 0;

Pages::Pages() {
    globalInstance_ = this;
    connect(this, SIGNAL(createPage(QString)),
            this, SLOT(onCreatePage(QString)));
    connect(this, SIGNAL(deletePage(QString)),
            this, SLOT(onDeletePage(QString)));
    connect(this, SIGNAL(loadInPage(QString, QUrl)),
            this, SLOT(onLoadInPage(QString, QUrl)));
    connect(this, SIGNAL(goBack(QString)),
            this, SLOT(onGoBack(QString)));
    connect(this, SIGNAL(renderPage(QString)),
            this, SLOT(onRenderPage(QString)));
    connect(this, SIGNAL(htmlPage(QString)),
            this, SLOT(onHtmlPage(QString)));
    connect(this, SIGNAL(updateInputs(QString, QString)),
            this, SLOT(onUpdateInputs(QString, QString)));
    connect(this, SIGNAL(setSize(QString, QSize)),
            this, SLOT(onSetSize(QString, QSize)));
    connect(this, SIGNAL(mouse(QString, QEvent::Type, QPoint,
                Qt::MouseButton, Qt::KeyboardModifiers)),
            this, SLOT(onMouse(QString, QEvent::Type, QPoint,
                Qt::MouseButton, Qt::KeyboardModifiers)));
    connect(this, SIGNAL(wheel(QString, int, QPoint,
                Qt::MouseButton, Qt::KeyboardModifiers)),
            this, SLOT(onWheel(QString, int, QPoint,
                Qt::MouseButton, Qt::KeyboardModifiers)));
    connect(this, SIGNAL(key(QString, int, QEvent::Type,
                Qt::KeyboardModifiers, QString)),
            this, SLOT(onKey(QString, int, QEvent::Type,
                Qt::KeyboardModifiers, QString)));
}

Pages* Pages::globalInstance() {
    return globalInstance_;
}

void Pages::onCreatePage(QString key) {
    if (!sender()) {
        return;
    }
    Page* page = new Page(key);
    pages_.insert(key, page);
}

void Pages::onDeletePage(QString key) {
    It it = pages_.find(key);
    if (it != pages_.end()) {
        Page* page = *it;
        page->deleteLater();
        pages_.erase(it);
    }
}

void Pages::onLoadInPage(QString key, QUrl url) {
    Page* page = pageOf(key);
    if (page) {
        page->mainFrame()->load(url);
    }
}

void Pages::onGoBack(QString key) {
    Page* page = pageOf(key);
    if (page) {
        QWebHistory* history = page->history();
        if (history->canGoBack()) {
            history->back();
        }
    }
}

void Pages::onRenderPage(QString key) {
    It it = pages_.find(key);
    if (it != pages_.end()) {
        Page* page = *it;
        page->renderPng();
    }
}

void Pages::onHtmlPage(QString key) {
    Page* page = pageOf(key);
    if (page) {
        page->onHtmlPage();
    }
}

void Pages::onUpdateInputs(QString key, QString values_json) {
    Page* page = pageOf(key);
    if (page) {
        page->onUpdateInputs(values_json);
    }
}

void Pages::onSetSize(QString key, QSize size) {
    Page* page = pageOf(key);
    if (page) {
        page->setViewportSize(size);
    }
}

void Pages::onMouse(QString key, QEvent::Type type,
        MOUSE_ARGS) {
    Page* page = pageOf(key);
    if (page) {
        QMouseEvent e(type, pos, button, button, modifiers);
        QApplication::sendEvent(page, &e);
    }
}

void Pages::onWheel(QString key, int delta, MOUSE_ARGS) {
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

void Pages::onKey(QString key, int k, QEvent::Type type,
        Qt::KeyboardModifiers modifiers, QString text) {
    if (k == Qt::Key_Control || k == Qt::Key_Shift ||
            k == Qt::Key_Alt) {
        return;
    }
    Page* page = pageOf(key);
    if (page) {
        if (text.isEmpty() && k <= UCHAR_MAX && isalpha(k)) {
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
