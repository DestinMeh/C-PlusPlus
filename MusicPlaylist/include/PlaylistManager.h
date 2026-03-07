#pragma once

#include <QMap>
#include "Song.h"

struct PlaylistManager {
	QList<Song> playList;

	Q_INVOKABLE void addSong(QString path);
	Q_INVOKABLE void getNextSong();
};