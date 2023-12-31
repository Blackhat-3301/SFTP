#ifndef SFTPTEST_H
#define SFTPTEST_H

#include "parameters.h"

#include <qssh/sftpchannel.h>
#include <qssh/sshconnection.h>

#include <QElapsedTimer>
#include <QHash>
#include <QList>
#include <QObject>
#include <QSharedPointer>

QT_FORWARD_DECLARE_CLASS(QFile);

class SftpTest : public QObject
{
    Q_OBJECT
public:
    SftpTest(const Parameters &params);
    ~SftpTest();
    void run();

private slots:
    void handleConnected();
    void handleError();
    void handleDisconnected();
    void handleChannelInitialized();
    void handleChannelInitializationFailure(const QString &reason);
    void handleSftpJobFinished(QSsh::SftpJobId job, const QSsh::SftpError errorType, const QString &error);
    void handleFileInfo(QSsh::SftpJobId job, const QList<QSsh::SftpFileInfo> &fileInfoList);
    void handleChannelClosed();

private:
    typedef QHash<QSsh::SftpJobId, QString> JobMap;
    typedef QSharedPointer<QFile> FilePtr;
    enum State {
        Inactive, Connecting, InitializingChannel, UploadingSmall, DownloadingSmall,
        RemovingSmall, UploadingBig, DownloadingBig, RemovingBig, CreatingDir,
        CheckingDirAttributes, CheckingDirContents, RemovingDir, ChannelClosing, Disconnecting
    };

    void removeFile(const FilePtr &filePtr, bool remoteToo);
    void removeFiles(bool remoteToo);
    QString cmpFileName(const QString &localFileName) const;
    QString remoteFilePath(const QString &localFileName) const;
    void earlyDisconnectFromHost();
    bool checkJobId(QSsh::SftpJobId job, QSsh::SftpJobId expectedJob, const char *activity);
    bool handleJobFinished(QSsh::SftpJobId job, JobMap &jobMap,
        const QString &error, const char *activity);
    bool handleJobFinished(QSsh::SftpJobId job, QSsh::SftpJobId expectedJob, const QString &error,
        const char *activity);
    bool handleBigJobFinished(QSsh::SftpJobId job, QSsh::SftpJobId expectedJob,
        const QString &error, const char *activity);
    bool compareFiles(QFile *orig, QFile *copy);

    const Parameters m_parameters;
    State m_state;
    bool m_error;
    QSsh::SshConnection *m_connection;
    QSsh::SftpChannel::Ptr m_channel;
    QList<FilePtr> m_localSmallFiles;
    JobMap m_smallFilesUploadJobs;
    JobMap m_smallFilesDownloadJobs;
    JobMap m_smallFilesRemovalJobs;
    FilePtr m_localBigFile;
    QSsh::SftpJobId m_bigFileUploadJob;
    QSsh::SftpJobId m_bigFileDownloadJob;
    QSsh::SftpJobId m_bigFileRemovalJob;
    QSsh::SftpJobId m_mkdirJob;
    QSsh::SftpJobId m_statDirJob;
    QSsh::SftpJobId m_lsDirJob;
    QSsh::SftpJobId m_rmDirJob;
    QElapsedTimer m_bigJobTimer;
    QString m_remoteDirPath;
    QSsh::SftpFileInfo m_dirInfo;
    QList<QSsh::SftpFileInfo> m_dirContents;
};


#endif // SFTPTEST_H