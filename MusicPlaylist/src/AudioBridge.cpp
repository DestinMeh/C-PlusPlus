#include "AudioBridge.h"

// implementations
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


//Instance 
AudioBridge::AudioBridge(QObject* parent) : QObject(parent), m_isInitialized(false) {
    memset(&m_currentSound, 0, sizeof(ma_sound));

    if (ma_engine_init(NULL, &m_engine) == MA_SUCCESS) {
        m_isInitialized = true;
        m_syncTimer = new QTimer(this);
  

        connect(m_syncTimer, &QTimer::timeout, this, [this]() 
            {
                // This flag is your shield!
                if (!m_songLoaded) return;

                // ask miniaudio for current time in seconds
                ma_uint64 cursor;
                ma_sound_get_cursor_in_pcm_frames(&m_currentSound, &cursor);
                m_songPosition = (float)cursor / ma_engine_get_sample_rate(&m_engine);
                emit songPositionChanged();

                // check if song finished
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

//BASIC SONG FUNCTIONS
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

        ma_uint64 length;
        ma_sound_get_length_in_pcm_frames(&m_currentSound, &length);
        m_songDuration = (float)length / ma_engine_get_sample_rate(&m_engine);
        emit songDurationChanged();

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

    m_songPosition = 0;
    m_songDuration = 0;
    emit songDurationChanged();
    emit songPositionChanged();
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
void AudioBridge::togglePlay() {
    if (!m_isInitialized) return;
    
    if (m_songLoaded) {
        if (ma_sound_is_playing(&m_currentSound)) {
            ma_sound_stop(&m_currentSound);
        }
        else {
            ma_sound_start(&m_currentSound);
        }
    }
    else if (!m_playlist.isEmpty()) {
        this->play(m_playlist.at(m_currentIndex));
    }
}


// PLAYLIST FUNCTIONS
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

// VOLUME CONTROL
void AudioBridge::setVolume(float newVolume) {
    if (m_volume == newVolume) return;

    m_volume = newVolume;

    if (m_songLoaded) {
        ma_sound_set_volume(&m_currentSound, m_volume);
    }

    emit volumeChanged();
    qDebug() << "Volume changed to: " << m_volume;
}

// SONG CONTROL
void AudioBridge::seek(float seconds) {
    if (!m_songLoaded) return;

    ma_uint64 frame = (ma_uint64)(seconds * ma_engine_get_sample_rate(&m_engine));
    ma_sound_seek_to_pcm_frame(&m_currentSound, frame);
}