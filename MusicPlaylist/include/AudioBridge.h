#pragma once

#include <QObject>
#include <QDebug>
#include <QUrl>
#include "miniaudio.h" // We will put this in /extern

class AudioBridge : public QObject {
    Q_OBJECT // This macro is REQUIRED for Qt magic to work
    Q_PROPERTY(int playlistCount READ getPlaylistCount NOTIFY playlistChanged)

public:
    explicit AudioBridge(QObject* parent = nullptr);
    ~AudioBridge();

    

    // Q_INVOKABLE means you can call this function from your QML buttons
    Q_INVOKABLE void play(const QString& path);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void addToPlaylist(const QUrl& url);
    Q_INVOKABLE void playFromPlaylist(int index);


    int getPlaylistCount() const { return m_playlist.count(); }

signals:
    void playlistChanged();
    

private:
    ma_engine m_engine;   // The miniaudio engine instance
    bool m_isInitialized;
    QList<QString> m_playlist;
};