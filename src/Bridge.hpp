#ifndef QIW_BRIDGE_HPP_
#define QIW_BRIDGE_HPP_

#include <boost/function.hpp>
#include <QtGui>

#include "mouse_args.hpp"

class Bridge : public QObject {
    Q_OBJECT;
public:
    Bridge();

    void createP();
    void deleteP();
    void loadInP(QUrl url);
    void renderP();
    void setS(QSize size);
    void mouse(QEvent::Type type, MOUSE_ARGS);
    void wheel(int delta, MOUSE_ARGS);

    typedef boost::function<void()> F;

    void post(const F& f);

signals:
    void createPage(QString key);
    void deletePage(QString key);
    void loadInPage(QString key, QUrl url);
    void renderPage(QString key);
    void setSize(QString key, QSize size);
    void moused(QString key, QEvent::Type, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);
    void wheeled(QString key, int, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);

public slots:
    void titleChanged(QString title);
    void urlChanged(QUrl url);

private:
    std::string sessionId_;
};

#endif

