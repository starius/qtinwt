#ifndef QIW_PAGES_HPP_
#define QIW_PAGES_HPP_

#include <QtGui>

#include "mouse_args.hpp"

class Page;

#define PAGES Pages::globalInstance()

class Pages : public QObject {
    Q_OBJECT;
public:
    Pages();

    static Pages* globalInstance();

    friend class App;

signals:
    void createPage(QString key);
    void deletePage(QString key);
    void loadInPage(QString key, QUrl url);
    void goBack(QString key);
    void renderPage(QString key);
    void setSize(QString key, QSize size);
    void mouse(QString key, QEvent::Type, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);
    void wheel(QString key, int, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);
    void key(QString key, int k, QEvent::Type type,
            Qt::KeyboardModifiers modifiers, QString text);

public slots:
    void onCreatePage(QString key);
    void onDeletePage(QString key);
    void onLoadInPage(QString key, QUrl url);
    void onGoBack(QString key);
    void onRenderPage(QString key);
    void onSetSize(QString key, QSize size);
    void onMouse(QString key, QEvent::Type, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);
    void onWheel(QString key, int, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);
    void onKey(QString key, int k, QEvent::Type type,
        Qt::KeyboardModifiers modifiers, QString text);

private:
    typedef QHash<QString, Page*> Hash;
    Hash pages_;
    typedef Hash::Iterator It;
    typedef Hash::const_iterator CIt;

    Page* pageOf(QString key) const;

    static Pages* globalInstance_;
};

#endif

