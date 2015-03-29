/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <Wt/WEnvironment>
#include <Wt/WStackedWidget>
#include <Wt/WScrollArea>
#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <QtGui>

#include "Application.hpp"
#include "Resource.hpp"
#include "Pages.hpp"
#include "Image.hpp"
#include "util.hpp"

using namespace Wt;

#define TO_S boost::lexical_cast<std::string>

App::App(const WEnvironment& env):
    WApplication(env),
    sessionId_(QString::fromUtf8(sessionId().c_str())),
    timed_(this, "timed"),
    resource_(0), html_mode_(false) {
}

void App::initialize() {
    if (!environment().ajax()) {
        new WText("Please enable JavaScript", root());
        return;
    }
    timed_.connect(this, &App::onTimeout);
    stacked_ = new WStackedWidget;
    html_scroll_ = new WScrollArea;
    html_ = new WText;
    html_scroll_->setWidget(html_);
    WPushButton* back = new WPushButton(" < ");
    back->clicked().connect(this, &App::goBack);
    mode_button_ = new WPushButton;
    mode_button_->clicked().connect(this, &App::changeMode);
    address_= new WLineEdit;
    input_= new WLineEdit;
    address_->enterPressed().connect(this, &App::navigate);
    emit PAGES->createPage(sessionId_);
    emit PAGES->loadInPage(sessionId_, QUrl("https://startpage.com/"));
    resource_ = new Resource;
    image_ = new Image;
    image_->setResource(resource_);
    image_->mouseWentDown().connect(this, &App::mouseDown);
    image_->mouseWentUp().connect(this, &App::mouseUp);
    image_->mouseWheel().connect(this, &App::mouseWheel);
    input_->keyPressed().connect(this, &App::keyPressed);
    globalKeyWentDown().connect(this, &App::keyDown);
    globalKeyWentUp().connect(this, &App::keyUp);
    WVBoxLayout* layout = new WVBoxLayout;
    layout->setContentsMargins(3, 0, 0, 0);
    root()->setLayout(layout);
    WHBoxLayout* top_layout = new WHBoxLayout;
    top_layout->setContentsMargins(0, 0, 0, 0);
    top_layout->addWidget(back);
    back->setMinimumSize(60, 20);
    mode_button_->setMinimumSize(60, 20);
    top_layout->addWidget(mode_button_);
    top_layout->addWidget(address_, 1);
    layout->addLayout(top_layout);
    layout->addWidget(stacked_, 1);
    layout->addWidget(input_);
    stacked_->addWidget(image_);
    stacked_->addWidget(html_scroll_);
    timeout_ = REFRESH_MSEC;
    onTimeout();
    setHtmlMode(false);
}

void App::finalize() {
    emit PAGES->deletePage(sessionId_);
    if (resource_) {
        delete resource_;
    }
}

App* App::instance() {
    return D_CAST<App*>(wApp);
}

void App::titleChanged(QString title) {
    wApp->setTitle(toWString(title));
}

void App::urlChanged(QUrl url) {
    qiwApp->address_->setText(toWString(url.toString()));
}

void App::imageChanged(QByteArray image) {
    App* app = qiwApp;
    if (image != app->resource_->image()) {
        app->resource_->setImage(image);
        app->timeout_ = REFRESH_MSEC;
        app->setInterval();
        app->resource_->setChanged();
    }
}

void App::htmlChanged(QString html) {
    App* app = qiwApp;
    app->html_->setText(toWString(html));
}

void App::setSize(int width, int height) {
    width = std::max(100, std::min(2000, width));
    height = std::max(100, std::min(2000, height));
    emit PAGES->setSize(sessionId_, QSize(width, height));
    requestRendering();
}

void App::requestRendering() {
    timeout_ = REFRESH_MSEC;
    setInterval();
    requestRenderingImpl();
}

void App::requestRenderingImpl() {
    emit PAGES->renderPage(sessionId_);
}

void App::onTimeout() {
    setInterval();
    timeout_ *= 2;
    requestRenderingImpl();
}

void App::setInterval() {
    doJavaScript("if (window.timeout_) {"
        "clearTimeout(window.timeout_); }"
        "window.timeout_ = setInterval(function(){" +
        timed_.createCall() + ";}," + TO_S(timeout_) + ");");
}

void App::navigate() {
    setHtmlMode(false);
    QString url = toQString(address_->text());
    if (!url.contains("://")) {
        url = "http://" + url;
    }
    address_->setText(toWString(url));
    emit PAGES->loadInPage(sessionId_, QUrl(url));
    requestRendering();
}

void App::goBack() {
    emit PAGES->goBack(sessionId_);
    requestRendering();
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
    emit PAGES->mouse(sessionId_, type, MOUSE_NAMES);
    requestRendering();
}

void App::mouseUp(const WMouseEvent& e) {
    QEvent::Type type = QEvent::MouseButtonRelease;
    QPoint pos = event2pos(e);
    Qt::MouseButton button = event2button(e);
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    emit PAGES->mouse(sessionId_, type, MOUSE_NAMES);
}

void App::mouseWheel(const WMouseEvent& e) {
    QPoint pos = event2pos(e);
    int delta = event2wheel(e);
    Qt::MouseButton button = event2button(e);
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    emit PAGES->wheel(sessionId_, delta, MOUSE_NAMES);
    requestRendering();
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
    emit PAGES->key(sessionId_, k, type, modifiers, "");
    requestRendering();
}

void App::keyUp(const WKeyEvent& e) {
    QEvent::Type type = QEvent::KeyRelease;
    int k = event2key(e);
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    emit PAGES->key(sessionId_, k, type, modifiers, "");
}

void App::keyPressed(const WKeyEvent& e) {
    int k = event2key(e);
    Qt::KeyboardModifiers modifiers = event2mod(e.modifiers());
    QString text = toQString(e.text());
    emit PAGES->key(sessionId_, k, QEvent::KeyPress,
            modifiers, text);
    emit PAGES->key(sessionId_, k, QEvent::KeyRelease,
            modifiers, text);
    requestRendering();
}

void App::setHtmlMode(bool html) {
    if (html == html_mode_) {
        return;
    }
    if (html) {
        mode_button_->setText("IMG");
        stacked_->setCurrentWidget(html_scroll_);
        requestHtml();
    } else {
        mode_button_->setText("HTML");
        stacked_->setCurrentWidget(image_);
        requestRendering();
    }
    html_mode_ = html;
}

void App::changeMode() {
    setHtmlMode(!html_mode_);
}

void App::requestHtml() {
    emit PAGES->htmlPage(sessionId_);
}
