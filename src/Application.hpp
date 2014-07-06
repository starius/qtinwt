#ifndef QIW_APPLICATION_HPP_
#define QIW_APPLICATION_HPP_

#include <Wt/WGlobal>

#include "WQApplication"

using namespace Wt;

class Resource;
class Bridge;
class Image;

#define qiwApp App::instance()

class App : public WQApplication {
public:
    App(const WEnvironment& env);

    void create();
    void destroy();

    static App* instance();

    Bridge* bridge() const;

    static void titleChanged(WString title);
    static void urlChanged(WString url);

    void requestRendering();

private:
    Resource* resource_;
    Bridge* bridge_;
    WLineEdit* address_;
    WLineEdit* input_;
    Image* image_;

    void navigate();
    void mouseDown(const WMouseEvent& e);
    void mouseUp(const WMouseEvent& e);
    void mouseWheel(const WMouseEvent& e);
    void keyDown(const WKeyEvent& e);
    void keyUp(const WKeyEvent& e);
    void keyPressed(const WKeyEvent& e);
    void onTimeout();
};

#endif

