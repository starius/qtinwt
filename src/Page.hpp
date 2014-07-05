#ifndef QIW_PAGE_HPP_
#define QIW_PAGE_HPP_

#include <QtWebKit>

class Page : public QWebPage {
    Q_OBJECT;
public:
    Page();

    void renderPng(QByteArray& ba) const;
};

#endif

