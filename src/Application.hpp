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
    Resource* resource_;
    WStackedWidget* stacked_;
    WPushButton* mode_button_;
    WLineEdit* address_;
    WLineEdit* input_;
    Image* image_;
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
    void onTimeout();
    void setInterval();
};

#endif

