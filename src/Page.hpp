/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

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
    void realRenderPng();
    void onHtmlPage();

private:
    std::string sessionId_;
    bool renderingPending_;
};

#endif
