#include <Wt/WApplication>

#include "util.hpp"

QString qsessionId() {
    return QString::fromUtf8(wApp->sessionId().c_str());
}

WString toWString(const QString& s) {
    return WString::fromUTF8((const char *)s.toUtf8());
}

QString toQString(const WString& s) {
    return QString::fromUtf8(s.toUTF8().c_str());
}

