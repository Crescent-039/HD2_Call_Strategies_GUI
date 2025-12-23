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
    QMap<SoundType, QVector<QUrl>> m_soundMap;                  // 只存储按键音效的URL“配方”
    //QVector<QSoundEffect*> m_activeSounds;      // 存储当前正在播放的音效实例
    QVector<QSoundEffect*> m_playerPool;        // 播放器池
    int m_poolIndex;        // 用于轮询的索引

};

#endif // AUDIOMANAGER_H
