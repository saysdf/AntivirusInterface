#ifndef HELPER_H
#define HELPER_H

#include <QFileInfo>
#include <QString>
#include <QIODevice>
#ifdef QT_DEBUG
#include <QDebug>
#endif



/*!
 * \brief The Malwares enum Just Easy way to create little DB of malwares.
 * TODO I think need to load some DB (From SQL or something else) with a lot of malwares and their descriptions.
 */
enum Malwares {
    NoMalware,
    Fixed,
    Error,
    Virus,
    Trojan,
    Rootkit,
    Worm,
    Spyware,
    Crimeware,
    Adware,
    OtherWare

};
/*!
 * \brief The ScanResult struct This struct sends from this unit to main window and other part of app.
 */
struct ScanResult {
    QFileInfo file;
    QString info;
    Malwares type = NoMalware;
};
/*!
 * \brief The AppState enum binflags of
 */
enum AppState {
    AllIsOk = 0,         //0000
    MalwareFound = 1,    //0001
    NeedUpdate = 2,      //0010
    UpdatingProcess = 4, //0100
    ScanProcess = 8      //1000
};
/*!
 * \brief The Helper class It has some global functions for all parts of app
 */
class Helper
{
public:
    Helper();
    /*!
     * \brief getMalwareString Get string from Malwares enum.
     * \param mlw Malwares enum
     * \return
     */
    static inline QString getMalwareString(Malwares mlw)
    {
        switch (mlw) {
        case NoMalware: return QString("NoMalware");
        case Virus:return QString("Virus");
        case Trojan:return QString("Trojan");
        case Rootkit:return QString("Rootkit");
        case Worm:return QString("Worm");
        case Spyware:return QString("Spyware");
        case Crimeware:return QString("Crimeware");
        case Adware:return QString("Adware");
        case OtherWare:return QString("OtherWare");
        case Fixed:return QString("Fixed");
        default : return QString("Error");
        }
    }
};

#endif // HELPER_H
