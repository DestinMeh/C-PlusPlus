#include "Song.h"
#include <QFileInfo>

Song::Song(const QString& path, const QString& title, const QString& artist, QObject* parent)
    : QObject(parent), m_path(path), m_title(title), m_artist(artist)
{
    // If title is empty, use the filename (e.g., "song.mp3") as the title
    if (m_title.isEmpty()) {
        m_title = QFileInfo(path).baseName();
    }
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
        emit artistChanged;
    }
}