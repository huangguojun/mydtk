/* dtkCorePluginFactory.cpp ---
 *
 * Author: Thibaud Kloczko
 * Created: jeu. avril  3 08:44:47 2014 (+0200)
 */

/*!
  \class dtkCorePluginFactory
  \inmodule dtkCore
  \brief The dtkCorePluginFactory class is a template class that provides a plugin factory for any type.
 */


/*! \fn dtkCorePluginFactory::dtkCorePluginFactory(void)
    Constructs the plugin factory.
 */

/*! \fn dtkCorePluginFactory::~dtkCorePluginFactory(void)
    Destroys the plugin factory
 */

/*!  \typedef dtkCorePluginFactory::creator
     Pattern defining creator functions provided by the plugin.
     The signature of such functions must be:
     \code T *(*creator) () \endcode
 */

/*! \fn void dtkCorePluginFactory::record(const QString& key, creator func)
    Registers the creator function \a func under key \a key.
 */

/*! \fn T *dtkCorePluginFactory::create(const QString& key) const
    Creates object from the creator function identified by \a key.

    Returns null pointer if \a key is not found.
 */

/*! \fn QStringList dtkCorePluginFactory::keys(void) const
    Returns all the recorded creator function keys.
 */
