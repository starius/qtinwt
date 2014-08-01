#ifndef QIW_SAFE_HTML_HPP_
#define QIW_SAFE_HTML_HPP_

#include <boost/function.hpp>
#include <QtWebKit>

typedef boost::function<bool(QString)> TagCheck;
typedef boost::function<bool(QString,
                             QString, QString&)> AttrCheck;

QString filterHtml(QWebElement element,
                   const TagCheck& tag_good,
                   const AttrCheck& attr_check);

QString safeHtml(QWebElement element);

#endif

