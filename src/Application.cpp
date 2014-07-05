#include <boost/bind.hpp>
#include <Wt/WTimer>
#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>

#include "Application.hpp"
#include "Resource.hpp"
#include "Pages.hpp"
#include "Bridge.hpp"
#include "Image.hpp"
#include "util.hpp"

using namespace Wt;

Application::Application(const WEnvironment& env):
    WQApplication(env, /* loop */ true) {
}

void Application::create() {
    bridge_ = new Bridge;
    bridge_->createP();
    bridge_->loadInP(QUrl("http://mail.ru/"));
    resource_ = new Resource;
    image_ = new Image;
    image_->setImageLink(resource_);
    WTimer* timer = new WTimer(this);
    timer->timeout().connect(resource_, &WResource::setChanged);
    timer->timeout().connect(boost::bind(&Bridge::renderP,
                bridge_));
    timer->setInterval(1000);
    timer->start();
    WVBoxLayout* layout = new WVBoxLayout;
    root()->setLayout(layout);
    layout->addWidget(image_, 1);
}

void Application::destroy() {
    bridge_->deleteP();
    bridge_->deleteLater();
    delete resource_;
}

::Application* Application::instance() {
    return D_CAST<Application*>(wApp);
}

Bridge* Application::bridge() const {
    return bridge_;
}

