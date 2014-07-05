#include "boost-xtime.hpp"
#include <boost/thread.hpp>

#include "Application.hpp"
#include "Pages.hpp"

#include <QtGui>

using namespace Wt;

WApplication* createApp(const WEnvironment& e) {
    return new ::Application(e);
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Pages pages;
    boost::thread wt_thread(&WRun, argc, argv, &createApp);
    return a.exec();
}

