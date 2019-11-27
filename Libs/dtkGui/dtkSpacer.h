// Version: $Id: 132e8ff8e62ec03d06e54f9d9ff0059d822f57ff $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include <QWidget>

#include <dtkGuiSupportExport.h>

class DTKGUISUPPORT_EXPORT dtkSpacer : public QWidget
{
    Q_OBJECT

public:
    dtkSpacer(QWidget *parent = 0, int width = 0, int height = 0);
    ~dtkSpacer(void);

    QSize sizeHint (void) const;
};

//
// dtkSpacer.h ends here
