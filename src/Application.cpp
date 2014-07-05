#include <Wt/WImage>
#include <Wt/WTimer>

#include "Application.hpp"
#include "Page.hpp"
#include "Resource.hpp"

using namespace Wt;

Application::Application(const WEnvironment& env):
    WQApplication(env, /* loop */ true) {
}

void Application::create() {
    page_ = new Page;
    page_->setViewportSize(QSize(640, 480));
    page_->mainFrame()->load(QUrl("http://mail.ru/"));
    resource_ = new Resource(page_);
    WImage* image = new WImage(root());
    image->resize(640, 480);
    image->setImageLink(resource_);
    WTimer* timer = new WTimer(this);
    timer->timeout().connect(resource_, &WResource::setChanged);
    timer->setInterval(1000);
    timer->start();
}

void Application::destroy() {
    //delete page_;
    //delete resource_;
}

