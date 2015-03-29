/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#ifndef QIW_APPLICATION_HPP_
#define QIW_APPLICATION_HPP_

#include <Wt/WGlobal>
#include <Wt/WApplication>
#include <QtCore>

using namespace Wt;

class Resource;
class Image;

#define qiwApp App::instance()

const int REFRESH_MSEC = 1000;

class App : public WApplication {
public:
    App(const WEnvironment& env);

    void initialize();
    void finalize();

    static App* instance();

    static void titleChanged(QString title);
    static void urlChanged(QUrl url);
    static void imageChanged(QByteArray image);
    static void htmlChanged(QString html);

    void setSize(int width, int height);

    void requestRendering();

    void requestHtml();

    void setHtmlMode(bool html);

    void changeMode();

private:
    QString sessionId_;
    JSignal<> timed_;
    JSignal<WString> inputs_updated_;
    Resource* resource_;
    WStackedWidget* stacked_;
    WPushButton* to_html_button_;
    WPushButton* to_img_button_;
    WLineEdit* address_;
    Image* image_;
    WScrollArea* html_scroll_;
    WText* html_;
    int timeout_;
    bool html_mode_;

    void navigate();
    void goBack();
    void mouseDown(const WMouseEvent& e);
    void mouseUp(const WMouseEvent& e);
    void mouseWheel(const WMouseEvent& e);
    void keyDown(const WKeyEvent& e);
    void keyUp(const WKeyEvent& e);
    void keyPressed(const WKeyEvent& e);
    void requestRenderingImpl();
    void updateInputs(const WString& values_json);
    void onTimeout();
    void setInterval();
};

#endif
