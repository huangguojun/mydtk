// Version: $Id: 25f8d2ea762b2f3a061411fa78572a56b7e4d7a4 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkScreenMenu.h"

#include <QtCore>
#include <QtWidgets>

class dtkScreenMenuPrivate
{
public:
    int width;
    int height;
};

dtkScreenMenu::dtkScreenMenu(const QString &title, QWidget *parent)
    : QMenu(title, parent), d(new dtkScreenMenuPrivate)
{
    QAction *screenshot = this->addAction(QString("Take screenshot"));
    screenshot->setVisible(true);
    screenshot->setShortcut(Qt::Key_Print);

    connect(screenshot, SIGNAL(triggered(bool)), this, SLOT(takeScreenshot(void)));
}

QImage dtkScreenMenu::screenshot(QWidget *widget, qlonglong maxsize)
{
    QPixmap pixmap(widget->size());

    widget->render(&pixmap);

    if (maxsize > 0 && pixmap.width() > maxsize)
        return pixmap.scaledToWidth(maxsize).toImage();

    return pixmap.toImage();
}

void dtkScreenMenu::takeScreenshot(void)
{
    if (QWidget *widget = dynamic_cast<QWidget *>(parent())) {

        QString path = QDir::homePath();
        QString name =
                QString("%1 - Screenshot - %2")
                        .arg(qApp->applicationName())
                        .arg(QDateTime::currentDateTime().toString("MMMM dd yyyy - hh:mm:ss"));
        QString file = QDir::home().filePath(name);
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save screenshot"), file,
                                                        tr("Screenshot (*.png)"));

        if (fileName.isEmpty())
            return;

        QImage image = screenshot(widget);

        image.save(fileName, "PNG");
    }
}

//
// dtkScreenMenu.cpp ends here
