// Version: $Id: 6b8a5842389ceed8febf26ccc4092f55190f0f44 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include <dtkGuiSupportExport.h>

#include <QtWidgets>

class dtkViewListPrivate;

class DTKGUISUPPORT_EXPORT dtkViewList : public QListWidget
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
