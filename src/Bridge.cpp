#include "Bridge.hpp"
#include "Pages.hpp"
#include "util.hpp"

Bridge::Bridge() {
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
    connect(this, SIGNAL(clicked(QString, QPoint)),
            PAGES, SLOT(clicked(QString, QPoint)));
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

void Bridge::click(QPoint xy) {
    emit clicked(qsessionId(), xy);
}

