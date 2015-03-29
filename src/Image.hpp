/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#ifndef QIW_IMAGE_HPP_
#define QIW_IMAGE_HPP_

#include <Wt/WImage>

using namespace Wt;

class Image : public WImage {
public:
    Image();

    void layoutSizeChanged(int widht, int height);
};

#endif
