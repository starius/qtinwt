#include <Wt/Http/Response>

#include "Resource.hpp"
#include "Application.hpp"
#include "Bridge.hpp"
#include "util.hpp"

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
    QByteArray ba = qiwApp->bridge()->image();
    if (ba.isEmpty()) {
        response.setMimeType("image/gif");
        response.out().write(EMPTY_GIF, EMPTY_GIF_SIZE);
    } else {
        response.setMimeType("image/png");
        response.out().write(ba.constData(), ba.size());
    }
}

