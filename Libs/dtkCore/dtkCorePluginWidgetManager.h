// Version: $Id: 21a66c6f2a66b42715ce67a3eae17eb2b446974e $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkCoreExport>

#include <QtCore>

class QWidget;

class DTKCORE_EXPORT dtkCorePluginWidgetManager : public QObject
{
    Q_OBJECT

public:
    static dtkCorePluginWidgetManager &instance(void);

public:
    void add(const void *, const QVariant &, QWidget *);

    int remove(const void *);
    int remove(QWidget *);

    void clear(void);

    QWidget *widget(const void *) const;
    QWidget *take(const void *) const;

    QString description(void) const;

signals:
    void added(const QVariant &, QWidget *) const;

protected:
    static dtkCorePluginWidgetManager *s_instance;

private:
    dtkCorePluginWidgetManager(void);
    ~dtkCorePluginWidgetManager(void);

private:
    class dtkCorePluginWidgetManagerPrivate *d;
};

DTKCORE_EXPORT QDebug operator<<(QDebug, dtkCorePluginWidgetManager &);

//
// dtkCorePluginWidgetManager.h ends here
