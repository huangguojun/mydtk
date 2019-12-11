#pragma once

#include <dtkVisualizationWidgetsExport>

#include <QtWidgets/QWidget>

// /////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsColorMapEditor
// /////////////////////////////////////////////////////////////////

class DTKVISUALIZATIONWIDGETS_EXPORT dtkVisualizationWidgetsColorMapEditor : public QWidget
{
    Q_OBJECT

public:
     dtkVisualizationWidgetsColorMapEditor(QWidget *parent = nullptr);
    ~dtkVisualizationWidgetsColorMapEditor(void);

    const QString& name(void) const;
    const QMap<double, QColor>& value(void) const;

    void setValue(const QMap<double, QColor>& value);
    void setValue(const QString& value);

protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

signals:
    void valueChanged(const QMap<double, QColor>& val);

public slots:
    void importColorMap(const QString& clut);

private:
    class dtkVisualizationWidgetsColorMapEditorPrivate *d;
};
