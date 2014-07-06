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

void Resource::handleRequest(const Http::Request& request,
                             Http::Response& response) {
    response.setMimeType("image/png");
    QByteArray ba = qiwApp->bridge()->image();
    response.out().write(ba.constData(), ba.size());
}

