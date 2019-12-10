// Version: $Id$
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

#include <dtkVisualizationExport>

#include <QtCore/QObject>

#include <list>

class dtkVisualizationDecorator;
class dtkVisualizationCanvas;

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorCollection : public QObject
{
    Q_OBJECT

public:
    using list = std::list<dtkVisualizationDecorator *>;
    using iterator = typename list::iterator;
    using const_iterator = typename list::const_iterator;
    using reverse_iterator = typename list::reverse_iterator;
    using const_reverse_iterator = typename list::const_reverse_iterator;

public:
     dtkVisualizationDecoratorCollection(void);
     dtkVisualizationDecoratorCollection(const QList<dtkVisualizationDecorator *>&);
     dtkVisualizationDecoratorCollection(dtkVisualizationDecorator *);
     dtkVisualizationDecoratorCollection(const std::list<dtkVisualizationDecorator *>&);
     dtkVisualizationDecoratorCollection(const dtkVisualizationDecoratorCollection&);
     dtkVisualizationDecoratorCollection(dtkVisualizationDecoratorCollection&&);
    ~dtkVisualizationDecoratorCollection(void);

    bool add(dtkVisualizationDecorator *);
    bool remove(dtkVisualizationDecorator *);

    void clear(void);

    bool empty(void) const;
    std::size_t size(void) const;

    dtkVisualizationDecoratorCollection &operator << (dtkVisualizationDecorator *);
    dtkVisualizationDecoratorCollection &operator << (const QList<dtkVisualizationDecorator *>&);
    dtkVisualizationDecoratorCollection &operator << (const std::list<dtkVisualizationDecorator *>&);
    dtkVisualizationDecoratorCollection &operator << (const dtkVisualizationDecoratorCollection&);

          iterator  begin(void);
    const_iterator  begin(void) const;
    const_iterator cbegin(void) const;

          iterator  end(void);
    const_iterator  end(void) const;
    const_iterator cend(void) const;

          reverse_iterator  rbegin(void);
    const_reverse_iterator  rbegin(void) const;
    const_reverse_iterator crbegin(void) const;

          reverse_iterator  rend(void);
    const_reverse_iterator  rend(void) const;
    const_reverse_iterator crend(void) const;

public:
    void setCanvas(dtkVisualizationCanvas *);
    void unsetCanvas(void);

    QVariant data(void) const;
    dtkVisualizationCanvas *canvas(void) const;

public slots:
    void draw(void);

private:
    class dtkVisualizationDecoratorCollectionPrivate *d = nullptr;
};

//
// dtkVisualizationDecoratorCollection.h ends here
