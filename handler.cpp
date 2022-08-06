#include "handler.h"

Handler::Handler(QObject *parent) : QObject(parent),
    window(nullptr),
    scanner(nullptr),
    fixer(nullptr),
    fixFlag(false)
{

}

Handler::~Handler()
{
    if (window) window->deleteLater();
    if (scanner) scanner->deleteLater();
    if (fixer) fixer->deleteLater();
}

void Handler::startProgramm()
{
    QSplashScreen spl(QPixmap(":/icons/icons/Coffee.png"));
    spl.show();
    window = new MainWindow();
    scanner = new Scanner();
    fixer = new Fixer();
    if (!window || !scanner || !fixer)
    {
        Q_ASSERT("Can't create window or/and scanner.");
        return;
    }
    makeAllConnections();
    window->show();
    spl.hide();
}

void Handler::makeAllConnections()
{
    connect(window,&MainWindow::sendFilesForScan,   scanner,qOverload<QStringList>(&Scanner::scanFilesRecursive));
    connect(window,&MainWindow::sendFileForScan,    scanner,qOverload<QString>(&Scanner::scanFilesRecursive));
    connect(window,&MainWindow::rejectScan,         scanner,&Scanner::stopScan);
    connect(window,&MainWindow::pauseScan,          scanner,&Scanner::pauseScan);

    connect(scanner,&Scanner::sendFilesCount,   window,&MainWindow::arcaneFilesCount);
    connect(scanner,&Scanner::sendMessage,      window,&MainWindow::arcaneMessage);
    connect(scanner,&Scanner::sendScanedCount,  window,&MainWindow::arcaneScannedFilesCount);
    connect(scanner,&Scanner::sendFinish,       window,&MainWindow::arcaneScanFinish);
    connect(scanner,&Scanner::sendVirusFounded, window,&MainWindow::arcaneVirusFoundedCount);

    connect(scanner,&Scanner::sendScanResult,   this,&Handler::handlingScanResult);

    connect(window,&MainWindow::sendToFix,      this,&Handler::arcaneFilesFix);
}

void Handler::handlingScanResult(ScanResult res)
{
    if (fixFlag)
    {
        if (Malwares(res.type) > Malwares::Error)
            fixer->fix(res);
    }
    window->arcaneScanResult(res);
}

void Handler::arcaneFixFlag(bool f)
{
    fixFlag = f;
}

void Handler::arcaneFilesFix(QList<ScanResult> &resFiles)
{
    for (auto &i : resFiles)
    {
        if (Malwares(i.type) > Malwares::Error)
            fixer->fix((i));
    }
    window->arcaneFixed();
}
