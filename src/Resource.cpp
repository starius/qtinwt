#include <Wt/Http/Response>

#include "Resource.hpp"
#include "Page.hpp"

Resource::Resource(Page* page):
    page_(page) {
}

Resource::~Resource() {
    beingDeleted();
}

void Resource::handleRequest(const Http::Request& request,
                             Http::Response& response) {
    response.setMimeType("image/png");
    QSize size(640, 480);
    page_->setViewportSize(size);
    QImage image(size, QImage::Format_ARGB32);
    QPainter painter(&image);
    page_->mainFrame()->render(&painter);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    response.out().write(ba.constData(), ba.size());
}

