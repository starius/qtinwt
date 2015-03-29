/* qtinwt, QtWebKit wrapped in Wt application
 * Copyright (C) 2014-2015 Boris Nagaev
 * See the LICENSE file for terms of use
 */

#include "boost-xtime.hpp"
#include <boost/thread.hpp>

#include "Application.hpp"
#include "Pages.hpp"

#include <QtGui>

#ifdef __MINGW32__

#include <QtPlugin>
// http://permalink.gmane.org/gmane.comp.lib.qt.general/42520
Q_IMPORT_PLUGIN(qcncodecs)
Q_IMPORT_PLUGIN(qjpcodecs)
Q_IMPORT_PLUGIN(qtwcodecs)
Q_IMPORT_PLUGIN(qkrcodecs)

#endif // __MINGW32__

using namespace Wt;

WApplication* createApp(const WEnvironment& e) {
    return new App(e);
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    qRegisterMetaType<QEvent::Type>("QEvent::Type");
    qRegisterMetaType<Qt::MouseButton>("Qt::MouseButton");
    qRegisterMetaType<Qt::KeyboardModifiers>("Qt::"
            "KeyboardModifiers");
    Pages pages;
    boost::thread wt_thread(&WRun, argc, argv, &createApp);
    return a.exec();
}
