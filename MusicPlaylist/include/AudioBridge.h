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

    //Property
    Q_PROPERTY(QStringList playlist READ getPlaylist NOTIFY playlistChanged)
    Q_PROPERTY(QString currentSongTitle READ getCurrentSongTitle NOTIFY songTitleChanged) // like like instance variable
    

public:
    explicit AudioBridge(QObject* parent = nullptr);
    ~AudioBridge();

    

    // Q_INVOKABLE for QML buttons
    // Basic functions
    Q_INVOKABLE void play(const QString& path);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void addToPlaylist(const QUrl& url);
    Q_INVOKABLE void playFromPlaylist(int index);
    Q_INVOKABLE void clearPlaylist();
    Q_INVOKABLE void shuffle();
    


    QStringList getPlaylist() const { return m_playlist; }
    QString getCurrentSongTitle() const { return m_currentSong; }

signals:
    void playlistChanged();
    void songTitleChanged();
    

private:
    ma_engine m_engine;   // The miniaudio engine instance
    bool m_isInitialized;
    QStringList m_playlist;
    QString m_currentSong;
    ma_sound m_currentSound;

    //For 
    QTimer* m_syncTimer;
    int m_currentIndex = 0;
    bool m_songLoaded = false;
};