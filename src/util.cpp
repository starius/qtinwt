#include <Wt/WApplication>

#include "util.hpp"

QString qsessionId() {
    return QString::fromUtf8(wApp->sessionId().c_str());
}

