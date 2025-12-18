#ifndef STRATEGYITEM_H
#define STRATEGYITEM_H
#include <QString>
#include <QVector> // 引入 QVector


// 在类定义的外面定义这个枚举,c++的enum枚举
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// 存战备类型的结构体
struct StrategyItem {
    QString name;
    QVector<Direction> sequence;

};

#endif // STRATEGYITEM_H

