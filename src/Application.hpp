#include "WQApplication"

using namespace Wt;

class Page;
class Resource;

class Application : public WQApplication {
public:
    Application(const WEnvironment& env);

    void create();
    void destroy();

private:
    Page* page_;
    Resource* resource_;
};

