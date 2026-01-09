#include "strategyitemwidget.h"
#include "ui_strategyItemWidget.h"
#include <QDebug>

StrategyItemWidget::StrategyItemWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::StrategyItemWidget)
{
    ui->setupUi(this);

    // 定义存可调用label的池子大小，由于绝地潜兵2最长的战备为地狱火炸弹，八个箭头，所以这里池子大小定义为10个
    const int POOL_SIZE = 10;
    QLayout* layout = ui->ArrowsContainer->layout();
    // 设置布局的对齐方式为：中心对齐
    layout->setAlignment(Qt::AlignHCenter);
    // 强制设置固定间距
    layout->setSpacing(6);

    for (int i = 0; i < POOL_SIZE; ++i)
    {
        QLabel *arrow = new QLabel(this);
        arrow->setFixedSize(16, 16);
        arrow->setScaledContents(true); // 让图片缩放到 QLabel 的大小
        arrow->hide();      // 创建出来后隐藏
        m_availableArrows.append(arrow);        // 留在内存里等待被放进ArrowContainer里
        layout->addWidget(arrow);       // 提前加入布局
    }

    // 创建”正在启动”的label， 和箭头放一起，放在最后
    m_launchingLabel = new QLabel("正在启动...", this);
    QFont font = m_launchingLabel->font();
    font.setBold(true);
    font.setPointSize(12);
    m_launchingLabel->setFont(font);
    m_launchingLabel->setFixedSize(100, 21);
    m_launchingLabel->setStyleSheet("background-color: transparent;color: #FFFFFF;");
    m_launchingLabel->setAlignment(Qt::AlignHCenter);
    m_launchingLabel->hide();
    layout->addWidget(m_launchingLabel);

}


void StrategyItemWidget::setStrategy(const StrategyItem& strategy)
{
    // 记下自己的数据
    m_strategy = strategy;

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

    // 初始状态下，显示完整的、未高亮的箭头序列
    updateMatchingStatus(QVector<Direction>()); // 传入一个空序列
}


// 跟踪玩家输入序列更新战备箭头显示
void StrategyItemWidget::updateMatchingStatus(const QVector<Direction> &currentInput)
{
    // 先隐藏
    //m_launchingLabel->hide();
    bool isPrefix = false;

    if (currentInput.size() <= m_strategy.sequence.size())
    {
        // 使用 std::equal 前缀匹配的判断
        isPrefix = std::equal(currentInput.begin(), currentInput.end(), m_strategy.sequence.begin());
    }

    auto effect = qobject_cast<QGraphicsOpacityEffect*>(this->graphicsEffect());
    if (!effect)
    {
        effect = new QGraphicsOpacityEffect(this);
        this->setGraphicsEffect(effect);
    }
    // 显示透明度判断
    if (currentInput.isEmpty() || isPrefix)
    {
        effect->setOpacity(1.0);
    }
    else
    {
        effect->setOpacity(0.3);
    }

    for (int i = 0; i < m_availableArrows.size(); ++i)
    {
        //QLabel *arrowLabel = m_availableArrows.takeFirst();
        QLabel* arrowLabel = m_availableArrows[i];

        if (i < m_strategy.sequence.size())
        {
            // 根据方向(dir)确定要使用哪个图片
            Direction dir = m_strategy.sequence[i];
            QString imagePath;
            switch (dir)
            {
                case UP:    imagePath = ":/resource/arrow_up.png";    break;
                case DOWN:  imagePath = ":/resource/arrow_down.png";  break;
                case LEFT:  imagePath = ":/resource/arrow_left.png";  break;
                case RIGHT: imagePath = ":/resource/arrow_right.png"; break;
            }
            arrowLabel->setPixmap(QPixmap(imagePath));

            if (i < currentInput.size() && isPrefix)
            {
                // 这部分是已匹配的，高亮显示
                arrowLabel->setStyleSheet("background-color: rgb(255, 255, 255, 120);");
            }
            else
            {
                // 未匹配部分，正常显示
                arrowLabel->setStyleSheet("background-color: transparent;");
            }
            arrowLabel->show();
            m_arrows.append(arrowLabel);
            //ui->ArrowsContainer->layout()->addWidget(arrowLabel);
        }
        else
        {
            arrowLabel->hide();
        }

    }
}

// 启动战备的方法
void StrategyItemWidget::showAsLaunching()
{
    // 先清空箭头
    for (QLabel *label : m_arrows)
    {
        ui->ArrowsContainer->layout()->removeWidget(label);
        label->hide();
        m_availableArrows.prepend(label);
    }
    m_launchingLabel->show();

}

//返回当前的战备名字
QString StrategyItemWidget::strategyName() const
{
    return m_strategy.name; // 直接返回内部存储的 strategy 的 name
}

// 重置面板状态
void StrategyItemWidget::resetState()
{
    this->setStyleSheet("background-color: transparent;");
    m_launchingLabel->hide();
    //调用 updateMatchingStatus 并传入一个空序列，
    //    这会自动让它恢复 1.0 的不透明度,重新绘制出所有未高亮的、完整的箭头序列
    updateMatchingStatus(QVector<Direction>());
}




