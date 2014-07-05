#include "Application.hpp"

#include <QtGui>

using namespace Wt;

WApplication* createApp(const WEnvironment& e) {
    return new ::Application(e);
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    return WRun(argc, argv, &createApp);
}
