#include "scanneddatamodel.h"

ScannedDataModel::ScannedDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int ScannedDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return indexes.count();
}

int ScannedDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (int)hLast;
}

QVariant ScannedDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= indexes.size() || index.column() >= (int)hLast)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
        case hRow:
            return index.row()+1;
        case hFileName:
            return indexes.at(index.row()).file.fileName();
        case hFilePath:
            return indexes.at(index.row()).file.filePath();
        case hMalwareType:
            return Helper::getMalwareString(indexes.at(index.row()).type);
        }
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == hMalwareType)
        {
            switch (indexes.at(index.row()).type){
            case NoMalware:
                return QIcon(":/icons/icons/Info_48.png");
            case Fixed:
                return QIcon(":/icons/icons/Ok_48.png");
            case Error:
                return QIcon(":/icons/icons/Error_48.png");
            default:
                return QIcon(":/icons/icons/Warning_48.png");
            }
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        if (indexes.at(index.row()).info.isEmpty())
            return QString(indexes.at(index.row()).file.filePath());
        return QString(indexes.at(index.row()).file.filePath()+"\n"+indexes.at(index.row()).info);
    }
    return QVariant();
}

QVariant ScannedDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();
    if (orientation == Qt::Horizontal)
        switch (section) {
        case hRow:
            return tr("#");
        case hFileName:
            return tr("File name");
        case hFilePath:
            return tr("File path");
        case hMalwareType:
            return tr("Malware type");
        }
    else
        return QStringLiteral("#%1").arg(section);
    return QVariant();
}

void ScannedDataModel::append(ScanResult res)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    indexes.append(res);
    endInsertRows();
}

void ScannedDataModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    indexes.clear();
    endRemoveRows();
}
