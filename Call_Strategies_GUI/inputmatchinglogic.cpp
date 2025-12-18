#include "inputmatchinglogic.h"
#include "strategymanager.h"
#include <QDebug>
#include <algorithm>

InputMatchingLogic::InputMatchingLogic(StrategyManager *strategyManager, QObject *parent)
    : QObject{parent}, m_strategyManager(strategyManager)
{
    // 初始化
    m_inputTimer = new QTimer(this);
    // 设置单次触发
    m_inputTimer->setSingleShot(true);
     // 将timeout和槽函数连接，信号与槽机制
    connect(m_inputTimer, &QTimer::timeout, this, &InputMatchingLogic::onInputTimeout);
}


// 用户在UI按下按键后，需要这样一个槽函数通知InputMatchingLogic
void InputMatchingLogic::onKeyPressed(Direction dir)
{
    // 记录信息并启动清空输入箭头的倒计时
    m_inputSequence.append(dir);
    // 广播输入已接受的信号，让UI画箭头
    emit inputAccepted(dir);
    m_inputTimer->start(5000);
    // 检查输入的方向序列是否匹配上了某个战备
    checkSequenceMatch();
}


// 核心逻辑，检查输入的方向序列是否匹配上了某个战备
void InputMatchingLogic::checkSequenceMatch()
{
    for (const auto& strategy : m_strategyManager->getStrategies())
    {
        if (m_inputSequence == strategy.sequence)
        {
            // 显示战备名称
            qDebug() << "成功匹配战备：" << strategy.name;
            // ui->resultLabel->setText(strategy.name);// 移植前的残留
            m_inputTimer->stop(); // 别忘了停止计时器，否则它1.5秒后还会再清空一次
            // 匹配成功就给UI发射匹配成功的信号
            emit matchSuccess(strategy.name);
            m_inputSequence.clear();
            return;
        }
    }
    // 检查是否是任何一个配方的前缀
    bool isStillPossible = false;
    if (m_inputSequence.isEmpty())
    { // 如果是空的，当然是可能的
        isStillPossible = true;
    }
    else
    {
        for (const auto& strategy : m_strategyManager->getStrategies())
        {
            if (strategy.sequence.size() < m_inputSequence.size())
            {
                break; // 玩家输入的比配方还长，肯定不对
            }
            // 使用 std::equal 比较玩家输入序列是否和配方序列的开头部分完全相等
            if (std::equal(m_inputSequence.begin(), m_inputSequence.end(), strategy.sequence.begin()))
            {
                isStillPossible = true; // 找到了一个可能性
                break; // 跳出循环
            }
        }
    }
    // 如果没有任何可能性了，则重置
    if (!isStillPossible)
    {
        qDebug() << "输入错误，序列重置！";
        m_inputTimer->stop();
        // 发射失败的信号
        emit matchFailed();
        m_inputSequence.clear();
    }
}


// 超时后的清空逻辑
void InputMatchingLogic::onInputTimeout()
{
    if (!m_inputSequence.isEmpty()) {
        qDebug() << "GameLogic: Input timeout.";
        m_inputSequence.clear();
        emit sequenceCleared(); // <-- 发射超时清空信号！
    }
}



