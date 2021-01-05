
#pragma once

//#include <dtkWidgets/dtkWidgetsMainWindaow>
#include <QMainWindow>

class dtkVisualizationViewer : public QMainWindow
{
    Q_OBJECT

public:
    dtkVisualizationViewer(QWidget *parent = nullptr);
    ~dtkVisualizationViewer(void);

public slots:
    void importDataSet(const QString &path);
    void applyScale(double x, double y, double z);

signals:
    void updated(void);

protected:
    void setup(void);
    void setdw(void);

protected:
    void keyPressEvent(QKeyEvent *) override;

protected:
    void resizeEvent(QResizeEvent *) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    class dtkVisualizationViewerPrivate *d;
};

//
// dtkVisualizationViewer.h ends here
