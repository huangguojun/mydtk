// Version: $Id: d0483bb92c89f9e404aa1c547efbd71d6696b8ae $
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

// /////////////////////////////////////////////////////////////////
// dtkGraphTemplateEdge interface
// /////////////////////////////////////////////////////////////////

template<typename Node>
class dtkGraphTemplateEdge
{
public:
    dtkGraphTemplateEdge(void) = default;
    dtkGraphTemplateEdge(const Node &source, const Node &destination);
    dtkGraphTemplateEdge(const dtkGraphTemplateEdge &o);
    virtual ~dtkGraphTemplateEdge(void);

public:
    dtkGraphTemplateEdge &operator=(const dtkGraphTemplateEdge &o);

public:
    bool operator==(const dtkGraphTemplateEdge &o) const;

public:
    virtual const Node &source(void) const;
    virtual const Node &destination(void) const;

public:
    void setSource(const Node &source);
    void setDestination(const Node &destination);

protected:
    Node m_source;
    Node m_destination;
};

// /////////////////////////////////////////////////////////////////

#include "dtkGraphTemplateEdge.tpp"

//
// dtkGraphTemplateEdge.h ends here
