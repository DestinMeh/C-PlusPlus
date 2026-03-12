#include "PlaylistManager.h"

PlaylistManager::PlaylistManager(QObject* parent)
	: QObject(parent)
{
	// Good place to load your saved data automatically!
	loadFromFile("library.json");
}

PlaylistManager::~PlaylistManager() {
	// Loop through every list in the map and delete the song objects
	for (auto it = m_allPlaylists.begin(); it != m_allPlaylists.end(); ++it) {
		qDeleteAll(it.value());
	}
	m_allPlaylists.clear();
}

// Playlist Functions
void PlaylistManager::createPlaylist(const QString& name) {
	if (!m_allPlaylists.contains(name)) {

		m_allPlaylists.insert(name, QList<Song*>());

		emit playlistNamesChanged();

		qDebug() << "Created new empty playlist";
	}
}
QList<Song*> PlaylistManager::loadPlaylist(const QString& playlistName) {
	if (!checkPlaylist(playlistName)) {
		return QList<Song*>();
	}

	return m_allPlaylists[playlistName];
}

//Songs Functions
void PlaylistManager::addSongToPlaylist(const QString& playlistName,Song* newSong) {
	if (!newSong) return;

	if (!checkPlaylist(playlistName)) {
		qDebug() << "Playlist" << playlistName << "not found. Creating a new playlist";
		createPlaylist(playlistName);
	}


	m_allPlaylists[playlistName].append(newSong);

	emit playlistNamesChanged();
	saveToFile();
}

void PlaylistManager::clearPlaylistSongs(const QString& playlistName) {

	qDeleteAll(m_allPlaylists[playlistName]);
	m_allPlaylists[playlistName].clear();

	saveToFile();

	emit playlistNamesChanged();
}

bool PlaylistManager::checkPlaylist(const QString& playlistName) {
	if (m_allPlaylists.contains(playlistName)) {
		return true;
	}
	else {
		qDebug() << "Playlist not found!";
		return false;
	}
}

// File Management
void PlaylistManager::saveToFile(const QString& fileName) {
	QJsonObject root;

	for (auto it = m_allPlaylists.begin(); it != m_allPlaylists.end(); ++it) {
		QJsonArray songArray;

		for (Song* song : it.value()) {
			if (!song) continue;

			QJsonObject songData;
			songData.insert("path", song->path());
			songData.insert("title", song->title());
			songData.insert("artist", song->artist());
			songData.insert("duration", song->duration());
			songData.insert("fileType", song->fileType());
			songArray.append(songData);
		}
		root.insert(it.key(), songArray);
	}

	QJsonDocument doc(root);
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(doc.toJson());
		file.close();
	}
}
void PlaylistManager::loadFromFile(const QString& fileName) {

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) return;

	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	QJsonObject root = doc.object();

	m_allPlaylists.clear();
	for (const QString& playlistName : root.keys()) {
		QList<Song*> songs;
		QJsonArray songArray = root.value(playlistName).toArray();

		for (int i = 0; i < songArray.size(); ++i) {
			QJsonObject obj = songArray.at(i).toObject();

			Song* newSong = new Song(
				obj.value("path").toString(),
				obj.value("title").toString(),
				obj.value("artist").toString(),
				obj.value("duration").toDouble(),
				obj.value("fileType").toString(),
				this
			);
			songs.append(newSong);
		}

		m_allPlaylists.insert(playlistName, songs);
	}
	emit playlistNamesChanged();

}

// Song Creation
Song* PlaylistManager::createSongFromFile(const QString& path) {
	ma_decoder decoder;
	ma_result result = ma_decoder_init_file(path.toStdString().c_str(), NULL, &decoder);

	if (result != MA_SUCCESS) {
		qDebug() << "Could not read metadata for:" << path;
		return new Song(path);
	}

	// duration
	ma_uint64 lengthInFrames;
	ma_decoder_get_length_in_pcm_frames(&decoder, &lengthInFrames);
	float duration = (float)lengthInFrames / decoder.outputSampleRate;

	// fileType
	QString fileType = QFileInfo(path).suffix().toUpper();

	// Title
	QString title = QFileInfo(path).baseName();

	ma_decoder_uninit(&decoder);

	Song* newSong = new Song(path,
		title,
		"Unknown Artist",
		duration,
		fileType,
		this);

	return newSong;
}