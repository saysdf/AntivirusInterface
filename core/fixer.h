#ifndef FIXER_H
#define FIXER_H

#include <QObject>

#include "global/helper.h"

/*!
 * \brief The Fixer class Dumb fixer for test. just change malware type to Malwares::Fixed
 */
class Fixer : public QObject
{
    Q_OBJECT
public:
    explicit Fixer(QObject *parent = nullptr);
    bool fix(ScanResult& res);
    bool fix(QFileInfo file);
signals:

};

#endif // FIXER_H
