#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QStorageInfo>
#include <QMouseEvent>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTranslator>

#include "ui/scanresultform.h"
#include "ui/infowidget.h"
#include "global/helper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief MainWindow Set all windowflags, status and info widgets
     * create all widgets inside pages, create scan window,
     * connections, styles and init started state.
     * TODO All this can be divided into functions in case of their increase.
     * \param parent
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /*!
     * \brief arcaneMessage Arcane message from Scanner and set it to statis label
     * \param msg Message text
     * \param lvl text color 0-info (black); 1-warning orange(yellow); 2-error (red)
     */
    void arcaneMessage(QString msg,int lvl);
    /*!
     * \brief arcaneFilesCount How many files in this scan. This check is done before scanning.
     * \param count
     */
    void arcaneFilesCount(quint64 count);
    /*!
     * \brief arcaneScannedFilesCount How many files scanned NOW.
     * Calculate process percents.
     * \param count
     */
    void arcaneScannedFilesCount(quint64 count);
    /*!
     * \brief arcaneScanResult Arcane ScanResult and add it to DB of scanned files.
     * And show current scan file.
     * \param res
     */
    void arcaneScanResult(ScanResult res);
    /*!
     * \brief arcaneScanFinish End scan event.
     */
    void arcaneScanFinish();
    /*!
     * \brief arcaneVirusFoundedCount How many Malwares found NOW.
     * \param count
     */
    void arcaneVirusFoundedCount(quint64 count);
    /*!
     * \brief arcaneFixed End fix event.
     */
    void arcaneFixed();
private:
    Ui::MainWindow *ui;

    //Windows
    ScanResultForm *scanResultForm;
    //InfoWidgets Scan
    InfoWidget *startScanWidget;
    InfoWidget *pauseScanWidget;
    InfoWidget *stopScanWidget;
    InfoWidget *targetScanWidget;
    InfoWidget *showLogWidget;
    //InfoWidgets Update
    InfoWidget *startUpdateWidget;
    InfoWidget *stopUpdateWidget;
    //InfoWidgets Help
    InfoWidget *aboutQtWidget;
    InfoWidget *helpWidget;
    InfoWidget *emailWidget;
    //Tray
    QSystemTrayIcon *trayIcon;
    //Scan
    quint64 currentScanFilesCount;
    quint64 currentScanMalwaresCount;
    //State
    AppState currentAppState;
    QString appStateString;
    InfoWidget *statusWidget;
    QLabel *messageLabel;
    //Mouse
    QPoint mousePressPos;
    //Translation
    QTranslator translator; // contains the translations for this application
    QTranslator translatorQt; // contains the translations for qt
    QString currLang; // contains the currently loaded language
    QString langPath; // Path of language files. This is always fixed to /languages.

    /*!
     * \brief addInfoWidgetTo Add InfoWidget to StackWidget page.
     * \param target Target page
     * \param widget InfoWidget.
     */
    void addInfoWidgetTo(QWidget *target, InfoWidget *widget);
    /*!
     * \brief changeEvent Event for change language and minimize to tray.
     * \param e
     */
    void changeEvent(QEvent *e);
    /*!
     * \brief createTrayIcon Create tray icon and it's QMenu.
     */
    void createTrayIcon();
    /*!
     * \brief closeEvent Event for question "need to exit from app"
     * \param event
     */
    void closeEvent(QCloseEvent *event);
    /*!
     * \brief dragEnterEvent event for accept file drop form explorer
     * \param event
     */
    void dragEnterEvent(QDragEnterEvent *event);
    /*!
     * \brief dropEvent Scan dopped files.
     * \param event
     */
    void dropEvent(QDropEvent *event);
    /*!
     * \brief mouseMoveEvent Need for change XY of borderless window
     * \param event
     */
    void mouseMoveEvent(QMouseEvent *event);
    /*!
     * \brief mousePressEvent just keep mouse press pos.
     * \param event
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief changeAppState Change app state in statusWidget
     * \param state
     * \param message
     */
    void changeAppState(AppState state,QString message);
    /*!
     * \brief onTrayShowHide show/hide app if double click on tray icon
     * \param reason
     */
    void onTrayShowHide(QSystemTrayIcon::ActivationReason reason);
    /*!
     * \brief onShowHide change visible of main window
     */
    void onShowHide();

    /*!
     * \brief createScanWidgets create widgets and make connections in scan page
     */
    void createScanWidgets();
    void fillTextScanWidgets();
    void connectionsScanWidgets();
    /*!
     * \brief createUpdateWidgets create widgets and make connections in update page
     */
    void createUpdateWidgets();
    void fillTextUpdateWidgets();
    void connectionsUpdateWidgets();
    /*!
     * \brief createScanWindow create scan window and make connections
     */
    void createScanWindow();
    void fillTextScanWindow();
    /*!
     * \brief createSettingsWidgets do nothing. Maybe will change later
     */
    void createSettingsWidgets();
    /*!
     * \brief createHelpWidgets create widgets and make connections in help page
     */
    void createHelpWidgets();
    void fillTextHelpWidgets();
    void connectionsHelpWidgets();
    /*!
     * \brief fullScan start all disks Scan
     */
    void fullScan();
    /*!
     * \brief targetScan select files in QFileDialog and start scan they
     */
    void targetScan();

    void arcaneChangeLanguage(int index);
    void initTranslations();
    void changeLanguage(const QString &targetLang);
    void switchTranslator(QTranslator& translator, const QString& filename);
signals:
    /*!
     * \brief sendFilesForScan Send files or folders or both to scanner for scanning.
     */
    void sendFilesForScan(QStringList); //Can sand dirs and files
    /*!
     * \brief sendFileForScan Send file of folder for scanning
     */
    void sendFileForScan (QString); //Can sand dirs and files
    /*!
     * \brief sendCurrentScanedFile String with current scanned file.
     * it sends to some labels. To inform the user.
     */
    void sendCurrentScanedFile(QString);
    /*!
     * \brief sendMessage never used.
     */
    void sendMessage (QString);
    /*!
     * \brief sendPercent send String with "Progress = X%"
     */
    void sendPercent(QString);
    /*!
     * \brief sendScanFinish signal scan is finished
     */
    void sendScanFinish();
    /*!
     * \brief rejectScan signal to scanner for reject scan
     */
    void rejectScan ();
    /*!
     * \brief pauseScan signal to scanner for pause scan
     */
    void pauseScan ();
    /*!
     * \brief sendAutoFixFlag send fix flag from settings to handler
     */
    void sendAutoFixFlag (bool);
    /*!
     * \brief sendToFix send scan results from scan model to fixer for scan. (don't make a copy)
     */
    void sendToFix (QList<ScanResult>&);
private slots:
    void on_pushButtonHelpPageFixImmediately_clicked(bool clicked);
};
#endif // MAINWINDOW_H
