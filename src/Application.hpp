#ifndef QIW_APPLICATION_HPP_
#define QIW_APPLICATION_HPP_

#include "WQApplication"

using namespace Wt;

class Resource;
class Bridge;
class Image;

#define qiwApp App::instance()

class App : public WQApplication {
public:
    App(const WEnvironment& env);

    void create();
    void destroy();

    static App* instance();

    Bridge* bridge() const;

private:
    Resource* resource_;
    Bridge* bridge_;
    Image* image_;
};

#endif

