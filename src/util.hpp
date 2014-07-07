#ifndef QIW_UTIL_HPP_
#define QIW_UTIL_HPP_

#include <boost/cast.hpp>
#include <Wt/WString>
#include <QtCore>

using namespace Wt;

#define D_CAST boost::polymorphic_downcast

QString qsessionId();

WString toWString(const QString& s);

QString toQString(const WString& s);

#endif

