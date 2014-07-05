#ifndef QIW_BRIDGE_HPP_
#define QIW_BRIDGE_HPP_

#include <QtCore>

class Bridge : public QObject {
    Q_OBJECT;
public:
    Bridge();

    void createP();
    void deleteP();
    void loadInP(QUrl url);
    void renderP();
    void setS(QSize size);
    void click(QPoint xy);

signals:
    void createPage(QString key);
    void deletePage(QString key);
    void loadInPage(QString key, QUrl url);
    void renderPage(QString key);
    void setSize(QString key, QSize size);
    void clicked(QString key, QPoint xy);
};

#endif

