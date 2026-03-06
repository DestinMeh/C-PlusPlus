#include "AudioBridge.h"

// We define the implementation here ONCE
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"



AudioBridge::AudioBridge(QObject* parent) : QObject(parent), m_isInitialized(false) {
    memset(&m_currentSound, 0, sizeof(ma_sound));

    if (ma_engine_init(NULL, &m_engine) == MA_SUCCESS) {
        m_isInitialized = true;
        m_syncTimer = new QTimer(this);

        connect(m_syncTimer, &QTimer::timeout, this, [this]() 
            {
                // This flag is your shield!
                if (!m_songLoaded) return;

                if (ma_sound_at_end(&m_currentSound)) {
                    m_songLoaded = false;
                    this->playNext();
                }
            }
        );

        // Only start if you want it polling; 
        // alternatively, only start it inside the play() function.
        m_syncTimer->start(500);

        qDebug() << "Program is starting properly";
    }
}

AudioBridge::~AudioBridge() {
    if (m_isInitialized) {
        m_syncTimer->stop();

        ma_sound_uninit(&m_currentSound);

        ma_engine_uninit(&m_engine);
        
    }
}

void AudioBridge::play(const QString& path) {
    if (!m_isInitialized) return;

    //Stop the timer so it doesn't poll during the transition
    m_syncTimer->stop();

    // IMPORTANT: Uninitialize the current sound
    // This tells the engine to stop referencing this memory
    ma_sound_uninit(&m_currentSound);

    // Zero out the memory to prevent "garbage" pointers
    memset(&m_currentSound, 0, sizeof(ma_sound));

    // Load the new file and check for success
    ma_result result = ma_sound_init_from_file(&m_engine,
        path.toStdString().c_str(),
        0, NULL, NULL, &m_currentSound);

    if (result == MA_SUCCESS) {
        ma_sound_start(&m_currentSound);

        m_songLoaded = true;

        m_currentSong = QFileInfo(path).fileName();
        emit songTitleChanged();

        //Only restart the heartbeat once the sound is safely loaded
        m_syncTimer->start(500);
    }
    else {
        qDebug() << "Miniaudio failed to load file. Error:" << (int)result;
    }
}

void AudioBridge::stop() {
    m_songLoaded = false;
    ma_sound_stop(&m_currentSound);
    m_currentSong = "Stopped";
    emit songTitleChanged();
    
}

void AudioBridge::playNext() {
    m_currentIndex++;
    if (m_currentIndex >= m_playlist.size()) {
        m_currentIndex = 0;
    }

    this->play(m_playlist.at(m_currentIndex));
    emit songTitleChanged();
}

void AudioBridge::addToPlaylist(const QUrl& url) {
    QString localPath = url.toLocalFile();

    if (!localPath.isEmpty()) {
        m_playlist.append(localPath);
        emit playlistChanged();
        qDebug() << "Added to playlist: " << localPath;
    }
    
}

void AudioBridge::playFromPlaylist(int index) {

    //check if index is out of Bounds
    if (index < 0 || index >= m_playlist.size()) {
        qDebug() << "Index is out of bounds";
        return;
    }

    if (m_playlist.isEmpty()) {
        qDebug() << "list is empty";
        return;
    }

    this->play(m_playlist.at(index));
}

void AudioBridge::clearPlaylist() {
    m_playlist.clear();
    emit playlistChanged();
}

void AudioBridge::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_playlist.begin(), m_playlist.end(), g);
    emit playlistChanged();
}