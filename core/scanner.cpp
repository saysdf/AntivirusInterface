#include "scanner.h"

Scanner::Scanner(QObject *parent) : QObject(parent), needStopThread(false), needPauseThread(false)
{
    randomizer = new QRandomGenerator(QRandomGenerator::system()->generate());
}

Scanner::~Scanner()
{
    if (future.isRunning())
    {
        needStopThread = true;
        future.waitForFinished();
    }
}

void Scanner::scanFilesRecursive(QStringList rootPathsList)
{
    rootPathsList.removeDuplicates();
    if (future.isRunning())
    {
        needStopThread = true;
        future.waitForFinished();
    }
    needStopThread = false;
    virusFoundedCount = 0;
    future = QtConcurrent::run(&Scanner::threadScanFilesRecursive,this,rootPathsList);
}

void  Scanner::scanFilesRecursive(QString rootPath)
{
    QStringList list;
    list << rootPath;
    scanFilesRecursive(list);
}

void Scanner::stopScan()
{
    needStopThread = true;
    if (needPauseThread) {
        pauseThreadMutex.unlock();
    }
    needPauseThread = false;
    future.waitForFinished();
}

void Scanner::pauseScan()
{
    if (future.isRunning())
    {
        needPauseThread = !needPauseThread;
        if (needPauseThread)
        {
            pauseThreadMutex.lock();
            emit sendMessage(tr("Scan is paused!"),0);
        }
        else
        {
            pauseThreadMutex.unlock();
            emit sendMessage(tr("Scan is unpaused!"),0);
        }
    }
}

Malwares Scanner::scanFile(const QString &path, QString &additionalInfo)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)){
        ///SCANNING!
        Malwares resMalw;
        //Random result.
        int acc = 100;
        int r = randomizer->bounded((Malwares::OtherWare-Malwares::Virus)*acc);
        resMalw = r % acc ? Malwares::NoMalware : Malwares((r+Malwares::Virus)/acc);
        switch (resMalw) {
            case Malwares::Adware:
            additionalInfo = tr("Adware often called advertising-supported software by its developers, "
                                "\nis software that generates revenue for its developer by automatically generating "
                                "\nonline advertisements in the user interface of the software or on a screen "
                                "\npresented to the user during the installation process.");
            break;
            case Malwares::Crimeware:
            additionalInfo = tr("Crimeware is a class of malware designed specifically to automate cybercrime.");
            break;
            case Malwares::Rootkit:
            additionalInfo = tr("Rootkit is a collection of computer software, typically malicious, "
                                "\ndesigned to enable access to a computer or an area of its "
                                "\nsoftware that is not otherwise allowed");
            break;
            case Malwares::Spyware:
            additionalInfo = tr("Spyware describes software with malicious behavior that aims to gather "
                                "\ninformation about a person or organization and send such information "
                                "\nto another entity in a way that harms the user");
            break;
            case Malwares::Trojan:
            additionalInfo = tr("Trojan horse (or simply trojan) is any malware which misleads users of its true intent.");
            break;
            case Malwares::Virus:
            additionalInfo = tr("Virus is a type of computer program that, when executed, replicates "
                                "\nitself by modifying other computer programs and inserting its own code.");
            break;
            case Malwares::Worm:
            additionalInfo = tr("Worm is a standalone malware computer program that "
                                "\nreplicates itself in order to spread to other computers");
            break;
            case Malwares::OtherWare:
            additionalInfo = tr("OtherWare check additional information in our wiki. www.CoffeeAntivirus.com/wiki (It's not real website)");
            break;
            default: break;
        }
        if (resMalw > Malwares::Error)
        {
            ++virusFoundedCount;
            emit sendVirusFounded(virusFoundedCount);
        }
        file.close();
        return resMalw;
    }
    else
    {
        additionalInfo = tr("Can't open file for read. No access.");
        return Malwares::Error;
    }
    additionalInfo = tr("No virus detected");
    return Malwares::NoMalware;
}

quint64 Scanner::checkFilesCount(QStringList rootPathsList)
{
    emit sendMessage(tr("Calculating files count..."),0);
    quint64 count = 0;
    for (const auto &path : rootPathsList)
    {
        if (QFileInfo(path).isFile())
        {
            ++count;
        }
        else
        {
            QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                it.next();
                ++count;
                if (needPauseThread && !needStopThread)
                {
                    pauseThreadMutex.lock();
                    pauseThreadMutex.unlock();
                }
                if (needStopThread)
                {
                    break;
                }
            }
        }
        if (needPauseThread && !needStopThread)
        {
            pauseThreadMutex.lock();
            pauseThreadMutex.unlock();
        }
        if (needStopThread)
        {
            emit sendMessage(tr("Scanning is rejected"),1);
            break;
        }
    }
    emit sendFilesCount(count);
    return count;
}

quint64 Scanner::threadScanFilesRecursive(QStringList rootPathsList)
{
    quint64 count = 0;
    checkFilesCount(rootPathsList);
    emit sendMessage(tr("Start scanning..."),0);
    for (const auto &path : rootPathsList)
    {
        if (needPauseThread && !needStopThread)
        {
            pauseThreadMutex.lock();
            pauseThreadMutex.unlock();
        }
        if (needStopThread)
        {
            emit sendMessage(tr("Scanning is rejected"),1);
            emit sendFinish();
            return count;
        }
        if (QFileInfo(path).isFile())
        {
            ScanResult result;
            result.type = scanFile(path,result.info);
            result.file = QFileInfo(path);
            ++count;
            emit sendScanResult(result);
            emit sendScanedCount(count);
        }
        else
        {
            QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                ScanResult result;
                result.type = scanFile(it.next(),result.info);
                result.file = it.fileInfo();
                ++count;
                emit sendScanResult(result);
                emit sendScanedCount(count);
                if (needPauseThread && !needStopThread)
                {
                    pauseThreadMutex.lock();
                    pauseThreadMutex.unlock();
                }
                if (needStopThread)
                {
                    break;
                }
            }
        }
    }
    emit sendMessage(tr("Scanning is over."),0);
    emit sendFinish();
    return count;
}
