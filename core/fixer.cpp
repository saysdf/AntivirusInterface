#include "fixer.h"

Fixer::Fixer(QObject *parent) : QObject(parent)
{

}

bool Fixer::fix(ScanResult& res)
{
    if (fix(res.file))
        res.type = Fixed;
    else
    {
        res.info = "NOT FIX! ERROR! "+res.info;
        res.type = Error;
        return false;
    }
    return true;
}

bool Fixer::fix(QFileInfo file)
{
    Q_UNUSED(file);
    return true;
}
