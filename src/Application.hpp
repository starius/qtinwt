#ifndef QIW_APPLICATION_HPP_
#define QIW_APPLICATION_HPP_

#include <Wt/WGlobal>

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
    WLineEdit* address_;
    Image* image_;

    void navigate();
    void clicked(const WMouseEvent& e);
};

#endif

