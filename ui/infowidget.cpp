#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hBox = new QHBoxLayout(this);
    this->setLayout(hBox);
    QVBoxLayout *vBox = new QVBoxLayout(this);
    labelIcon  = new QLabel(this);
    hBox->addWidget(labelIcon);
    hBox->addLayout(vBox);
    labelInfo  = new QLabel(this);
    labelTitle = new QLabel(this);
    button     = new QPushButton(this);
    vBox->addWidget(labelTitle);
    vBox->addWidget(labelInfo);
    vBox->addWidget(button);

    labelIcon->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    labelTitle->setText("Title");
    labelInfo->setText("Info Text");
    QFont font("MS Shell Dlg 2",12);
    font.setBold(true);
    labelTitle->setFont(font);

    button->setText("Button Text");
    button->setFlat(true);

    setIcon(IconOk);

    connect(button,&QPushButton::clicked,this,&InfoWidget::sendButtonClick);
}

void InfoWidget::setIcon(InfoWidget::InfoWidegtIcon ico)
{
    QString colorText;
    QString colorButtonGradient;
    switch (ico) {
    case IconOk:
        labelIcon->setPixmap(QPixmap(":/icons/icons/Ok_48.png"));
        colorText = "#1C783B";
        colorButtonGradient = "#74CC92";
        break;
    case IconWarning:
        labelIcon->setPixmap(QPixmap(":/icons/icons/Warning_48.png"));
        colorText = "#F59829";
        colorButtonGradient = colorText;
        break;
    case IconError:
        labelIcon->setPixmap(QPixmap(":/icons/icons/Error_48.png"));
        colorText = "#DB797C";
        colorButtonGradient = colorText;
        break;
    case IconPlay:
        labelIcon->setPixmap(QPixmap(":/icons/icons/Play_48.png"));
        colorText = "#1C783B";
        colorButtonGradient = "#74CC92";
        break;
    case IconPause:
        labelIcon->setPixmap(QPixmap(":/icons/icons/Pause_48.png"));
        colorText = "#7EA0CF";
        colorButtonGradient = colorText;
        break;
    case IconInfo:
        labelIcon->setPixmap(QPixmap(":/icons/icons/Info_48.png"));
        colorText = "#7EA0CF";
        colorButtonGradient = colorText;
        break;
    case IconStop:
        labelIcon->setPixmap(QPixmap(":/icons/icons/Stop_48.png"));
        colorText = "#DB797C";
        colorButtonGradient = colorText;
        break;
    default:
        labelIcon->setPixmap(QPixmap(":/icons/icons/Warning_48.png"));
        colorText = "#F59829";
        colorButtonGradient = colorText;
        labelIcon->setText(tr("Somthing Wrong!"));
        break;
    }
    labelTitle->setStyleSheet("QLabel {color : "+colorText+"}");
    button->setStyleSheet("QPushButton {border: 1px solid #1C783B; "
                          "Text-align:left;"
                          "border-radius: 3px; background-color : "
                          "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                          "stop: 0 #f6f7fa, stop: 1 "+colorButtonGradient+");}"
                          "QPushButton:pressed {"
                          "background-color : "
                          "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                          "stop: 0 "+colorButtonGradient+", stop: 1 #f6f7fa);}"
                          "QPushButton:checked {"
                          "background-color : "
                          "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                          "stop: 0 "+colorButtonGradient+", stop: 1 #f6f7fa);}");
}

void InfoWidget::setText(const QString &text)
{
    labelInfo->setText(text);
}

void InfoWidget::setTitle(const QString &title)
{
    labelTitle->setText(title);
}

void InfoWidget::setButtonText(const QString &text)
{
    button->setText(text);
}

void InfoWidget::setVisibleButton(bool v)
{
    button->setVisible(v);
}

void InfoWidget::setVisibleText(bool v)
{
    labelInfo->setVisible(v);
}

void InfoWidget::setVisibleTitle(bool v)
{
    labelTitle->setVisible(v);
}

void InfoWidget::setVisibleIcon(bool v)
{
    labelIcon->setVisible(v);
}

void InfoWidget::setButtonCheckable(bool flag)
{
    button->setCheckable(flag);
}

void InfoWidget::setButtonChecked(bool flag)
{
    button->setChecked(flag);
}

void InfoWidget::buttonCheck()
{
    button->setChecked(true);
}

void InfoWidget::buttonUncheck()
{
    button->setChecked(false);
}


