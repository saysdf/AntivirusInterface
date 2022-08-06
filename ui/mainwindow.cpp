#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),scanResultForm(nullptr)
{
    ui->setupUi(this);

    setAcceptDrops(true);
    setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    setWindowFlag(Qt::FramelessWindowHint,true);
    setWindowTitle(qAppName());

    messageLabel = new QLabel(ui->statusbar);
    ui->statusbar->addWidget(messageLabel);

    statusWidget = new InfoWidget(ui->pageHome);
    statusWidget->setVisibleButton(false);
    addInfoWidgetTo(ui->pageHome,statusWidget);

    createScanWindow();

    createScanWidgets();
    fillTextScanWidgets();
    connectionsScanWidgets();
    createUpdateWidgets();
    fillTextUpdateWidgets();
    connectionsUpdateWidgets();
    createHelpWidgets();
    fillTextHelpWidgets();
    connectionsHelpWidgets();

    createTrayIcon();

    changeAppState(AppState::AllIsOk,QString(tr("All protection modules is active.")));

    ui->pushButtonHomePage->setStyleSheet("Text-align:left");
    ui->pushButtonScanningPage->setStyleSheet("Text-align:left");
    ui->pushButtonUpdatePage->setStyleSheet("Text-align:left");
    ui->pushButtonSettingsPage->setStyleSheet("Text-align:left");
    ui->pushButtonHelpPage->setStyleSheet("Text-align:left");
    connect(ui->pushButtonHomePage,&QPushButton::clicked,       [&](){ui->stackedWidgetMain->setCurrentWidget(ui->pageHome);});
    connect(ui->pushButtonScanningPage,&QPushButton::clicked,   [&](){ui->stackedWidgetMain->setCurrentWidget(ui->pageScanning);});
    connect(ui->pushButtonUpdatePage,&QPushButton::clicked,     [&](){ui->stackedWidgetMain->setCurrentWidget(ui->pageUpdate);});
    connect(ui->pushButtonSettingsPage,&QPushButton::clicked,   [&](){ui->stackedWidgetMain->setCurrentWidget(ui->pageSettings);});
    connect(ui->pushButtonHelpPage,&QPushButton::clicked,       [&](){ui->stackedWidgetMain->setCurrentWidget(ui->pageHelp);});

    connect( ui->pushButtonClose, &QPushButton::clicked, this, &QWidget::close );
    connect( ui->pushButtonMinimize, &QPushButton::clicked, this, &MainWindow::onShowHide );

    connect(ui->comboBoxLanguageSelect, &QComboBox::currentIndexChanged, this, &MainWindow::arcaneChangeLanguage);
    ui->pushButtonHomePage->click();
    langPath = qApp->applicationDirPath()+"/language";
    initTranslations();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::arcaneMessage(QString msg, int lvl)
{
    if (lvl == 0) messageLabel->setStyleSheet("QLabel{color:black}");
    else if (lvl == 1) messageLabel->setStyleSheet("QLabel{color:#F59829}");
    else if (lvl == 2) messageLabel->setStyleSheet("QLabel{color:#DB797C}");
    messageLabel->setText(msg);
#ifdef QT_DEBUG
    qDebug()<<lvl<<msg;
#endif
}

void MainWindow::arcaneFilesCount(quint64 count)
{
    currentScanFilesCount = count;
#ifdef QT_DEBUG
    qDebug()<<"FILES COUNT ="<<count;
#endif
}

void MainWindow::arcaneScannedFilesCount(quint64 count)
{
    QString prstr=tr("Progress = ")+QString::number(int((float)count / ((float)currentScanFilesCount / 100.)))+"%";
    emit sendPercent(prstr);
    changeAppState(AppState(currentAppState | ScanProcess),QString(prstr));
#ifdef QT_DEBUG
    qDebug()<<"SCANNED FILES COUNT =" << count;
#endif
}

void MainWindow::arcaneScanResult(ScanResult res)
{
    scanResultForm->addScannedData(res);
    QString str = res.file.absoluteFilePath();
    int maxStr = 70;
    if(str.length()>maxStr)
    {
        QString fname = res.file.fileName();
        if (maxStr < fname.length()-3)
            str = str.left(3) + "..." +fname;
        else
            str = str.left(maxStr - fname.length()) + "..." +fname;
    }
    str = str.left(maxStr);
    emit sendCurrentScanedFile(str);
#ifdef QT_DEBUG
    qDebug()<<res.file<<res.info<<res.type;
#endif
}

void MainWindow::arcaneScanFinish()
{
    emit sendScanFinish();
    changeAppState(AppState(currentAppState ^ ScanProcess),QString("Scan is over."));
    emit sendCurrentScanedFile(tr("Checking files..."));
    emit sendPercent(tr("Calculation elapsed files..."));
}

void MainWindow::arcaneVirusFoundedCount(quint64 count)
{
    currentScanMalwaresCount = count;
    if (currentScanMalwaresCount > 0)
        changeAppState(AppState(currentAppState | MalwareFound),tr("Malwares count = ")+QString::number(currentScanMalwaresCount));
}

void MainWindow::arcaneFixed()
{
    changeAppState(AppState(currentAppState ^ MalwareFound),tr("Last malwares count = ")+QString::number(currentScanMalwaresCount));
}


void MainWindow::addInfoWidgetTo(QWidget *target, InfoWidget *widget)
{
    QLayoutItem *item = target->layout()->takeAt(target->layout()->count()-1);
    target->layout()->addWidget(widget);
    target->layout()->addItem(item);
}

void MainWindow::changeEvent(QEvent* e)
{
    switch (e->type())
    {
    case QEvent::LanguageChange:
        this->ui->retranslateUi(this);
        fillTextScanWindow();
        fillTextScanWidgets();
        fillTextUpdateWidgets();
        fillTextHelpWidgets();
        changeAppState(currentAppState,QString());
        break;
        // this event is send, if the system, language changes
    case QEvent::LocaleChange:
        changeLanguage(QLocale::languageToString(QLocale::system().language()));
        break;
    case QEvent::WindowStateChange:
    {
        if (this->windowState() & Qt::WindowMinimized)
        {
            this->hide();
        }
        break;
    }
    default:
        break;
    }

    QMainWindow::changeEvent(e);
}

void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/icons/Coffee.png"), this);
    connect( trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayShowHide );
    QAction *quitAction = new QAction(QIcon(":/icons/icons/Error_48.png"), tr("Exit"), trayIcon );
    connect( quitAction, &QAction::triggered, this, &QWidget::close );

    QAction *hideAction = new QAction(QIcon(":/icons/icons/Home.png"), tr("Show/Hide"), trayIcon );
    connect( hideAction, &QAction::triggered, this, &MainWindow::onShowHide );

    QMenu *trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( hideAction );
    trayIconMenu->addAction( quitAction );

    trayIcon->setContextMenu( trayIconMenu );
    trayIcon->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!QMessageBox::question(this,qAppName(),tr("Are you sure you want to close the application?"),tr("Close"),tr("Cansel")))
    {
        trayIcon->hide();
        emit rejectScan();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
        event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
      QStringList pathList;
      QList<QUrl> urlList = mimeData->urls();
      // extract the local paths of the files
      for (int i = 0; i < urlList.size() && i < 1000; ++i)
      {
        pathList.append(urlList.at(i).toLocalFile());
      }
      // send files to a function to scan the files
      emit sendFilesForScan(pathList);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (ui->labelTitle->geometry().contains(mousePressPos))
        {
            this->setGeometry(this->x() + (event->pos().x() - mousePressPos.x()),
                              this->y() + (event->pos().y() - mousePressPos.y()),
                              this->width(),this->height());
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mousePressPos = event->pos();
}

void MainWindow::changeAppState(AppState state, QString message)
{
    currentAppState = state;
    if (!message.isEmpty())
    {
        appStateString = message;
        trayIcon->setToolTip(message);
        statusWidget->setText(message);
    }
    if (state == AllIsOk)
    {
        statusWidget->setIcon(InfoWidget::IconOk);
        statusWidget->setTitle(tr("You are safe."));
    }
    else if (state & ScanProcess)
    {
        statusWidget->setIcon(InfoWidget::IconInfo);
        QString title = tr("Scanning...");
        if (state & MalwareFound)
            title +=" founded malwares count = "+QString::number(currentScanMalwaresCount);
        statusWidget->setTitle(title);
    }
    else
    {
        statusWidget->setIcon(InfoWidget::IconWarning);
        QString title = tr("Warning!");
        if (state & MalwareFound)
            title +=" founded malwares count = "+QString::number(currentScanMalwaresCount);
        statusWidget->setTitle(title);
    }
}

void MainWindow::onTrayShowHide(QSystemTrayIcon::ActivationReason reason )
{
    if( reason )
    {
        if( reason != QSystemTrayIcon::DoubleClick )
            return;
    }
    onShowHide();
}

void MainWindow::onShowHide()
{
    if( isVisible() )
    {
        trayIcon->showMessage(tr("Application in tray"),tr("Coffe Antivirus minimized to tray\n"
                                                           "Double click for show application."));
        hide();
    }
    else
    {
        showNormal();
        raise();
        setFocus();
    }
}

void MainWindow::createScanWidgets()
{
    startScanWidget  = new InfoWidget(ui->pageScanning);
    pauseScanWidget  = new InfoWidget(ui->pageScanning);
    stopScanWidget   = new InfoWidget(ui->pageScanning);
    targetScanWidget = new InfoWidget(ui->pageScanning);
    showLogWidget    = new InfoWidget(ui->pageScanning);

    addInfoWidgetTo(ui->pageScanning,startScanWidget);
    addInfoWidgetTo(ui->pageScanning,pauseScanWidget);
    addInfoWidgetTo(ui->pageScanning,stopScanWidget);
    addInfoWidgetTo(ui->pageScanning,targetScanWidget);
    addInfoWidgetTo(ui->pageScanning,showLogWidget);
}

void MainWindow::fillTextScanWidgets()
{
    startScanWidget->setIcon(InfoWidget::IconPlay);
    startScanWidget->setTitle(tr("Scan computer"));
    startScanWidget->setText(tr("Start full scan of computer, all local disks."));
    startScanWidget->setButtonText(tr("START"));
    startScanWidget->setToolTip(tr("Scan all logic disks."));

    pauseScanWidget->hide();
    pauseScanWidget->setIcon(InfoWidget::IconPause);
    pauseScanWidget->setTitle(tr("Pause full scan of computer."));
    pauseScanWidget->setText(tr("Checking files..."));
    pauseScanWidget->setButtonText(tr("PAUSE"));
    pauseScanWidget->setButtonCheckable(true);

    stopScanWidget->hide();
    stopScanWidget->setIcon(InfoWidget::IconStop);
    stopScanWidget->setTitle(tr("Stop full scan of computer."));
    stopScanWidget->setText(tr("Calculation elapsed files..."));
    stopScanWidget->setButtonText(tr("STOP"));

    targetScanWidget->setIcon(InfoWidget::IconPlay);
    targetScanWidget->setTitle(tr("Scan selected."));
    targetScanWidget->setText(tr("Scan selected files or drop files here."));
    targetScanWidget->setButtonText(tr("SELECT"));
    targetScanWidget->setToolTip(tr("Select files for scan or\nDrop they here."));

    showLogWidget->setIcon(InfoWidget::IconInfo);
    showLogWidget->setTitle(tr("Scan Log"));
    showLogWidget->setText(tr("Last/current scan information. All paths and finded malwares."));
    showLogWidget->setButtonText(tr("Open last scan information."));

}

void MainWindow::connectionsScanWidgets()
{

    connect(startScanWidget,&InfoWidget::sendButtonClick,this,&MainWindow::fullScan);
    connect(pauseScanWidget,&InfoWidget::sendButtonClick,this,&MainWindow::pauseScan);
    connect(stopScanWidget,&InfoWidget::sendButtonClick,this,&MainWindow::rejectScan);
    connect(startScanWidget,&InfoWidget::sendButtonClick,pauseScanWidget,&InfoWidget::buttonUncheck);
    connect(targetScanWidget,&InfoWidget::sendButtonClick,this,&MainWindow::targetScan);
    connect(this,&MainWindow::sendCurrentScanedFile,pauseScanWidget,&InfoWidget::setText);
    connect(this,&MainWindow::sendPercent,stopScanWidget,&InfoWidget::setText);

    connect(this,&MainWindow::sendFilesForScan,pauseScanWidget,&InfoWidget::show);
    connect(this,&MainWindow::sendFilesForScan,stopScanWidget,&InfoWidget::show);
    connect(this,&MainWindow::sendFilesForScan,startScanWidget,&InfoWidget::hide);
    connect(this,&MainWindow::sendFilesForScan,targetScanWidget,&InfoWidget::hide);
    connect(this,&MainWindow::sendScanFinish,pauseScanWidget,&InfoWidget::hide);
    connect(this,&MainWindow::sendScanFinish,stopScanWidget,&InfoWidget::hide);
    connect(this,&MainWindow::sendScanFinish,startScanWidget,&InfoWidget::show);
    connect(this,&MainWindow::sendScanFinish,targetScanWidget,&InfoWidget::show);

    connect(showLogWidget,&InfoWidget::sendButtonClick,scanResultForm,&QWidget::show);

}

void MainWindow::createUpdateWidgets()
{
    startUpdateWidget = new InfoWidget(ui->pageUpdate);
    stopUpdateWidget = new InfoWidget(ui->pageUpdate);
    addInfoWidgetTo(ui->pageUpdate,startUpdateWidget);
    addInfoWidgetTo(ui->pageUpdate,stopUpdateWidget);
}

void MainWindow::fillTextUpdateWidgets()
{
    startUpdateWidget->setIcon(InfoWidget::IconPlay);
    startUpdateWidget->setTitle(tr("Actual Version."));
    startUpdateWidget->setText(tr("Current version 0.0000.0001"));
    startUpdateWidget->setVisibleButton(false);
    startUpdateWidget->setButtonText(tr("START"));
    stopUpdateWidget->hide();
    stopUpdateWidget->setIcon(InfoWidget::IconStop);
    stopUpdateWidget->setTitle(tr("Updating..."));
    stopUpdateWidget->setText(tr("Please wait..."));
    stopUpdateWidget->setButtonText(tr("Cancel."));

}

void MainWindow::connectionsUpdateWidgets()
{
    connect(startUpdateWidget,&InfoWidget::sendButtonClick,startUpdateWidget,&InfoWidget::hide);
    connect(startUpdateWidget,&InfoWidget::sendButtonClick,stopUpdateWidget,&InfoWidget::show);

    connect(stopUpdateWidget,&InfoWidget::sendButtonClick,stopUpdateWidget,&InfoWidget::hide);
    connect(stopUpdateWidget,&InfoWidget::sendButtonClick,startUpdateWidget,&InfoWidget::show);
}

void MainWindow::createScanWindow()
{
    scanResultForm = new ScanResultForm(this);
    scanResultForm->setWindowFlags(Qt::Window);
    connect(scanResultForm,&ScanResultForm::sendToFix,this,&MainWindow::sendToFix);
}

void MainWindow::fillTextScanWindow()
{
    scanResultForm->fillTextBoxes();
}

void MainWindow::createSettingsWidgets()
{
    //TODO add settings here if it's needed
}

void MainWindow::createHelpWidgets()
{
    aboutQtWidget = new InfoWidget(ui->pageHelp);
    helpWidget    = new InfoWidget(ui->pageHelp);
    emailWidget   = new InfoWidget(ui->pageHelp);
    addInfoWidgetTo(ui->pageHelp,aboutQtWidget);
    addInfoWidgetTo(ui->pageHelp,helpWidget);
    addInfoWidgetTo(ui->pageHelp,emailWidget);
}

void MainWindow::fillTextHelpWidgets()
{
    aboutQtWidget->setIcon(InfoWidget::IconOk);
    aboutQtWidget->setTitle(tr("About Qt."));
    aboutQtWidget->setText(tr("Show information about Qt framework."));
    aboutQtWidget->setButtonText(tr("About."));
    helpWidget->setIcon(InfoWidget::IconInfo);
    helpWidget->setTitle(tr("Help."));
    helpWidget->setText(tr("Show documentation about Coffee Antivirus."));
    helpWidget->setButtonText(tr("Documentation."));
    emailWidget->setIcon(InfoWidget::IconWarning);
    emailWidget->setTitle(tr("Support."));
    emailWidget->setText(tr("Filippov@programmer.net"));
    emailWidget->setButtonText(tr("Write E-Mail."));
}

void MainWindow::connectionsHelpWidgets()
{
    connect(aboutQtWidget,&InfoWidget::sendButtonClick,&QApplication::aboutQt);
    connect(helpWidget,&InfoWidget::sendButtonClick,[](){
        QDesktopServices::openUrl(QUrl("file:///"+qApp->applicationDirPath()+"/help.pdf"));
    });
    connect(emailWidget,&InfoWidget::sendButtonClick,[](){
        QDesktopServices::openUrl(QUrl("mailto:filippov@programmer.net?subject=COFFEE ANTIVIRUS SUPPORT&body=Hi Oleg."));
    });
}

void MainWindow::fullScan()
{
    QStringList list;
    for (const QStorageInfo &storage: QStorageInfo::mountedVolumes())
    {
        list.push_back(storage.displayName());
    }
    changeAppState(AppState(currentAppState | ScanProcess),QString(tr("Scanning...")));
    emit sendFilesForScan(list);
}

void MainWindow::targetScan()
{
    changeAppState(AppState(currentAppState | ScanProcess),QString(tr("Scanning...")));
    emit sendFilesForScan(QFileDialog::getOpenFileNames(this,tr("Select files...")));
}

void MainWindow::arcaneChangeLanguage(int index)
{
    QString locale = ui->comboBoxLanguageSelect->itemData(index).toString();
    changeLanguage(locale);
}

void MainWindow::initTranslations()
{
    ui->comboBoxLanguageSelect->clear();
    QDir dir(langPath);
    QString defaultLocale = QLocale::system().name(); // e.g. "en_EN"
    QString defLang = QLocale::languageToString(QLocale(defaultLocale).language());
    QStringList fileNames = dir.entryList(QStringList("*.qm"));
    ui->comboBoxLanguageSelect->blockSignals(true);
    for (int i = 0; i < fileNames.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i]; // "en_EN.qm"
        locale.truncate(locale.lastIndexOf('.')); // "en_EN"
        locale.truncate(locale.lastIndexOf('_')); //en
        QString lang = QLocale::languageToString(QLocale(locale).language());
        ui->comboBoxLanguageSelect->addItem(lang,locale);
    }
    ui->comboBoxLanguageSelect->setCurrentText(defLang);
    ui->comboBoxLanguageSelect->blockSignals(false);
    arcaneChangeLanguage(ui->comboBoxLanguageSelect->currentIndex());
}

void MainWindow::changeLanguage(const QString &targetLang)
{

    if(currLang != targetLang) {
        currLang = targetLang;
        QLocale locale = QLocale(currLang);
        QLocale::setDefault(locale);
        switchTranslator(translator, QString("%1.qm").arg(locale.name()));
        switchTranslator(translatorQt, QString("qt_%1.qm").arg(locale.name()));
    }
}

void MainWindow::switchTranslator(QTranslator &translator, const QString &filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);
    // load the new translator
    QString path = langPath;
    if(translator.load(path + "/" + filename))
        qApp->installTranslator(&translator);
}

void MainWindow::on_pushButtonHelpPageFixImmediately_clicked(bool clicked)
{
    if (clicked)
        ui->pushButtonHelpPageFixImmediately->setIcon(QIcon(":/icons/icons/Ok_48.png"));
    else
        ui->pushButtonHelpPageFixImmediately->setIcon(QIcon(":/icons/icons/Error_48.png"));
    emit sendAutoFixFlag(clicked);
}
