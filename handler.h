#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QSplashScreen>

#include "ui/mainwindow.h"
#include "core/scanner.h"
#include "core/fixer.h"

/*!
 * \brief The Handler class Core class of application
 * It's Create all parts of app and connect they
 *
 */
class Handler : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Handler after constructor need call Start Programm
     * \param parent just parent QObject
     */
    explicit Handler(QObject *parent = nullptr);
    ~Handler();
    /*!
     * \brief startProgramm Generate Fixer Scanner and MainWindow modules
     */
    void startProgramm();
    /*!
     * \brief makeAllConnections Create All connections between all modules
     */
    void makeAllConnections();
    /*!
     * \brief handlingScanResult Arcane Scan result,
     * 1) if fixFlag = true Fix it
     * 2) send it to MainWindow
     */
    void handlingScanResult(ScanResult);
    /*!
     * \brief arcaneFixFlag Arcane fix flag from MainWindow
     * \param f Flag true = Auto Fix, false = no auto fix
     */
    void arcaneFixFlag(bool f);
    /*!
     * \brief arcaneFilesFix Arcane files for fix from MainWindow->ScannerResultForm->ScannerDataModel
     * \param resFiles Reference of results. They sends to fixer. And Fixer threir type to Fixed;
     */
    void arcaneFilesFix(QList<ScanResult> &resFiles);
private:
    MainWindow *window;
    Scanner *scanner;
    Fixer *fixer;
    bool fixFlag;
signals:

};

#endif // HANDLER_H
