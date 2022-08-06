#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QStringList>
#include <QFile>
#include <QDirIterator>
#include <QRandomGenerator>
#include <QMutex>

#include <atomic>

#include <QtConcurrent/QtConcurrent>

#include "global/helper.h"

/*!
 * \brief The Scanner class
 * Dumb scanner that can check files and randomly return it's a Malware or not.
 *
 */
class Scanner : public QObject
{
    Q_OBJECT
public:
    explicit Scanner(QObject *parent = nullptr);
    virtual ~Scanner();
    /*!
     * \brief scanFilesRecursive Recursive antivirus scan.
     * \param rootPathsList List of paths for scan. It's useful for scan a different disks and list of dirs.
     * \return count of checked files (tryed to open it).
     */
    void scanFilesRecursive(QStringList rootPathsList);
    /*!
     * \brief scanFilesRecursive Recursive antivirus scan.
     * \param rootPath Path of upper dir or disk.
     * \return count of checked files (tryed to open it).
     */
    void scanFilesRecursive(QString rootPath);
    /*!
     * \brief stopScan Function for stop scanning. It change stop flag. And scanning in another thread will be stopped.
     */
    void stopScan();
    void pauseScan();
private:
    /*!
     * \brief scanFile Dumb scan file. Try to open it. And create random number. Any number divisible by precision without a remainder will be a virus.
     * \param path File Path
     * \param additionalInfo Dumb additional information about virus.
     * TODO need to get this information from DB.
     * \return type of Malwares:: . Malwares::Error if file can't be opened.
     */
    Malwares scanFile(const QString& path, QString& additionalInfo);
    /*!
     * \brief checkFilesCount return count of files.
     * \param rootPathsList
     * \return count of files for scan
     */
    quint64 checkFilesCount(QStringList rootPathsList);
    quint64 threadScanFilesRecursive(QStringList rootPathsList);
    QRandomGenerator* randomizer;
    QFuture<void> future;
    std::atomic_bool needStopThread;
    QMutex pauseThreadMutex;
    std::atomic_bool needPauseThread;
    quint64 virusFoundedCount;
signals:
    void sendScanResult(ScanResult);
    /*!
     * \brief sendMessage
     * \param Message
     * \param 0 - info, 2 - error, 3 - warning
     */
    void sendMessage(QString,int);
    void sendFinish();
    void sendScanedCount(quint64);
    void sendFilesCount(quint64);
    void sendVirusFounded(quint64);
};

#endif // SCANNER_H
