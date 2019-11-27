// Version: $Id: 511173d1a21a1405957a1bc672d60111e07fdeec $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsTagCloudLayout.h"
#include "dtkWidgetsTagCloudScope.h"
#include "dtkWidgetsTagCloudScopeItem.h"

class dtkWidgetsTagCloudScopePrivate
{
public:
    QStringList filters;
    QLineEdit *edit;
    QToolButton *clear;
    QCheckBox *switsh;

public:
    QList<dtkWidgetsTagCloudScopeItem *> tags;

public:
    QHash<QString, int> counts;

public:
    QCompleter *completer;
    QStringListModel *completer_model;

public:
    dtkWidgetsTagCloudLayout *layout;

public:
    bool light;
    bool doom;
};

dtkWidgetsTagCloudScope::dtkWidgetsTagCloudScope(QWidget *parent) : QFrame(parent)
{
    d = new dtkWidgetsTagCloudScopePrivate;

    d->light = true;
    d->doom = false;

    d->completer_model = new QStringListModel(this);

    d->completer = new QCompleter(this);
    d->completer->setModel(d->completer_model);
    d->completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);

    d->edit = new QLineEdit(this);
    d->edit->setFixedHeight(21);
    // d->edit->setAttribute(Qt::WA_MacShowFocusRect, false);
    d->edit->setCompleter(d->completer);

    d->clear = new QToolButton(this);
    d->clear->setFixedHeight(21);
    // d->clear->setAttribute(Qt::WA_MacShowFocusRect, false);

    d->switsh = new QCheckBox(this);
    // d->switsh->setAttribute(Qt::WA_MacShowFocusRect, false);

    QHBoxLayout *t_layout = new QHBoxLayout;
    t_layout->setContentsMargins(5, 5, 5, 5);
    // t_layout->setSpacing(0);
    t_layout->addWidget(d->edit);
    t_layout->addWidget(d->clear);
    t_layout->addWidget(d->switsh);

    d->layout = new dtkWidgetsTagCloudLayout;
    d->layout->setContentsMargins(5, 5, 5, 5);
    d->layout->setSpacing(0);

    QFrame *header = new QFrame(this);
    header->setObjectName("dtkWidgetsTagCloudScopeHeader");
    header->setLayout(t_layout);

    QFrame *well = new QFrame(this);
    well->setObjectName("dtkWidgetsTagCloudScopeWell");
    well->setLayout(d->layout);
    well->setMinimumHeight(50);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(header);
    layout->addWidget(well);

    this->setFrameShape(QFrame::NoFrame);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    connect(d->edit, SIGNAL(returnPressed()), this, SLOT(onTagAdded()));
    connect(d->clear, SIGNAL(clicked()), this, SIGNAL(cleared()));
    connect(d->switsh, SIGNAL(toggled(bool)), this, SIGNAL(unionMode(bool)));
}

dtkWidgetsTagCloudScope::~dtkWidgetsTagCloudScope(void)
{
    delete d->clear;
    delete d->edit;
    delete d;
}

QSize dtkWidgetsTagCloudScope::sizeHint(void) const
{
    return QSize(100, 24);
}

void dtkWidgetsTagCloudScope::clear(void)
{
    d->filters.clear();
    d->counts.clear();
    d->edit->clear();
}

void dtkWidgetsTagCloudScope::setDark(void)
{
    d->light = false;
}

void dtkWidgetsTagCloudScope::setDoom(void)
{
    d->doom = true;
}

void dtkWidgetsTagCloudScope::toggle(void)
{
    d->switsh->toggle();
}

void dtkWidgetsTagCloudScope::render(void)
{
    foreach (dtkWidgetsTagCloudScopeItem *tag, d->tags) {
        d->layout->removeWidget(tag);
    }

    qDeleteAll(d->tags); d->tags.clear();

    foreach (QString filter, d->filters) {
        dtkWidgetsTagCloudScopeItem *tag = new dtkWidgetsTagCloudScopeItem;

        if (!d->light)
            tag->setDark();

        if(d->doom)
            tag->setDoom();

        tag->setText(filter);

        if (d->counts.contains(filter))
            tag->setCount(d->counts[filter]);

        d->tags << tag;
        d->layout->addWidget(tag);

        connect(tag, SIGNAL(clicked()), this, SLOT(onTagRemoved()));
    }

    this->update();
}

void dtkWidgetsTagCloudScope::addTag(QString tag)
{
    d->filters << tag;
}

void dtkWidgetsTagCloudScope::addTag(QString tag, int count)
{
    d->filters << tag;

    d->counts[tag] = count;
}

void dtkWidgetsTagCloudScope::setTags(const QStringList& tags)
{
    QList<QString> t = tags;
    qSort(t.begin(), t.end(), qLess<QString>());

    d->completer_model->setStringList(t);
}

void dtkWidgetsTagCloudScope::onTagAdded(void)
{
    QString tag = d->edit->text();

    emit tagAdded(tag);

    d->edit->clear();
}

void dtkWidgetsTagCloudScope::onTagRemoved(void)
{
    dtkWidgetsTagCloudScopeItem *tag = dynamic_cast<dtkWidgetsTagCloudScopeItem *>(sender());

    if (!tag)
        return;

    QString value = tag->text();

    emit tagRemoved(value);
}

//
// dtkWidgetsTagCloudScope.cpp ends here
