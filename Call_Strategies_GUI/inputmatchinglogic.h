#ifndef INPUTMATCHINGLOGIC_H
#define INPUTMATCHINGLOGIC_H

#include <QObject>
#include <QString>
#include <QVector> // 引入 QVector
#include <QTimer>
#include "strategyitem.h"

// 前向声明，避免循环引用
class StrategyManager;

class InputMatchingLogic : public QObject
{
    Q_OBJECT
public:
    // 构造函数需要传入 StrategyManager 的地址
    explicit InputMatchingLogic(StrategyManager *strategyManager, QObject *parent = nullptr);

    void clearSequence();
    // 公共的槽，用来接收 MainWindow 的按键通知
    void onKeyPressed(Direction dir);

public slots:



signals:
    // 广播系统，向 MainWindow 报告事件
    void inputAccepted(Direction dir);
    void matchSuccess(const QString &strategyName); // const & 提高效率
    void matchFailed();
    void sequenceCleared();
    void sequenceUpdated(const QVector<Direction>& currentSequence);

private slots:
    // 私有的槽，用来响应计时器
    void onInputTimeout();

private:
    // 检查序列匹配的的核心逻辑
    void checkSequenceMatch();

    StrategyManager *m_strategyManager; // 指向数据专家的指针
    QVector<Direction> m_inputSequence; // 玩家输入序列
    QTimer *m_inputTimer;               // 超时计时器

};

#endif // INPUTMATCHINGLOGIC_H


