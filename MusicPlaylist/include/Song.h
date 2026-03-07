#pragma once


#include <QObject>
#include <QString>

class Song : public QObject {
	Q_OBJECT

	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
	Q_PROPERTY(QString path READ path CONSTANT)

public:
	explicit Song(const QString &path, const QString &title = "", const QString& artist = "Unknown", QObject* parent = nullptr);

	QString title() const { return m_title; }
	QString artist() const { return m_artist; }
	QString path() const { return m_path; }

	void setTitle(const QString &title);
	void setArtist(const QString& artist);

signals:
	void titleChanged();
	void artistChanged();

private:
	QString m_title;
	QString m_artist;
	QString m_path;
};