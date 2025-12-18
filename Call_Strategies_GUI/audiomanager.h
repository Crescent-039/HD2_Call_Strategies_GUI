#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QVector> // 引入 QVector
#include <QSoundEffect>
#include <QRandomGenerator>
#include <QMap>


class AudioManager : public QObject
{
    Q_OBJECT // 这个宏对于QObject子类是必须的
public:
    explicit AudioManager(QObject *parent = nullptr);
    // 音效种类
    enum SoundType {
            KeyPress,
            MatchSuccess,
            MatchFailure
        };

    void playKeyPressSound(SoundType type);
    void clearActiveSounds();
    AudioManager();

private:
// 音效池方案如果在随机音效池选择中，连续两次随机到一个音效池那就还是会卡顿
// 所以这里采用根据按键生成随机音效池的方案
QMap<SoundType, QVector<QUrl>> m_soundMap;                  // 只存储按键音效的URL“配方”
QVector<QSoundEffect*> m_activeSounds;      // 存储当前正在播放的音效实例

};

#endif // AUDIOMANAGER_H
