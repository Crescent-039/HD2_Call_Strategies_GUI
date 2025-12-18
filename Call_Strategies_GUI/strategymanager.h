#ifndef STRATEGYMANAGER_H
#define STRATEGYMANAGER_H

#include <QObject>
#include <QVector>
#include "strategyitem.h"

class StrategyManager : public QObject
{
    Q_OBJECT
public:
    explicit StrategyManager(QObject *parent = nullptr);

    // 公共接口：获取所有战备数据 (const引用，防止外部修改)
    const QVector<StrategyItem>& getStrategies() const;

private:
    // 用于存储所有战备的“配方”
    QVector<StrategyItem> m_strategies;
    void load();

};

#endif // STRATEGYMANAGER_H
