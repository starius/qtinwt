#include "Pages.hpp"
#include "Page.hpp"

Pages* Pages::globalInstance_ = 0;

Pages::Pages() {
    globalInstance_ = this;
}

Pages* Pages::globalInstance() {
    return globalInstance_;
}

QByteArray Pages::imageOfPage(QString key) const {
    return pages_[key].image_;
}

void Pages::createPage(QString key) {
    PageA pa;
    pa.page_ = new Page;
    pages_.insert(key, pa);
}

void Pages::deletePage(QString key) {
    It it = pages_.find(key);
    if (it != pages_.end()) {
        it->page_->deleteLater();
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
        Page* page = it->page_;
        QByteArray& ba = it->image_;
        page->renderPng(ba);
    }
}

Page* Pages::pageOf(QString key) const {
    CIt it = pages_.find(key);
    if (it != pages_.end()) {
        return it->page_;
    } else {
        return 0;
    }
}

