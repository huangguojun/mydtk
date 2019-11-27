/* dtkSearchField.mm ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Mar 21 23:47:10 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Mar 22 00:00:21 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 11
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkSearchField.h"

#import "Foundation/NSAutoreleasePool.h"
#import "Foundation/NSNotification.h"
#import "AppKit/NSSearchField.h"

#include <QtCore>
#include <QtWidgets>
#include <QtWidgets/QMacCocoaViewContainer>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

static inline NSString* fromQString(const QString &string)
{
    const QByteArray utf8 = string.toUtf8();
    const char* cString = utf8.constData();
    return [[NSString alloc] initWithUTF8String:cString];
}

static inline QString toQString(NSString *string)
{
    if (!string)
        return QString();
    return QString::fromUtf8([string UTF8String]);
}

static inline void setupLayout(void *cocoaView, QWidget *parent)
{
    parent->setAttribute(Qt::WA_NativeWindow);
    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->setMargin(0);
    layout->addWidget(new QMacCocoaViewContainer((NSView *)cocoaView, parent));
}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class dtkSearchFieldPrivate : public QObject
{
public:
    dtkSearchFieldPrivate(dtkSearchField *qSearchField, NSSearchField *nsSearchField)
        : QObject(qSearchField), qSearchField(qSearchField), nsSearchField(nsSearchField) {}

    void textDidChange(const QString &text)
    {
        if (qSearchField)
            emit qSearchField->textChanged(text);
    }

    void textDidEndEditing()
    {
        if (qSearchField)
            emit qSearchField->editingFinished();
    }

    QPointer<dtkSearchField> qSearchField;
    NSSearchField *nsSearchField;
};

@interface dtkSearchFieldDelegate : NSObject<NSTextFieldDelegate>
{
@public
    QPointer<dtkSearchFieldPrivate> d;
}
-(void)controlTextDidChange:(NSNotification*)notification;
-(void)controlTextDidEndEditing:(NSNotification*)notification;
@end

@implementation dtkSearchFieldDelegate
-(void)controlTextDidChange:(NSNotification*)notification {
    Q_ASSERT(d);
    if (d)
        d->textDidChange(toQString([[notification object] stringValue]));
}

-(void)controlTextDidEndEditing:(NSNotification*)notification {
    Q_UNUSED(notification);
    // No Q_ASSERT here as it is called on destruction.
    if (d)
        d->textDidEndEditing();
}
@end

dtkSearchField::dtkSearchField(QWidget *parent) : QWidget(parent)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSSearchField *search = [[NSSearchField alloc] init];

    dtkSearchFieldDelegate *delegate = [[dtkSearchFieldDelegate alloc] init];
    delegate->d = new dtkSearchFieldPrivate(this, search);
    [search setDelegate:delegate];

    setupLayout(search, this);

    setFixedHeight(24);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    [search release];

    [pool drain];
}

dtkSearchField::~dtkSearchField(void)
{
    delete d;

    d = NULL;
}

void dtkSearchField::setText(const QString &text)
{
    Q_ASSERT(d);
    if (!d)
        return;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [d->nsSearchField setStringValue:fromQString(text)];
    [pool drain];
}

void dtkSearchField::setPlaceholderText(const QString &text)
{
    Q_ASSERT(d);
    if (!d)
        return;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [[d->nsSearchField cell] setPlaceholderString:fromQString(text)];
    [pool drain];
}

void dtkSearchField::clear()
{
    Q_ASSERT(d);
    if (!d)
        return;

    [d->nsSearchField setStringValue:@""];
    emit textChanged(QString());
}

QString dtkSearchField::text() const
{
    Q_ASSERT(d);
    if (!d)
        return QString();

    return toQString([d->nsSearchField stringValue]);
}

void dtkSearchField::resizeEvent(QResizeEvent *resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
}
