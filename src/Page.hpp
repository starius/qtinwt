#ifndef QIW_PAGE_HPP_
#define QIW_PAGE_HPP_

#include <boost/function.hpp>
#include <QtWebKit>

class Page : public QWebPage {
    Q_OBJECT;
public:
    Page(QString session_id);

    void renderPng();

    typedef boost::function<void()> F;

    void post(const F& f);

public slots:
    void loadInMainFrame(QUrl url);
    void onTitleChanged(QString title);
    void onUrlChanged(QUrl url);

private:
    std::string sessionId_;
};

#endif

