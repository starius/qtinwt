#include "Image.hpp"
#include "Application.hpp"
#include "Bridge.hpp"

Image::Image() {
    setLayoutSizeAware(true);
}

void Image::layoutSizeChanged(int width, int height) {
    qiwApp->bridge()->setS(QSize(width, height));
}

