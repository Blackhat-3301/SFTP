#include "argumentscollector.h"
#include "sftptest.h"

#include <qssh/sftpchannel.h>
#include <qssh/sshconnection.h>

#include <QCoreApplication>
#include <QObject>
#include <QStringList>

#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    bool parseSuccess;
    const Parameters parameters = ArgumentsCollector(app.arguments()).collect(parseSuccess);
    if (!parseSuccess)
        return EXIT_FAILURE;
    SftpTest sftpTest(parameters);
    sftpTest.run();
    return app.exec();
}