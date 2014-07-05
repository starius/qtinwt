#ifndef QIW_UTIL_HPP_
#define QIW_UTIL_HPP_

#include <boost/cast.hpp>
#include <QtCore>

QString qsessionId();

#define D_CAST boost::polymorphic_downcast

#endif

