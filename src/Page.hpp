#include <QtWebKit>

class Page : public QWebPage {
    Q_OBJECT;
public:
    Page();

    void renderPng(QByteArray& ba) const;
};

