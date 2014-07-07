#include <algorithm>

#include "Image.hpp"
#include "Application.hpp"

Image::Image() {
    setLayoutSizeAware(true);
}

void Image::layoutSizeChanged(int width, int height) {
    qiwApp->setSize(width, height);
}

