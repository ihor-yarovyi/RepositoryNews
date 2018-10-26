#ifndef POPUPNOTIFIERWINDOW_H
#define POPUPNOTIFIERWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>
#include "rpnews/helpers/rpnews_types.h"

class PopUpNotifierWindow : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)
public:
    explicit PopUpNotifierWindow(QWidget* parent = nullptr);

public slots:
    void setPopUpText(const commit& commit, const std::string& name);
    void show();

private slots:
    void stopTimerSlot();
    void hideSlot();
    void pushButtonClickedSlot();

protected:
    void paintEvent(QPaintEvent* event);

private:
    void setPopupOpacity(double opacity);
    double getPopupOpacity() const;

private:
    QLabel m_LabelAuthor;
    QLabel m_LabelMessage;
    QLabel m_LabelDate;
    QLabel m_LabelRepositoryName;
    QPushButton m_Button;
    QGridLayout m_Layout;
    QPropertyAnimation m_Animation;
    double m_PopUpOpacity;
    QTimer* m_Timer;
};

#endif // POPUPNOTIFIERWINDOW_H