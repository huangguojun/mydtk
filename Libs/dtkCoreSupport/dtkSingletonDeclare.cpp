
#include "dtkSingletonDeclare.h"

/* Commentary:
 */

/**
 * \def DTK_DECLARE_SINGLETON( T )
 *
 * \brief    Declares a method instance() returning a pointer to the current
class.
 *
 * Adds friend classes to allow the constructor and destructor to be called even
if they are private.
 *
 * Code example :
 * { mySingleton.h }
 *
 * \code
    #include "dtkSingletoneDeclare.h"

    class MySingleton
    {
        // declares MySingleton::instance()
        DTK_DECLARE_SINGLETON( MySingleton );

        public:

        protected:
        // hide the constructor
        MySingleton() {};
        ~MySingleton() {};

        // class method example
        void doSomething() {...};

        private:
    };
 * \endcode
{ mySingleton.cpp }
 * \code
#include <dtkCore/dtkSingleton>

DTK_IMPLEMENT_SINGLETON( MySingleton );
 * \endcode
 *
 *  \sa       DTK_IMPLEMENT_SINGLETON( T )
 */
