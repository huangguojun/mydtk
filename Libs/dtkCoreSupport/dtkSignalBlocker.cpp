/* dtkSignalBlocker.cpp ---
 *
 * Author: John Stark
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun 24 16:21:08 2011 (+0200)
 * Version: $Id: a9b4ae5eafba3e5bbe4d56a2c41d81a29b7f69aa $
 * Last-Updated: Fri Jun 24 16:21:32 2011 (+0200)
 *           By: Julien Wintz
 *     Update #: 5
 */

/* Commentary:
 * This file only provides documentation - the implementation of the class
 * is in the header.
 */

/* Change log:
 *
 */

#include "dtkSignalBlocker.h"

// dtkSignalBlocker :

/*! \class      dtkSignalBlocker
 *  \brief      Blocks signals to a given object as long it remains
 * instantiated.
 *
 * The purpose is to increase exception-safety and to ensure that objects are
 * not still unintentionally blocked what ever the return path from a function.
 *
 * object->blockSignals(true) is called when the signalBlocker is created, and
 * object->blockSignals(false) is called when the signalBlocker is destroyed.
 * This ensures that a managed object is always unblocked, whatever the return
 * path.
 *
 * Code example :
 * \code
 *  {
 *    dtkSignalBlocker blocker(mySlider);
 *    // code which generates signals that mySlider needs to be protected from.
 *  }
 * \endcode
 */

/*! \fn         dtkSignalBlocker::dtkSignalBlocker(QObject * o)
    \brief      Constructor. Immediately calls o->blockSignals(true);
    \param      o : QObject that is blocked.
*/

/*! \fn         dtkSignalBlocker::~dtkSignalBlocker()
    \brief      Destructor. Calls o->blockSignals(false);
*/

/*! \fn         dtkSignalBlocker::blockSignals( bool v )
    \brief      Allows early unblocking / reblocking
    \param      v : passed to object->blockSignals(v).
*/
