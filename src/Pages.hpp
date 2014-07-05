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

    QByteArray imageOfPage(QString key) const;

public slots:
    void createPage(QString key);
    void deletePage(QString key);
    void loadInPage(QString key, QUrl url);
    void renderPage(QString key);
    void setSize(QString key, QSize size);
    void moused(QString key, QEvent::Type, QPoint,
        Qt::MouseButton, Qt::KeyboardModifiers);

private:
    struct PageA {
        Page* page_;
        QByteArray image_;
    };
    typedef QHash<QString, PageA> Hash;
    Hash pages_;
    typedef Hash::Iterator It;
    typedef Hash::const_iterator CIt;

    Page* pageOf(QString key) const;

    static Pages* globalInstance_;
};

#endif

