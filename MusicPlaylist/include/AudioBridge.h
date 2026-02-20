#pragma once

#include <QObject>
#include <QDebug>
#include "miniaudio.h" // We will put this in /extern

class AudioBridge : public QObject {
    Q_OBJECT // This macro is REQUIRED for Qt magic to work

public:
    explicit AudioBridge(QObject* parent = nullptr);
    ~AudioBridge();

    // Q_INVOKABLE means you can call this function from your QML buttons
    Q_INVOKABLE void play(const QString& path);
    Q_INVOKABLE void stop();

private:
    ma_engine m_engine;   // The miniaudio engine instance
    bool m_isInitialized;
};