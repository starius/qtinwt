/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#include <algorithm>

#include "Image.hpp"
#include "Application.hpp"

Image::Image() {
    setLayoutSizeAware(true);
}

void Image::layoutSizeChanged(int width, int height) {
    qiwApp->setSize(width, height);
}
