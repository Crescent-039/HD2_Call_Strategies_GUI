#ifndef STRATEGYITEMWIDGET_H
#define STRATEGYITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include "strategyitem.h"
#include <algorithm>
#include <QGraphicsOpacityEffect>

namespace Ui { class StrategyItemWidget; }


class StrategyItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StrategyItemWidget(QWidget *parent = nullptr);
    // 在widget里让已经选择的战备可见
    void setStrategy(const StrategyItem& strategy);

    // 跟踪玩家输入序列
    void updateMatchingStatus(const QVector<Direction>& currentInput);

    // 把战备单元切换到已启动状态
    void showAsLaunching();

    // 重置面板状态，包括但不限于高亮的箭头以及正在启动这四个字
    void resetState();

    //让widget能返回自己的名字
    QString strategyName() const;

private:
    Ui::StrategyItemWidget *ui;

    // 用来记住自己代表的是哪个战备
    StrategyItem m_strategy;

    QVector<QLabel*> m_arrows;         // 记录界面上所有箭头QLabel的指针，为已经放进ArrowContainer的正在使用的箭头
    QVector<QLabel*> m_availableArrows;         // 存放空闲箭头label的仓库

    QLabel* m_launchingLabel;

signals:

};

#endif // STRATEGYITEMWIDGET_H
