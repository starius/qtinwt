#include "Image.hpp"
#include "Application.hpp"
#include "Bridge.hpp"

void Image::layoutSizeChanged(int width, int height) {
    qiwApp->bridge()->setS(QSize(width, height));
}

