#ifndef SCANRESULTFORM_H
#define SCANRESULTFORM_H

#include <QWidget>
#include <QSortFilterProxyModel>

#include "ui/scanneddatamodel.h"

namespace Ui {
class ScanResultForm;
}

/*!
 * \brief The ScanResultForm class form with tree view of scanner result.
 */
class ScanResultForm : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief ScanResultForm Create empty model to tree view and sorting proxy model.
     * \param parent
     */
    explicit ScanResultForm(QWidget *parent = nullptr);
    ~ScanResultForm();
    /*!
     * \brief addScannedData add data to model (and it's shows in tree view)
     * \param res Scan result
     */
    void addScannedData(ScanResult res);
    /*!
     * \brief fixAll just send indexes referens from model to fixer and update view.
     */
    void fixAll();
    /*!
     * \brief fillTextBoxes slot for update translations
     */
    void fillTextBoxes();
private:
    Ui::ScanResultForm *ui;
    ScannedDataModel *model;
    QSortFilterProxyModel *proxyModel;
signals:
    /*!
     * \brief sendToFix just send indexes referens from model to fixer
     */
    void sendToFix(QList<ScanResult>&);
private slots:
    void on_pushButtonFix_clicked();
    void on_pushButtonClear_clicked();
};

#endif // SCANRESULTFORM_H
