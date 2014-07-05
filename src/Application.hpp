#ifndef QIW_APPLICATION_HPP_
#define QIW_APPLICATION_HPP_

#include "WQApplication"

using namespace Wt;

class Resource;
class Bridge;

class Application : public WQApplication {
public:
    Application(const WEnvironment& env);

    void create();
    void destroy();

private:
    Resource* resource_;
    Bridge* bridge_;
};

#endif

