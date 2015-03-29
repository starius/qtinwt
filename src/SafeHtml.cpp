/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#include <boost/foreach.hpp>

#include "SafeHtml.hpp"

typedef QSet<QString> TagSet;

TagSet good_tags_ = TagSet()
    << "html"
    << "body"
    << "div"
    << "span"
    << "form"
    << "input"
    << "select"
    << "option"
    << "optgroup"
    << "h1"
    << "h2"
    << "h3"
    << "h4"
    << "h5"
    << "h6"
    << "label"
    << "a"
    << "b"
    << "blockquote"
    << "br"
    << "cite"
    << "code"
    << "i"
    << "li"
    << "ol"
    << "p"
    << "pre"
    << "strong"
    << "sub"
    << "sup"
    << "table"
    << "th"
    << "tbody"
    << "tr"
    << "td"
    ;

TagSet self_closing_tags_ = TagSet()
    << "input"
    << "br"
    ;

static bool isGoodTag(QString tag) {
    return good_tags_.contains(tag);
}

static bool isAttrGood(QString tag, QString attr,
                       QString& value) {
    if (tag == "input" && attr == "type") {
        if (value == "image" || value == "submit") {
            return false;
        }
        return true;
    }
    if (tag == "input" && attr == "class") {
        return true;
    }
    return false;
}

void examineElement(QWebElement element,
                    const TagCheck& tag_good,
                    const AttrCheck& attr_check) {
    QString tag = element.tagName().toLower();
    if (!tag_good(tag)) {
        element.removeFromDocument();
        return;
    }
    BOOST_FOREACH (QString attr, element.attributeNames()) {
        QString value = element.attribute(attr);
        attr = attr.toLower();
        if (attr_check(tag, attr, value)) {
            element.setAttribute(attr, value);
        } else {
            element.removeAttribute(attr);
        }
    }
    QWebElement child = element.firstChild();
    while (!child.isNull()) {
        // preserve next element, because current
        // element can be removed by examineElement
        QWebElement next = child.nextSibling();
        examineElement(child, tag_good, attr_check);
        child = next;
    }
}

static QString removeHtmlComments(QString html) {
    while (true) {
        int start = html.indexOf("<!--");
        if (start == -1) {
            break;
        }
        int stop = html.indexOf("-->");
        if (stop == -1) {
            html.truncate(start);
            break;
        }
        int length = stop - start + 3; // 3 is length of "-->"
        html.remove(start, length);
    }
    return html;
}

static QString closeTags(QString html) {
    // TODO make toOuterXml return valid XML
    BOOST_FOREACH (QString tag, self_closing_tags_) {
        QRegExp pattern("<" + tag + "([^>]*)>",
                        Qt::CaseInsensitive);
        QString replacement("<" + tag +"\\1/>");
        html = html.replace(pattern, replacement);
    }
    return html;
}

static void buildMap(Class2Element& class2element,
                     QWebElement body, QWebElement copy) {
    QString tag_body = body.tagName().toLower();
    QString tag_copy = copy.tagName().toLower();
    if (tag_body == tag_copy) {
        if (tag_body == "input" || tag_body == "select") {
            int r = qrand();
            QString clas = "qtinwt" + QString::number(r);
            copy.addClass(clas);
            class2element[clas] = body;
        }
        QWebElement b = body.firstChild();
        QWebElement c = copy.firstChild();
        while (!b.isNull() && !c.isNull()) {
            buildMap(class2element, b, c);
            b = b.nextSibling();
            c = c.nextSibling();
        }
    }
}

QString filterHtml(QWebElement element,
                   const TagCheck& tag_good,
                   const AttrCheck& attr_check,
                   Class2Element& class2element) {
    QWebElement body = element.findFirst("body");
    QWebElement copy = body.clone();
    buildMap(class2element, body, copy);
    examineElement(copy, tag_good, attr_check);
    QString html = copy.toInnerXml();
    html = html.simplified();
    html = removeHtmlComments(html);
    html = closeTags(html);
    return html;
}

QString safeHtml(QWebElement element,
                 Class2Element& class2element) {
    return filterHtml(element, isGoodTag, isAttrGood,
                      class2element);
}

void updateInputs(Class2Element& class2element,
                  const Class2Value& new_values) {
    BOOST_FOREACH (Class2Element::value_type& cl_and_el,
                   class2element) {
        QString clas = cl_and_el.first;
        QWebElement element = cl_and_el.second;
        Class2Value::const_iterator it = new_values.find(clas);
        if (it != new_values.end()) {
            element.setAttribute("value", it->second);
        }
    }
}
