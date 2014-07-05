#ifndef QIW_IMAGE_HPP_
#define QIW_IMAGE_HPP_

#include <Wt/WImage>

using namespace Wt;

class Image : public WImage {
public:
    void layoutSizeChanged(int widht, int height);
};

#endif

