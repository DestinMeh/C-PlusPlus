#pragma once

#include <iostream>
#include <algorithm>
#include <random>

#include <QObject>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QTimer>

#include "PlaylistManager.h"
#include "miniaudio.h" // We will put this in /extern

class AudioBridge : public QObject {
    Q_OBJECT // This macro is REQUIRED for Qt magic to work

    //Properties
    Q_PROPERTY(PlaylistManager* playlistManager READ playlistManager CONSTANT)
    Q_PROPERTY(QList<Song*> currentPlaylistSongs READ getCurrentPlaylistSongs NOTIFY playlistSongsChanged)
    Q_PROPERTY(QString currentPlaylistName READ getCurrentPlaylistName NOTIFY playlistNameChanged)
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
    Q_INVOKABLE void loadPlaylist(const QString& playlistName);
    Q_INVOKABLE void addToPlaylist(const QUrl& url);
    Q_INVOKABLE void playFromPlaylist(int index);
    Q_INVOKABLE void clearPlaylist();
    Q_INVOKABLE void shuffle(); // shuffle playlist
    Q_INVOKABLE void setVolume(float newVolume);
    Q_INVOKABLE void seek(float seconds); // click the slider to different parts of the song
    


    PlaylistManager* playlistManager() { return &m_playlistManager; }
    QList<Song*> getCurrentPlaylistSongs() { return m_currentPlaylistSongs; }
    QString getCurrentPlaylistName() { return m_currentPlaylistName; }
    QString getCurrentSongTitle() const { return m_currentSong; }
    float volume() const { return m_volume; }
    float songPosition() const { return m_songPosition; }
    float songDuration() const { return m_songDuration; }


signals:
    void songTitleChanged();
    void volumeChanged();
    void songPositionChanged();
    void songDurationChanged();
    void playlistManagerChanged();
    void playlistNameChanged();
    void playlistSongsChanged();
    

private:
    //miniaudio engine instance
    ma_engine m_engine;   
    bool m_isInitialized;

    //songs and playlist object instance
    PlaylistManager m_playlistManager;
    QString m_currentPlaylistName;
    QList<Song*> m_currentPlaylistSongs;
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