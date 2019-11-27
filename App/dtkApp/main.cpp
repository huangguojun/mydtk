
#include <dtkCore>
#include <dtkWidgets>


int main(int argc, char **argv)
{
    dtkApplication *application = dtkApplication::create(argc, argv);

    application->setApplicationName("dtkComposerEvaluator");
    application->setApplicationVersion("1.7.0");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    bool no_gui = application->noGui();

    return qApp->exec();
}
