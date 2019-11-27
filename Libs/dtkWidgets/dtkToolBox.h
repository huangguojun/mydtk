// Version: $Id: 8dd4802930facec66d78da59defe3a36ce77e037 $
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

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkToolBoxButton : public QAbstractButton
{
    Q_OBJECT

public:
    dtkToolBoxButton(QWidget *parent);

public:
    QSize sizeHint(void) const;
    QSize minimumSizeHint(void) const;

public:
    void setSelected(bool selected);
    bool isSelected(void) const;

protected:
    void initStyleOption(QStyleOptionToolBox *opt) const;
    void paintEvent(QPaintEvent *event);

private:
    bool m_selected;
};

inline void dtkToolBoxButton::setSelected(bool selected)
{
    m_selected = selected;
}

inline bool dtkToolBoxButton::isSelected(void) const
{
    return m_selected;
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class dtkToolBox;
class dtkToolBoxItemPrivate;
class DTKWIDGETS_EXPORT dtkToolBoxItem : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool expanded READ isExpanded WRITE setExpanded)
    Q_PROPERTY(bool enforced READ isEnforced WRITE setEnforced)
    Q_PROPERTY(QString  name READ       name WRITE     setName)

public:
    explicit dtkToolBoxItem(QWidget *parent = 0);
    ~dtkToolBoxItem(void);

public:
    bool isExpanded(void) const;
    bool isEnforced(void) const;
    QString    name(void) const;

public:
    void showButton(void);
    void hideButton(void);

public:
    void setWidget(QWidget *widget, const QString& text, const QIcon& icon = QIcon());

public slots:
    void setExpanded(bool expanded);
    void setEnforced(bool enforced);
    void setName(const QString& name);

public slots:
    void onButtonClicked(void);

public:
    static dtkToolBoxItem *fromObject(QObject *object, int hierarchy_level = -1);

public:
    void setToolBox(dtkToolBox *box);

private:
    dtkToolBoxItemPrivate *d;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class dtkToolBoxPrivate;
class DTKWIDGETS_EXPORT dtkToolBox : public QScrollArea
{
    Q_OBJECT
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(Order order READ order WRITE setOrder)
    Q_ENUMS(Order)
    Q_PROPERTY(DisplayMode displayMode READ displayMode WRITE setDisplayMode)
    Q_ENUMS(DisplayMode)

public:
    enum Order {
        Ascending,
        Descending,
        AlphaBetics
    };

public:
    enum DisplayMode {
        Default,
        AllItemExpanded,
        OneItemExpanded
    };

public:
    explicit dtkToolBox(QWidget *parent = 0);
    ~dtkToolBox(void);

public:
    void clear(void);

public:
    int count(void) const;
    Order order(void) const;
    DisplayMode displayMode(void) const;

public:
    dtkToolBoxItem *itemAt(int index) const;

public:
    void    addItem(dtkToolBoxItem *item);
    void insertItem(int index, dtkToolBoxItem *item);

public:
    void setCurrentItem(dtkToolBoxItem *item);

public:
    void removeItem(int index);
    void removeItem(dtkToolBoxItem *item);

public slots:
    void setOrder(Order order);
    void setDisplayMode(DisplayMode mode);

private:
    dtkToolBoxPrivate *d;
};

inline void dtkToolBox::addItem(dtkToolBoxItem *item)
{
    this->insertItem(this->count(), item);
}

//
// dtkToolBox.h ends here
