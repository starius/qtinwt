#include <Wt/WResource>

using namespace Wt;

class Page;

class Resource : public WResource {
public:
    Resource();

    ~Resource();

    void handleRequest(const Http::Request& request,
                       Http::Response& response);

private:
    Page* page_;
};

