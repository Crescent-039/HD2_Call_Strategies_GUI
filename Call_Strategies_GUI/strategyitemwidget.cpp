#include "strategyitemwidget.h"
#include "ui_strategyItemWidget.h"
#include <QDebug>

StrategyItemWidget::StrategyItemWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::StrategyItemWidget)
{
    ui->setupUi(this);
}


void StrategyItemWidget::setStrategy(const StrategyItem& strategy)
{
    ui->nameLabel->setText(strategy.name);
    // 增加图标
    if (!strategy.iconPath.isEmpty())
    {
        QString appDir = QCoreApplication::applicationDirPath();
        QString iconPixmap(strategy.iconPath);
        QString iconPath = QString("%1/%2").arg(appDir).arg(iconPixmap);
        ui->iconLabel->setPixmap(iconPath);
    }
    else
    {
        qDebug() << "错误：加载图标失败";
    }
    QLayout* layout = ui->ArrowsContainer->layout();
    // 设置布局的对齐方式为：中心对齐
    layout->setAlignment(Qt::AlignHCenter);
    // 强制设置固定间距
    layout->setSpacing(6);
    // 循环遍历新的指令序列，创建并添加新的箭头
    for (Direction dir : strategy.sequence) {
        // 创建一个新的 QLabel 用来显示箭头图片
        QLabel* arrowLabel = new QLabel(this);

        // 根据方向(dir)确定要使用哪个图片
        QString imagePath;
        switch (dir) {
            case UP:    imagePath = ":/resource/arrow_up.png";    break;
            case DOWN:  imagePath = ":/resource/arrow_down.png";  break;
            case LEFT:  imagePath = ":/resource/arrow_left.png";  break;
            case RIGHT: imagePath = ":/resource/arrow_right.png"; break;
        }

        // 加载图片并设置给 QLabel
        QPixmap arrowPixmap(imagePath);
        arrowLabel->setPixmap(arrowPixmap);

        //设置一下图片大小，让它看起来更精致
        arrowLabel->setFixedSize(16, 16);
        arrowLabel->setScaledContents(true); // 让图片缩放到 QLabel 的大小

        // 把新创建的 arrowLabel 添加到水平布局中
        layout->addWidget(arrowLabel);
    }
    this->show();
}
