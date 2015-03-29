/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

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

static bool isGoodTag(QString tag) {
    return good_tags_.contains(tag);
}

static bool isAttrGood(QString tag, QString attr,
                       QString& value) {
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
    foreach (QString attr, element.attributeNames()) {
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
    html = html.replace("<br>", "<br/>", Qt::CaseInsensitive);
    return html;
}

QString filterHtml(QWebElement element,
                   const TagCheck& tag_good,
                   const AttrCheck& attr_check) {
    QWebElement body = element.findFirst("body");
    QWebElement copy = body.clone();
    examineElement(copy, tag_good, attr_check);
    QString html = copy.toInnerXml();
    html = html.simplified();
    html = removeHtmlComments(html);
    html = closeTags(html);
    return html;
}

QString safeHtml(QWebElement element) {
    return filterHtml(element, isGoodTag, isAttrGood);
}
