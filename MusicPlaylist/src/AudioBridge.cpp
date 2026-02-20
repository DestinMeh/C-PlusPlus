#include "AudioBridge.h"

// We define the implementation here ONCE
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"



AudioBridge::AudioBridge(QObject* parent) : QObject(parent), m_isInitialized(false) {
    // Initialize the audio engine
    if (ma_engine_init(NULL, &m_engine) == MA_SUCCESS) {
        m_isInitialized = true;
        qDebug() << "Audio Engine started successfully!";
    }
}

AudioBridge::~AudioBridge() {
    if (m_isInitialized) {
        ma_engine_uninit(&m_engine);
    }
}

void AudioBridge::play(const QString& path) {
    if (!m_isInitialized) return;

    // Convert Qt string to standard C string for miniaudio
    ma_engine_play_sound(&m_engine, path.toStdString().c_str(), NULL);
    qDebug() << "Playing:" << path;
}

void AudioBridge::stop() {
    // For a simple start, we'll just stop all sounds
    // Later, you can manage specific sound objects!
}