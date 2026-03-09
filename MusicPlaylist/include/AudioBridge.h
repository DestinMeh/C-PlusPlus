#pragma once

#include <iostream>
#include <algorithm>
#include <random>

#include <QObject>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QTimer>
#include "miniaudio.h" // We will put this in /extern

class AudioBridge : public QObject {
    Q_OBJECT // This macro is REQUIRED for Qt magic to work

    //Properties
    Q_PROPERTY(QStringList playlist READ getPlaylist NOTIFY playlistChanged)
    Q_PROPERTY(QString currentSongTitle READ getCurrentSongTitle NOTIFY songTitleChanged) // like like instance variable
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(float songPosition READ songPosition NOTIFY songPositionChanged)
    Q_PROPERTY(float songDuration READ songDuration NOTIFY songDurationChanged)

public:
    explicit AudioBridge(QObject* parent = nullptr);
    ~AudioBridge();

    

    // Q_INVOKABLE for QML buttons
    // Basic functions
    Q_INVOKABLE void play(const QString& path);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void togglePlay();
    //Q_INVOKABLE void loadPlaylist();
    Q_INVOKABLE void addToPlaylist(const QUrl& url);
    Q_INVOKABLE void playFromPlaylist(int index);
    Q_INVOKABLE void clearPlaylist();
    Q_INVOKABLE void shuffle(); // shuffle playlist
    Q_INVOKABLE void setVolume(float newVolume);
    Q_INVOKABLE void seek(float seconds); // click the slider to different parts of the song
    


    QStringList getPlaylist() const { return m_playlist; }
    QString getCurrentSongTitle() const { return m_currentSong; }
    float volume() const { return m_volume; }
    float songPosition() const { return m_songPosition; }
    float songDuration() const { return m_songDuration; }


signals:
    void playlistChanged();
    void songTitleChanged();
    void volumeChanged();
    void songPositionChanged();
    void songDurationChanged();
    

private:
    //miniaudio engine instance
    ma_engine m_engine;   
    bool m_isInitialized;

    //songs and playlist object instance
    QStringList m_playlist;
    QString m_currentSong;
    ma_sound m_currentSound;

    //volume
    float m_volume = 0.8;

    //song part
    float m_songPosition = 0;
    float m_songDuration = 0;

    //parallel song checker
    QTimer* m_syncTimer;
    int m_currentIndex = 0;
    bool m_songLoaded = false;
};