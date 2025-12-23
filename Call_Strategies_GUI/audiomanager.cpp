#include "audiomanager.h"
#include <QSoundEffect>
#include <QUrl>


AudioManager::AudioManager(QObject *parent)
    : QObject{parent}, m_poolIndex(0)
{
    // 新方案直接根据按键生成随机选择音效文件的播放器，确保不会连续调用同一播放器
    // 多种音效的文件列表
    QVector<QUrl> KeySoundUrls;
    KeySoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/KeySound/DirectionKey01.wav"));
    KeySoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/KeySound/DirectionKey02.wav"));
    KeySoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/KeySound/DirectionKey03.wav"));
    KeySoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/KeySound/DirectionKey04.wav"));
    KeySoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/KeySound/DirectionKey05.wav"));
    KeySoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/KeySound/DirectionKey06.wav"));
    KeySoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/KeySound/DirectionKey07.wav"));

    QVector<QUrl> SuccessSoundUrls;
    SuccessSoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/ActivateSound/SuccessSound01.wav"));
    SuccessSoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/ActivateSound/SuccessSound02.wav"));
    SuccessSoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/ActivateSound/SuccessSound03.wav"));
    SuccessSoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/ActivateSound/SuccessSound04.wav"));

    QVector<QUrl> FailureSoundUrls;
    FailureSoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/CancelSound/FailureSound01.wav"));
    FailureSoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/CancelSound/FailureSound02.wav"));
    FailureSoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/CancelSound/FailureSound03.wav"));
    FailureSoundUrls.append(QUrl::fromLocalFile(":/sounds/HD2Sound/CancelSound/FailureSound04.wav"));

    // 文件柜逻辑，把上面那些列表都装进QMap里
    m_soundMap[KeyPress] = KeySoundUrls;
    m_soundMap[MatchSuccess] = SuccessSoundUrls;
    m_soundMap[MatchFailure] = FailureSoundUrls;    

    // 预创建音效播放池，8个音效播放器足以应对很快速的输入
    const int POOL_SIZE = 8;
    for (int i = 0; i<POOL_SIZE; ++i)
    {
        m_playerPool.append(new QSoundEffect(this));
    }
}

// 按键播放音效的方法
void AudioManager::playKeyPressSound(SoundType type)
{
    // 检查文件柜有无这个类型的地址列表
    if (!m_soundMap.contains(type))
    {
        return;
    }
    // 拿出对应URL列表
    const  QVector<QUrl>& urlList = m_soundMap[type];
    // 确保里面有东西
    if (urlList.isEmpty())
    {
        return;
    }
    // 旧方法为创建 QSoundEffect 实例，新方法使用播放器池来节省内存
    QSoundEffect *soundPlayer =m_playerPool[m_poolIndex];
    // 随机选择一个音效 URL
    int randomIndex = QRandomGenerator::global()->bounded(urlList.size());
    const QUrl &randomSoundUrl = urlList.at(randomIndex);
    // 设置音源、音量，然后立即播放
    soundPlayer->setSource(randomSoundUrl);
    soundPlayer->setVolume(2);
    soundPlayer->play();
    // 更新轮询索引，下一次调用下一个播放器
    // 用% POOL_SIZE确保索引能在0到7之间循环
    m_poolIndex = (m_poolIndex + 1) % m_playerPool.size();

}

// 销毁所有活跃的音效播放器，释放内存
void AudioManager::clearActiveSounds()
{
//    // 销毁所有活跃的音效播放器
//    for (const auto &sound : m_activeSounds)
//    {
//        sound->deleteLater();
//    }
//    m_activeSounds.clear(); // 清空列表，为下一次输入做准备
}




