/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#include <Wt/Http/Response>

#include "Application.hpp"
#include "Resource.hpp"

Resource::Resource() {
}

Resource::~Resource() {
    beingDeleted();
}

static const char EMPTY_GIF[] =
  { 0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x01, 0x00, 0x01, 0x00,
    0x80, 0x00, 0x00, 0xdb, 0xdf, 0xef, 0x00, 0x00, 0x00, 0x21,
    0xf9, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x02, 0x02, 0x44,
    0x01, 0x00, 0x3b };
const int EMPTY_GIF_SIZE = 43;

void Resource::handleRequest(const Http::Request& request,
                             Http::Response& response) {
    QByteArray ba = image();
    if (ba.isEmpty()) {
        response.setMimeType("image/gif");
        response.out().write(EMPTY_GIF, EMPTY_GIF_SIZE);
    } else {
        response.setMimeType("image/png");
        response.out().write(ba.constData(), ba.size());
    }
}

QByteArray Resource::image() const {
    return image_;
}

void Resource::setImage(QByteArray image) {
    image_ = image;
}
