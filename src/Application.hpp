#ifndef QIW_APPLICATION_HPP_
#define QIW_APPLICATION_HPP_

#include "WQApplication"

using namespace Wt;

class Resource;
class Bridge;
class Image;

#define qiwApp Application::instance()

class Application : public WQApplication {
public:
    Application(const WEnvironment& env);

    void create();
    void destroy();

    static Application* instance();

    Bridge* bridge() const;

private:
    Resource* resource_;
    Bridge* bridge_;
    Image* image_;
};

#endif

