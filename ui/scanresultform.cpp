#include "scanresultform.h"
#include "ui_scanresultform.h"

ScanResultForm::ScanResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanResultForm)
{
    ui->setupUi(this);
    model = new ScannedDataModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->treeViewScan->setModel(proxyModel);
    ui->treeViewScan->setSortingEnabled(true);

}

ScanResultForm::~ScanResultForm()
{
    delete ui;
}

void ScanResultForm::addScannedData(ScanResult res)
{
    model->append(res);
}

void ScanResultForm::fixAll()
{
    emit sendToFix(model->indexes);
    emit model->dataChanged(model->index(0,0),model->index(model->rowCount(),model->columnCount()));
}

void ScanResultForm::fillTextBoxes()
{
    ui->retranslateUi(this);
    emit model->headerDataChanged(Qt::Horizontal,0,model->columnCount());
}


void ScanResultForm::on_pushButtonFix_clicked()
{
    fixAll();
}

void ScanResultForm::on_pushButtonClear_clicked()
{
    model->clear();
}
