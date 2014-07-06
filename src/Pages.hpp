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

public slots:
    void createPage(QString key);
    void deletePage(QString key);
    void loadInPage(QString key, QUrl url);
    void renderPage(QString key);
    void setSize(QString key, QSize size);
    void moused(QString key, QEvent::Type, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);
    void wheeled(QString key, int, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);
    void keyed(QString key, int k, QEvent::Type type,
        Qt::KeyboardModifiers modifiers, QString text);

private:
    Page* page_;
    typedef QHash<QString, Page*> Hash;
    Hash pages_;
    typedef Hash::Iterator It;
    typedef Hash::const_iterator CIt;

    Page* pageOf(QString key) const;

    static Pages* globalInstance_;
};

#endif

