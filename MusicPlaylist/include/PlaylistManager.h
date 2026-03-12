#pragma once

#include <QMap>
#include <QStringList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

#include "Song.h"
#include "miniaudio.h"

class PlaylistManager : public QObject {
	Q_OBJECT

	Q_PROPERTY(QStringList playlistNames READ playlistNames NOTIFY playlistNamesChanged)

public:
	explicit PlaylistManager(QObject* parent = nullptr); 
	virtual ~PlaylistManager();

	Q_INVOKABLE void createPlaylist(const QString& name);
	Q_INVOKABLE void deletePlaylist(const QString& name);
	Q_INVOKABLE void addSongToPlaylist(const QString& playlistName, Song* newSong);
	Q_INVOKABLE void saveToFile(const QString& fileName = "library.json");
	Q_INVOKABLE void loadFromFile(const QString& fileName = "library.json");
	Q_INVOKABLE bool checkPlaylist(const QString& playlistName);
	
	void clearPlaylistSongs(const QString& playlistName);
	QList<Song*> loadPlaylist(const QString& playlistName);
	Song* createSongFromFile(const QString& path);
	

	QStringList playlistNames() const { return m_allPlaylists.keys(); }
	
signals:
	void playlistNamesChanged();

private:
	QMap<QString, QList<Song*> > m_allPlaylists;
};