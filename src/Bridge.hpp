#include <QtCore>

class Bridge : public QObject {
    Q_OBJECT;
public:
    Bridge();

    void createP();
    void deleteP();
    void loadInP(QUrl url);
    void renderP();

signals:
    void createPage(QString key);
    void deletePage(QString key);
    void loadInPage(QString key, QUrl url);
    void renderPage(QString key);
};

