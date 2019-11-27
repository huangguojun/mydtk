// Version: $Id: 6b51c862dc2fbd9783cdc01188820b7107900dea $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkViewListPrivate;

class DTKWIDGETS_EXPORT dtkViewList : public QListWidget
{
    Q_OBJECT

public:
    dtkViewList(QWidget *parent = 0);
    ~dtkViewList(void);

public slots:
    void clear(void);

protected slots:
    void update(void);

protected:
    QMimeData *mimeData(const QList<QListWidgetItem *> items) const;
    QStringList mimeTypes(void) const;

private:
    dtkViewListPrivate *d;
};

//
// dtkViewList.h ends here
