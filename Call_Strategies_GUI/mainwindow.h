#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <QVector> // 引入 QVector
#include <QTimer>  // 引入 QTimer
#include <QLabel>   // 使用 QLabel
#include <QPixmap>  // 使用 QPixmap
#include <QPalette>
#include <QColor>

#include "strategyitem.h"
#include "audiomanager.h"
#include "strategymanager.h"
#include "inputmatchinglogic.h"
#include "animations.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class InputMatchingLogic;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots: // Qt特有的关键字，专门用于声明槽函数
    void clearInputSequence();
    void onMatchSuccess(const QString &strategyName);
    void onMatchFailed();
    void onInputAccepted(Direction dir); // 用来画箭头
    void delayclearAudio();

private:
    Ui::MainWindow *ui;

    void addArrow(Direction dir);
    QVector<QLabel*> m_arrowLabels;         // 记录界面上所有箭头QLabel的指针，为了能删除它们
    AudioManager *m_audioManager;
    StrategyManager *m_strategyManager;
    InputMatchingLogic *m_inputMatchingLogic;

    QTimer *m_delayClearAudio;
    bool m_isInputLocked;

    QMovie *m_strategyMovie;

};
#endif // MAINWINDOW_H
