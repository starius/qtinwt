#include <algorithm>

#include "Image.hpp"
#include "Application.hpp"
#include "Bridge.hpp"

Image::Image() {
    setLayoutSizeAware(true);
}

void Image::layoutSizeChanged(int width, int height) {
    width = std::max(100, std::min(2000, width));
    height = std::max(100, std::min(2000, height));
    qiwApp->bridge()->setS(QSize(width, height));
    qiwApp->requestRendering();
}

