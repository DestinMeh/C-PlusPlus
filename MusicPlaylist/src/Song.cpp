#include "Song.h"
#include <QFileInfo>

Song::Song(const QString& path, const QString& title, const QString& artist,const QString& fileType,const float& duration, QObject* parent)
    : QObject(parent), m_path(path), m_title(title), m_artist(artist), m_fileType(fileType), m_duration(duration)
{
    // If title is empty, use the filename (e.g., "song.mp3") as the title
    if (m_title.isEmpty()) {
        m_title = QFileInfo(path).baseName();
    }
}

void Song::updateMetadata(const QString& title, const QString& artist, float duration, const QString& type) {
    if (m_title != title) { m_title = title; emit titleChanged(); }
    if (m_artist != artist) { m_artist = artist; emit artistChanged(); }
    m_duration = duration;
    m_fileType = type;

    emit metadataChanged();
}

void Song::setTitle(const QString& title) {
    if (m_title != title) {
        m_title = title;
        emit titleChanged();
    }
}

void Song::setArtist(const QString& artist) {
    if (m_artist != artist) {
        m_artist = artist;
        emit artistChanged();
    }
}