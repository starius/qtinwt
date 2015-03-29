/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#ifndef QIW_RESOURCE_HPP_
#define QIW_RESOURCE_HPP_

#include <Wt/WResource>
#include <QtCore>

using namespace Wt;

class Page;

class Resource : public WResource {
public:
    Resource();

    ~Resource();

    void handleRequest(const Http::Request& request,
                       Http::Response& response);

    QByteArray image() const;

    void setImage(QByteArray image);

private:
    Page* page_;
    QByteArray image_;
};

#endif
