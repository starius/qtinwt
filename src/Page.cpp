/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <Wt/WServer>

#include "Application.hpp"
#include "Page.hpp"
#include "SafeHtml.hpp"

#include <QtCore>

Page::Page(QString session_id):
    sessionId_(session_id.toStdString()),
    renderingPending_(false) {
    setViewportSize(QSize(640, 480));
    setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(this, SIGNAL(linkClicked(QUrl)),
            this, SLOT(loadInMainFrame(QUrl)));
    QWebFrame* frame = mainFrame();
    connect(frame, SIGNAL(titleChanged(QString)),
            this, SLOT(onTitleChanged(QString)));
    connect(frame, SIGNAL(urlChanged(QUrl)),
            this, SLOT(onUrlChanged(QUrl)));
}

void Page::post(const F& f) {
    WServer::instance()->post(sessionId_, f);
}

void Page::renderPng() {
    if (!renderingPending_) {
        renderingPending_ = true;
        QTimer::singleShot(REFRESH_MSEC / 2, this,
                SLOT(realRenderPng()));
    }
}

void Page::realRenderPng() {
    renderingPending_ = false;
    QByteArray ba;
    QImage image(viewportSize(),
                 QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    mainFrame()->render(&painter);
    painter.end();
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    post(boost::bind(&App::imageChanged, ba));
}

void Page::onHtmlPage() {
    QWebElement document = mainFrame()->documentElement();
    class2element_.clear();
    QString html = safeHtml(document, class2element_);
    post(boost::bind(&App::htmlChanged, html));
}

// get changed values as json {class: value}
void Page::onUpdateInputs(QString values_json) {
    Class2Value new_values;
    try
    {
        std::stringstream ss;
        QByteArray utf8 = values_json.toUtf8();
        ss.write(utf8.constData(), utf8.size());
        namespace Pt = boost::property_tree;
        Pt::ptree pt;
        Pt::read_json(ss, pt);
        BOOST_FOREACH(const Pt::ptree::value_type& v, pt) {
            QString clas = QString::fromUtf8(v.first.c_str());
            std::string val = v.second.data();
            QString value = QString::fromUtf8(val.c_str());
            new_values[clas] = value;
        }
    } catch (std::exception const& e) {
        qDebug() << e.what();
    }
    updateInputs(class2element_, new_values);
    class2element_.clear();
}

void Page::loadInMainFrame(QUrl url) {
    mainFrame()->load(url);
}

void Page::onTitleChanged(QString title) {
    post(boost::bind(&App::titleChanged, title));
}

void Page::onUrlChanged(QUrl url) {
    post(boost::bind(&App::urlChanged, url));
}
