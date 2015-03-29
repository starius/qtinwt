/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#ifndef QIW_SAFE_HTML_HPP_
#define QIW_SAFE_HTML_HPP_

#include <map>
#include <boost/function.hpp>
#include <QtWebKit>

typedef boost::function<bool(QString)> TagCheck;
typedef boost::function<bool(QString,
                             QString, QString&)> AttrCheck;
/* Some tags (input, select) get unique class
   value in returned HTML. Map maps from class name
   to QWebElement. */
typedef std::map<QString, QWebElement> Class2Element;
typedef std::map<QString, QString> Class2Value;

QString filterHtml(QWebElement element,
                   const TagCheck& tag_good,
                   const AttrCheck& attr_check,
                   Class2Element& class2element);

QString safeHtml(QWebElement element,
                 Class2Element& class2element);

void updateInputs(Class2Element& class2element,
                  const Class2Value& new_values);

#endif
