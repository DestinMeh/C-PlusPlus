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
        
        
        //connect(&m_playlistManager, &PlaylistManager::playlistNamesChanged, this, [this]() {       // Refreshes the QStringList property
        //    emit playlistSongsChanged();   // Refreshes the QList<Song*> property
        // });

        
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
        ma_sound_set_volume(&m_currentSound, m_volume);
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

    m_songPosition = 0;
    m_songDuration = 0;
    emit songDurationChanged();
    emit songPositionChanged();
    emit songTitleChanged();
    
}
void AudioBridge::playNext() {
    if (m_currentPlaylistSongs.isEmpty()) return;

    m_currentIndex++;
    if (m_currentIndex >= m_currentPlaylistSongs.size()) {
        m_currentIndex = 0;
    }

    // Use the Song object!
    Song* nextSong = m_currentPlaylistSongs.at(m_currentIndex);
    this->play(nextSong->path());

    // Important: manually update title/duration for the UI
    m_currentSong = nextSong->title();
    m_songDuration = nextSong->duration();
    emit songTitleChanged();
    emit songDurationChanged();
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
    // If no song is loaded, but we have songs in the object list
    else if (!m_currentPlaylistSongs.isEmpty()) {
        // Use playFromPlaylist to ensure metadata signals (title, duration) are emitted
        this->playFromPlaylist(m_currentIndex);
    }
}


// PLAYLIST FUNCTIONS
void AudioBridge::loadPlaylist(const QString& playlistName) {
    
    m_currentPlaylistSongs = m_playlistManager.loadPlaylist(playlistName);
    m_currentPlaylistName = playlistName;
    
    emit playlistSongsChanged();

    qDebug() << "Loaded playlist:" << playlistName << "with" << m_currentPlaylistSongs.size() << "songs.";
}
void AudioBridge::addToPlaylist(const QUrl& url) {
    QString localPath = url.toLocalFile();
   
    Song* newSong = m_playlistManager.createSongFromFile(localPath);


    m_playlistManager.addSongToPlaylist(m_currentPlaylistName, newSong);
    
    loadPlaylist(m_currentPlaylistName);

    emit playlistSongsChanged();
    
}
void AudioBridge::playFromPlaylist(int index) {

    if (index < 0 || index >= m_currentPlaylistSongs.size()) return;

    Song* songToPlay = m_currentPlaylistSongs.at(index);
    m_currentIndex = index;

    // Use the path from our object
    this->play(songToPlay->path());

    // Update metadata properties
    m_currentSong = songToPlay->title();
    m_songDuration = songToPlay->duration();

    emit songTitleChanged();
    emit songDurationChanged();
}


void AudioBridge::clearPlaylist() {
    m_playlistManager.clearPlaylistSongs(m_currentPlaylistName);
    m_currentPlaylistSongs.clear();

    emit playlistSongsChanged();
}

void AudioBridge::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_currentPlaylistSongs.begin(), m_currentPlaylistSongs.end(), g);


    emit playlistSongsChanged();
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