// Version: $Id: 2866713d53fc2de48cbf033117513e4b61e69835 $
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
// dtkGraphTemplateEdge implementation
// /////////////////////////////////////////////////////////////////

template < typename Node > inline dtkGraphTemplateEdge<Node>::dtkGraphTemplateEdge(const Node& source, const Node& destination) : m_source(source), m_destination(destination)
{
}

template < typename Node > inline dtkGraphTemplateEdge<Node>::dtkGraphTemplateEdge(const dtkGraphTemplateEdge& o) : m_source(o.m_source), m_destination(o.m_destination)
{
}

template < typename Node > inline dtkGraphTemplateEdge<Node>::~dtkGraphTemplateEdge(void)
{
}


template < typename Node > inline dtkGraphTemplateEdge<Node>& dtkGraphTemplateEdge<Node>::operator = (const dtkGraphTemplateEdge& o)
{
    m_source = o.m_source;
    m_destination = o.m_destination;

    return *this;
}

template < typename Node > inline bool dtkGraphTemplateEdge<Node>::operator == (const dtkGraphTemplateEdge& o) const
{
    return (m_source == o.m_source && m_destination == o.m_destination);
}

template < typename Node > inline const Node& dtkGraphTemplateEdge<Node>::source(void) const
{
    return m_source;
}

template < typename Node > inline const Node& dtkGraphTemplateEdge<Node>::destination(void) const
{
    return m_destination;
}

template < typename Node > inline void dtkGraphTemplateEdge<Node>::setSource(const Node& source)
{
    m_source = source;
}

template < typename Node > inline void dtkGraphTemplateEdge<Node>::setDestination(const Node& destination)
{
    m_destination = destination;
}

//
// dtkGraphTemplateEdge.tpp ends here
