#include "animations.h"


Animations::Animations()
{

}

// 播箭头动画
void Animations::PulseAnimationForArrows(const QVector<QLabel*>& arrowLabels, QObject* parent)
{
    if (arrowLabels.isEmpty()) {
        return;
    }

    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(parent);

    // 步骤 1: 备份所有原始箭头图片
    // 我们创建一个 QMap，键是 QLabel 的指针，值是它对应的原始 QPixmap
    QMap<QLabel*, QPixmap> originalPixmaps;
    for (QLabel *label : arrowLabels) {
        if (const QPixmap* pix = label->pixmap()) { // 安全地获取 pixmap
            originalPixmaps[label] = *pix;
        }
    }
    const QColor startColor(255, 255, 255, 0);
    const QColor endColor(0, 255, 255, 255);
    animationGroup->setProperty("dummy", startColor); // 随便给个初始值
    QPropertyAnimation *anim = new QPropertyAnimation(animationGroup, "dummy");
    anim->setDuration(800);

    // 步骤 2: 改造 Lambda，让它使用备份好的 QMap
    // 我们需要捕获 originalPixmaps 这个 map
    QObject::connect(anim, &QPropertyAnimation::valueChanged, parent, [originalPixmaps](const QVariant &value){
        QColor currentColor = value.value<QColor>();

        // 为 Map 中的每一个箭头，都进行一次独立的绘制
        for (auto it = originalPixmaps.constBegin(); it != originalPixmaps.constEnd(); ++it) {
            QLabel* label = it.key();
            const QPixmap& originalPixmap = it.value(); // 获取该 label 对应的原始图片

            // --- 独立的“绘画工作室” ---
            QPixmap coloredPixmap(originalPixmap.size());
            coloredPixmap.fill(Qt::transparent);
            QPainter painter(&coloredPixmap);
            painter.drawPixmap(0, 0, originalPixmap); // 使用它自己的模板！
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.fillRect(coloredPixmap.rect(), currentColor);
            painter.end();

            label->setPixmap(coloredPixmap); // 只更新它自己！
        }
    });

    anim->setKeyValueAt(0, startColor);
    anim->setKeyValueAt(0.5, endColor);
    anim->setKeyValueAt(1, startColor);
    animationGroup->addAnimation(anim);

    // 步骤 3: 动画结束后，用备份的 QMap 来恢复
    QObject::connect(animationGroup, &QParallelAnimationGroup::finished, parent, [originalPixmaps](){
        for (auto it = originalPixmaps.constBegin(); it != originalPixmaps.constEnd(); ++it) {
            it.key()->setPixmap(it.value());
        }
    });

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}


// 播战备的gif
void Animations::playStrategyGif(QLabel* displayLabel, QMovie* movie, const QString& strategyName)
{
    // 如果和label断联则重新连接
    if (displayLabel->movie() != movie)
    {
        displayLabel->setMovie(movie);
    }

    QString appDir = QCoreApplication::applicationDirPath();
    QString gifPath = QString("%1/StrategyGifs/%2.gif").arg(appDir).arg(strategyName);

    if (!QFile::exists(gifPath))
    {
        qDebug() << "AnimationManager Warning: GIF not found for strategy:" << strategyName;
        movie->stop();
        displayLabel->setText(strategyName); // 恢复显示文字
        return;
    }
    movie->setSpeed(130);
    movie->stop();
    movie->setFileName(gifPath);
    movie->start();
}


