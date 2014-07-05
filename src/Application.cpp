#include <boost/bind.hpp>
#include <Wt/WTimer>
#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WLineEdit>
#include <QtGui>

#include "Application.hpp"
#include "Resource.hpp"
#include "Pages.hpp"
#include "Bridge.hpp"
#include "Image.hpp"
#include "util.hpp"

using namespace Wt;

App::App(const WEnvironment& env):
    WQApplication(env, /* loop */ true) {
    enableUpdates(true);
}

void App::create() {
    address_= new WLineEdit;
    address_->enterPressed().connect(this, &App::navigate);
    bridge_ = new Bridge;
    bridge_->createP();
    bridge_->loadInP(QUrl("http://mail.ru/"));
    resource_ = new Resource;
    image_ = new Image;
    image_->setImageLink(resource_);
    image_->mouseWentDown().connect(this, &App::mouseDown);
    image_->mouseWentUp().connect(this, &App::mouseUp);
    image_->mouseWheel().connect(this, &App::mouseWheel);
    WTimer* timer = new WTimer(this);
    timer->timeout().connect(resource_, &WResource::setChanged);
    timer->timeout().connect(boost::bind(&Bridge::renderP,
                bridge_));
    timer->setInterval(1000);
    timer->start();
    WVBoxLayout* layout = new WVBoxLayout;
    root()->setLayout(layout);
    layout->addWidget(address_);
    layout->addWidget(image_, 1);
}

void App::destroy() {
    bridge_->deleteP();
    bridge_->deleteLater();
    delete resource_;
}

App* App::instance() {
    return D_CAST<App*>(wApp);
}

Bridge* App::bridge() const {
    return bridge_;
}

void App::titleChanged(WString title) {
    wApp->setTitle(title);
    wApp->triggerUpdate();
}

void App::navigate() {
    QString url = toQString(address_->text());
    if (!url.contains("://")) {
        url = "http://" + url;
    }
    address_->setText(toWString(url));
    bridge_->loadInP(QUrl(url));
}

static QPoint cor2pos(const Coordinates& cor) {
    return QPoint(cor.x, cor.y);
}

static QPoint event2pos(const WMouseEvent& event) {
    return cor2pos(event.widget());
}

static Qt::MouseButton event2button(const WMouseEvent& event) {
    WMouseEvent::Button button = event.button();
    if (button == WMouseEvent::LeftButton) {
        return Qt::LeftButton;
    } else if (button == WMouseEvent::RightButton) {
        return Qt::RightButton;
    } else if (button == WMouseEvent::MiddleButton) {
        return Qt::MiddleButton;
    }
    return Qt::NoButton;
}

static Qt::KeyboardModifiers event2mod(const WMouseEvent& e) {
    WFlags<KeyboardModifier> md = e.modifiers();
    Qt::KeyboardModifiers result = Qt::NoModifier;
    if (md.testFlag(Wt::ShiftModifier)) {
        result |= Qt::ShiftModifier;
    }
    if (md.testFlag(Wt::ControlModifier)) {
        result |= Qt::ControlModifier;
    }
    if (md.testFlag(Wt::AltModifier)) {
        result |= Qt::AltModifier;
    }
    if (md.testFlag(Wt::MetaModifier)) {
        result |= Qt::MetaModifier;
    }
    return result;
}

static int event2wheel(const WMouseEvent& e) {
    return e.wheelDelta() * 8 * 15;
}

void App::mouseDown(const WMouseEvent& e) {
    QEvent::Type type = QEvent::MouseButtonPress;
    QPoint pos = event2pos(e);
    Qt::MouseButton button = event2button(e);
    Qt::KeyboardModifiers modifiers = event2mod(e);
    bridge_->mouse(type, MOUSE_NAMES);
}

void App::mouseUp(const WMouseEvent& e) {
    QEvent::Type type = QEvent::MouseButtonRelease;
    QPoint pos = event2pos(e);
    Qt::MouseButton button = event2button(e);
    Qt::KeyboardModifiers modifiers = event2mod(e);
    bridge_->mouse(type, MOUSE_NAMES);
}

void App::mouseWheel(const WMouseEvent& e) {
    QPoint pos = event2pos(e);
    int delta = event2wheel(e);
    Qt::MouseButton button = event2button(e);
    Qt::KeyboardModifiers modifiers = event2mod(e);
    bridge_->wheel(delta, MOUSE_NAMES);
}

