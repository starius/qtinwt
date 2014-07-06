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
    globalKeyWentDown().connect(this, &App::keyDown);
    globalKeyWentUp().connect(this, &App::keyUp);
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
}

void App::urlChanged(WString url) {
    qiwApp->address_->setText(url);
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

typedef WFlags<KeyboardModifier> Modifiers;

static Qt::KeyboardModifiers event2mod(Modifiers md) {
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
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    bridge_->mouse(type, MOUSE_NAMES);
}

void App::mouseUp(const WMouseEvent& e) {
    QEvent::Type type = QEvent::MouseButtonRelease;
    QPoint pos = event2pos(e);
    Qt::MouseButton button = event2button(e);
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    bridge_->mouse(type, MOUSE_NAMES);
}

void App::mouseWheel(const WMouseEvent& e) {
    QPoint pos = event2pos(e);
    int delta = event2wheel(e);
    Qt::MouseButton button = event2button(e);
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    bridge_->wheel(delta, MOUSE_NAMES);
}

typedef QHash<Wt::Key, Qt::Key> Key2Key;

static Key2Key keymap() {
    Key2Key k2k;
    k2k[Wt::Key_Enter] = Qt::Key_Enter;
    k2k[Wt::Key_Tab] = Qt::Key_Tab;
    k2k[Wt::Key_Backspace] = Qt::Key_Backspace;
    k2k[Wt::Key_Shift] = Qt::Key_Shift;
    k2k[Wt::Key_Control] = Qt::Key_Control;
    k2k[Wt::Key_Alt] = Qt::Key_Alt;
    k2k[Wt::Key_PageUp] = Qt::Key_PageUp;
    k2k[Wt::Key_PageDown] = Qt::Key_PageDown;
    k2k[Wt::Key_End] = Qt::Key_End;
    k2k[Wt::Key_Home] = Qt::Key_Home;
    k2k[Wt::Key_Left] = Qt::Key_Left;
    k2k[Wt::Key_Up] = Qt::Key_Up;
    k2k[Wt::Key_Right] = Qt::Key_Right;
    k2k[Wt::Key_Down] = Qt::Key_Down;
    k2k[Wt::Key_Insert] = Qt::Key_Insert;
    k2k[Wt::Key_Delete] = Qt::Key_Delete;
    k2k[Wt::Key_Escape] = Qt::Key_Escape;
    k2k[Wt::Key_F1] = Qt::Key_F1;
    k2k[Wt::Key_F2] = Qt::Key_F2;
    k2k[Wt::Key_F3] = Qt::Key_F3;
    k2k[Wt::Key_F4] = Qt::Key_F4;
    k2k[Wt::Key_F5] = Qt::Key_F5;
    k2k[Wt::Key_F6] = Qt::Key_F6;
    k2k[Wt::Key_F7] = Qt::Key_F7;
    k2k[Wt::Key_F8] = Qt::Key_F8;
    k2k[Wt::Key_F9] = Qt::Key_F9;
    k2k[Wt::Key_F10] = Qt::Key_F10;
    k2k[Wt::Key_F11] = Qt::Key_F11;
    k2k[Wt::Key_F12] = Qt::Key_F12;
    k2k[Wt::Key_Space] = Qt::Key_Space;
    for (int c = 'A'; c <= 'Z'; c++) {
        k2k[Wt::Key(c)] = Qt::Key(c);
    }
    return k2k;
}

Key2Key k2k_ = keymap();

static int event2key(const WKeyEvent& e) {
    if (e.key() == Wt::Key_unknown) {
        // http://redmine.webtoolkit.eu/issues/3435
        return e.keyCode();
    } else {
        return k2k_.value(e.key(), Qt::Key_Escape);
    }
}

void App::keyDown(const WKeyEvent& e) {
    QEvent::Type type = QEvent::KeyPress;
    int k = event2key(e);
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    bridge_->keye(k, type, modifiers);
}

void App::keyUp(const WKeyEvent& e) {
    QEvent::Type type = QEvent::KeyRelease;
    int k = event2key(e);
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    bridge_->keye(k, type, modifiers);
}

