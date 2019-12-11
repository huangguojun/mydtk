#pragma once

#include <dtkVisualizationWidgetsExport>

#include <QtWidgets/QWidget>

class DTKVISUALIZATIONWIDGETS_EXPORT dtkVisualizationWidgetsSliceControls : public QWidget
{
    Q_OBJECT

public:
    dtkVisualizationWidgetsSliceControls(QWidget *parent = nullptr);
    ~dtkVisualizationWidgetsSliceControls(void);

public slots:
    void setValue(double value);
    void setChecked(bool);

public:
    double value(void);
    void setCheckState(Qt::CheckState state);
    bool isChecked(void);

signals:
    void valueChanged(double);
    void toggled(bool);
    void reset(void);

private:
    class dtkVisualizationWidgetsSliceControlsPrivate *d;
};
