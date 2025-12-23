#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <QVector>
#include <QObject>
#include <QLabel>
#include <QMovie>
#include <QPropertyAnimation>
#include <QFile>
#include <QParallelAnimationGroup>
#include <QVariant>
#include <QGraphicsColorizeEffect>
#include <QColor>
#include <QMap>
#include <QDebug>
#include <QPainter> // 引入强大的绘图工具 QPainter
#include <QPixmapCache> // 引入 QPixmap 缓存，提升性能
#include <QCoreApplication>

class QLabel;     // 前向声明
class QMovie;     // 前向声明

class Animations : public QObject
{
    Q_OBJECT
public:
    Animations();
    // 工具类就该用静态方法
    // 静态方法：播放箭头脉冲动画
    // 参数：需要播放动画的 QLabel 列表，和一个父对象用于管理动画生命周期
    static void PulseAnimationForArrows(const QVector<QLabel*>& arrowLabels, const QColor& pulseColor, QObject* parent);

    // 静态方法：播放战备GIF
    // 参数：显示GIF的QLabel，控制GIF的QMovie，以及战备名
    static void playStrategyGif(QLabel* displayLabel, QMovie* movie, const QString& strategyName);
    // 创建静态实例用以发射信号，提供一个公共的、获取唯一实例的方法
    static Animations& instance();

signals:
    void pulseAnimationFinished();     // 告诉主窗口动画已经播完，可以开始清理

private:


};

#endif // ANIMATIONS_H
