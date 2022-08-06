#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QLayout>
#include <QLabel>
#include <QPushButton>

/*!
 * \brief The InfoWidget class
 * Widget for adding to MainWindow.
 * Simple styled widget has 2 labels 1 icon label and 1 button.
 */
class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = nullptr);
    enum InfoWidegtIcon {
        IconOk,
        IconWarning,
        IconError,
        IconStop,
        IconPlay,
        IconPause,
        IconInfo,
        IconLast
    };
    void setIcon(const InfoWidegtIcon ico);
    void setText(const QString &text);
    void setTitle(const QString &title);
    void setButtonText(const QString &text);

    void setVisibleButton(bool v);
    void setVisibleText(bool v);
    void setVisibleTitle(bool v);
    void setVisibleIcon(bool v);

    void setButtonCheckable(bool flag);
    void setButtonChecked(bool flag);
    void buttonCheck();
    void buttonUncheck();
private:
    QLabel* labelIcon;
    QLabel* labelInfo;
    QLabel* labelTitle;
    QPushButton* button;

signals:
    void sendButtonClick();
};

#endif // INFOWIDGET_H
