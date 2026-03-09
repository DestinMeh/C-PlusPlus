#pragma once


#include <QObject>
#include <QString>

class Song : public QObject {
	Q_OBJECT

	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
	Q_PROPERTY(QString path READ path CONSTANT)
	Q_PROPERTY(QString fileType READ fileType CONSTANT)
	Q_PROPERTY(float duration READ duration CONSTANT)

public:
	explicit Song(const QString& path, 
				const QString& title = "", 
				const QString& artist = "Unknown", 
				const QString& fileType,
				const float& duration,
				QObject* parent = nullptr);

	void updateMetadata(const QString& title, const QString& artist, float duration, const QString& type);


	QString title() const { return m_title; }
	QString artist() const { return m_artist; }
	QString path() const { return m_path; }
	QString fileType() const { return m_fileType; }
	float duration() const { return m_duration; }

	void setTitle(const QString &title);
	void setArtist(const QString& artist);

signals:
	void titleChanged();
	void artistChanged();
	void metadataChanged();

private:
	QString m_title;
	QString m_artist;
	QString m_path;
	QString m_fileType;
	float m_duration;
};