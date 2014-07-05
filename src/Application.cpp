#include <boost/bind.hpp>
#include <Wt/WImage>
#include <Wt/WTimer>

#include "Application.hpp"
#include "Resource.hpp"
#include "Pages.hpp"
#include "Bridge.hpp"

using namespace Wt;

Application::Application(const WEnvironment& env):
    WQApplication(env, /* loop */ true) {
}

void Application::create() {
    bridge_ = new Bridge;
    bridge_->createP();
    bridge_->loadInP(QUrl("http://mail.ru/"));
    resource_ = new Resource;
    WImage* image = new WImage(root());
    image->resize(640, 480);
    image->setImageLink(resource_);
    WTimer* timer = new WTimer(this);
    timer->timeout().connect(resource_, &WResource::setChanged);
    timer->timeout().connect(boost::bind(&Bridge::renderP,
                bridge_));
    timer->setInterval(1000);
    timer->start();
}

void Application::destroy() {
    bridge_->deleteLater();
    delete resource_;
}

