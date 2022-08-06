#ifndef SCANNEDDATAMODEL_H
#define SCANNEDDATAMODEL_H

#include <QAbstractTableModel>
#include <QIcon>

#include "global/helper.h"

/*!
 * \brief The ScannedDataModel class Simple table model for show Scan Results.
 */
class ScannedDataModel : public QAbstractTableModel
{
public:
    ScannedDataModel(QObject *parent=nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    /*!
     * \brief append Adding Scan result to model
     * \param res Scan result. (it make copy here)
     */
    void append (ScanResult res);
    /*!
     * \brief clear just remove all Scan results from model.
     */
    void clear ();

    /*!
     * \brief indexes simple DB of scanned result
     */
    QList<ScanResult> indexes;
private:
    enum Headers {
        hRow,
        hFileName,
        hMalwareType,
        hFilePath,
        hLast
    };

};

#endif // SCANNEDDATAMODEL_H
