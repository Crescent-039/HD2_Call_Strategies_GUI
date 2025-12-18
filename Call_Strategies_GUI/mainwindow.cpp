#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCoreApplication>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
    // 在全局设置箭头容器的布局
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(ui->ArrowContainer->layout());
    if (layout)
    {
        // 设置布局的对齐方式为：中心对齐
        layout->setAlignment(Qt::AlignHCenter);
        // 强制设置固定间距
        layout->setSpacing(6);
    }

    // 窗口颜色
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(32,33,34));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    // 创建 实例
    m_strategyManager = new StrategyManager(this);
    m_audioManager = new AudioManager(this);
    m_inputMatchingLogic = new InputMatchingLogic(m_strategyManager, this);
    m_strategyMovie = new QMovie(this);
    // 将放映机和 resultLabel关联起来
    ui->resultLabel->setMovie(m_strategyMovie);
    //各种信号与槽
    connect(m_inputMatchingLogic, &InputMatchingLogic::inputAccepted, this, &MainWindow::onInputAccepted);
    connect(m_inputMatchingLogic, &InputMatchingLogic::matchSuccess, this, &MainWindow::onMatchSuccess);
    connect(m_inputMatchingLogic, &InputMatchingLogic::matchFailed, this, &MainWindow::onMatchFailed);
    connect(m_inputMatchingLogic, &InputMatchingLogic::sequenceCleared, this, &MainWindow::clearInputSequence);

    // 初始化输入锁为 false
    m_isInputLocked = false;
    // 创建延迟清除音效的计时器
    m_delayClearAudio = new QTimer(this);
    m_delayClearAudio->setSingleShot(true);
    connect(m_delayClearAudio, &QTimer::timeout, this, &MainWindow::delayclearAudio);

}

// 接受输入
void MainWindow::onInputAccepted(Direction dir)
{
    addArrow(dir); // addArrow 现在只负责画画
    m_audioManager->playKeyPressSound(AudioManager::KeyPress);// 播放音效
}

// 匹配成功
void MainWindow::onMatchSuccess(const QString &strategyName)
{
    m_isInputLocked = true;
    //ui->resultLabel->setText(strategyName);
    m_audioManager->playKeyPressSound(AudioManager::MatchSuccess);// 播放音效
    // 上特技
    Animations::PulseAnimationForArrows(m_arrowLabels, this);
    Animations::playStrategyGif(ui->resultLabel, m_strategyMovie, strategyName);
    m_delayClearAudio->start(3000);
    //clearInputSequence(); // 立即清除
}

// 匹配失败
void MainWindow::onMatchFailed()
{
    m_audioManager->playKeyPressSound(AudioManager::MatchFailure);// 播放音效
    clearInputSequence(); // 立即清除
    m_delayClearAudio->start(3000);// 延迟清理音效
}


// 延迟清理音效的槽函数
void MainWindow::delayclearAudio()
{
    //clearInputSequence();
    // 解锁键盘输入
    m_audioManager->clearActiveSounds();
    m_isInputLocked = false;
}


// 清空输入的槽函数
void MainWindow::clearInputSequence()
{
    for (QLabel *label : m_arrowLabels)
    {
        label->deleteLater();
    }
    // 清空箭头的列表
    m_arrowLabels.clear();
    // 清空UI的箭头显示
    ui->resultLabel->clear();
    m_strategyMovie->stop(); // 只停止播放
    // ui->resultLabel->clear();
    ui->resultLabel->setText(""); // 用设置空字符串来代替 clear()，这样不会断开movie的连接
    // 清理音效
    //m_audioManager->clearActiveSounds();
}

//重写键盘输入事件
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (m_isInputLocked)
    {
        return;
    }
    switch (event->key()) {
        case Qt::Key_Up:                //case穿透技巧，两个case共享一个事件
        case Qt::Key_W:
        // 转发给inputMatchingLogic的onKeyPressed函数
            m_inputMatchingLogic->onKeyPressed(UP);
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            m_inputMatchingLogic->onKeyPressed(DOWN);
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            m_inputMatchingLogic->onKeyPressed(LEFT);
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            m_inputMatchingLogic->onKeyPressed(RIGHT);
            break;
    }
}


// 添加输入的箭头的函数
void MainWindow::addArrow(Direction dir)
{
    // 根据传入的 dir使用图片
    QString imagePath; // 声明一个字符串变量来存储图片路径
    switch (dir) {
        case UP:
            imagePath = ":/resource/arrow_up.png";
            break;
        case DOWN:
            imagePath = ":/resource/arrow_down.png";
            break;
        case LEFT:
            imagePath = ":/resource/arrow_left.png";
            break;
        case RIGHT:
            imagePath = ":/resource/arrow_right.png";
            break;
    }

    // 创建并显示 QLabel
    QLabel *arrowLabel = new QLabel(this);
    QPixmap arrowPixmap(imagePath);
    if (arrowPixmap.isNull()) {
        qDebug() << "错误：加载图片失败！路径：" << imagePath;
        delete arrowLabel; // 创建了但加载图片失败，就把它删掉，避免内存泄漏
        return; // 直接退出函数
    }
    arrowLabel->setPixmap(arrowPixmap);
    ui->ArrowContainer->layout()->addWidget(arrowLabel);
    arrowLabel->show();

    m_arrowLabels.append(arrowLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}



// QT学习：主窗口函数第一阶段逻辑总结
/*
首先学一下开局代码的逻辑  QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(ui->ArrowContainer->layout());
用QHBoxLayout* layout声明一个指向QHBoxLayout类型对象的指针，然后用qobject_cast，这算是个专门用于 QObject 及其子类 之间进行动态类型转换的模板函数
<QHBoxLayout*>，让qobject_cast鉴定(ui->ArrowContainer->layout())是不是QHBoxLayout*水平布局，括号里的是传给他的实际东西，是ui里的ArrowContainer模块里面的layout信息
这一步的目的是找到布局方式，下面开始判断，强制设置布局方式为中心对齐，然后设置间距

下一步设置窗口颜色，为当前窗口部件创建一个当前的调色板QPalette的副本，并将其存储在一个名为 palette 的局部变量中
当一个对象调用自己的成员函数时，编译器会把这个对象的地址作为参数传递给该函数，这个地址就被存放在 this 指针里，也就是说这里的this是指针
this->palette()则是，获取this指针指向的对象实例，然后调用指向的这个对象的palette()函数，其实就是调用自己的palette()，因为自己也就是主窗口就是继承自QObject



*/


