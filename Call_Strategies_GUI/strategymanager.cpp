#include "strategymanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

StrategyManager::StrategyManager(QObject *parent)
    : QObject{parent}
{
    load();

    // 临时实验，选择前四个战备
    if (m_strategies.size()>=6)
    {
        m_equippedStrategies.push_back(m_strategies[0]);
        m_equippedStrategies.push_back(m_strategies[1]);
        m_equippedStrategies.push_back(m_strategies[2]);
        m_equippedStrategies.push_back(m_strategies[3]);
        m_equippedStrategies.push_back(m_strategies[4]);
        m_equippedStrategies.push_back(m_strategies[5]);
    }
}


const QVector<StrategyItem>& StrategyManager::getStrategies() const
{
    return m_strategies; // 直接返回成员变量
}

const QVector<StrategyItem>& StrategyManager::getEquippedStrategies() const
{
    return m_equippedStrategies; // 直接返回“已装备列表”这个成员变量
}

// 载入战略配备的配置文件
void StrategyManager::load()
{
    QFile file(":/setting/strategies.json");
    if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "发生错误：无法打开战备种类配置文件，请检查根目录。";
            return;
        }
    // 读取文件内容，先读，再解析，再遍历
    QByteArray jsonData = file.readAll();
    file.close();
    // 解析json文件
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
            qDebug() << "错误：JSON文件格式不正确，元素必须是一个数组。";
            return;
        }
    // 遍历json中的数组
    QJsonArray jsonArray = doc.array();
        for (const QJsonValue &value : jsonArray)
        {
            QJsonObject jsonObj = value.toObject();
            StrategyItem newItem; // 创建一个新的战备“盒子”
            newItem.name = jsonObj["name"].toString();
            newItem.iconPath = jsonObj["iconPath"].toString();
            QJsonArray sequenceArray = jsonObj["sequence"].toArray();

            for (const QJsonValue &seqValue : sequenceArray)
            {
                        // 将JSON中的数字转换为Direction枚举
                newItem.sequence.append(static_cast<Direction>(seqValue.toInt()));
            }
                    // 把数据填入总列表中
            m_strategies.append(newItem);
        }
        // 打印出来验证一下
        qDebug() << "成功加载" << m_strategies.count() << "个战备";
        for(const auto& strategy : m_strategies)
        {
            qDebug() << "战备名:" << strategy.name << " 指令序列:" << strategy.sequence << " 图标:" << strategy.iconPath;
        }
}




