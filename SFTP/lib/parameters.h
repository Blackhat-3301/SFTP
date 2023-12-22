#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <qssh/sshconnection.h>

struct Parameters {
    QSsh::SshConnectionParameters sshParams;
    int smallFileCount;
    int bigFileSize;
    QString remotePath;
};

#endif // PARAMETERS_H