// Version: $Id: 8a7bcf83762687c25079358e156468b78923dab0 $
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

#include <dtkWidgetsExport.h>

#include <QtCore>
#include <QtWidgets>

class dtkWidgetsTagCloudScopePrivate;

class DTKWIDGETS_EXPORT dtkWidgetsTagCloudScope : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsTagCloudScope(QWidget *parent = 0);
    ~dtkWidgetsTagCloudScope(void);

    QSize sizeHint(void) const;

    void render(void);

    void addTag(QString tag);
    void addTag(QString tag, int count);

    void setTags(const QStringList &tags);

    void setDark(void);
    void setDoom(void);

signals:
    void tagSet(QString tag);
    void tagAdded(QString tag);
    void tagRemoved(QString tag);
    void cleared(void);
    void unionMode(bool);

public slots:
    void clear(void);

public slots:
    void toggle(void);

protected slots:
    void onTagAdded(void);
    void onTagRemoved(void);

protected:
    dtkWidgetsTagCloudScopePrivate *d;
};

//
// dtkWidgetsTagCloudScope.h ends here
