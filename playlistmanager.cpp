#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QDataStream>
#include <QFileDialog>
#include <QGuiApplication>

#include <algorithm>
#include <random>

#include "playlistmanager.h"
#include "playlistitem.h"
#include "qmdxplayer.h"

PlaylistManager::PlaylistManager(QQmlContext* rootContext, QObject *parent)
	: QObject(parent)
	, rootContext_(rootContext)
	, randomPlayIndex_(0)
{
	Q_ASSERT(rootContext);
	rootContext_->setContextProperty("playlist", QVariant::fromValue(playlist_));
}

void PlaylistManager::clearPlaylist()
{
	while(!playlist_.isEmpty()){
		playlist_.takeFirst()->deleteLater();
	}
	// クリアした状態をデフォルトとして反映
	saveDefaultPlaylist();
	notifyPlaylistUpdated();
}

bool PlaylistManager::loadPlaylist(const QString& playlistName)
{
	QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	loadPlaylist(loadPath, playlistName);
	// 読み込んだプレイリストはデフォルトとして反映
	saveDefaultPlaylist();
}

bool PlaylistManager::savePlaylist(const QString& playlistName)
{
	QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	savePlaylist(savePath, playlistName);
}

bool PlaylistManager::loadDefaultPlaylist()
{
	QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	loadPlaylist(loadPath, "Playlist");
}

bool PlaylistManager::saveDefaultPlaylist()
{
	QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	savePlaylist(savePath, "Playlist");
}

bool PlaylistManager::addFile(const QString& mdxFile)
{
	QMDXPlayer player;
	if(!player.loadSong(false, mdxFile, "", 1, true)){
		return false;
	}
	PlaylistItem* item = new PlaylistItem(player.title(), mdxFile);
	playlist_.append(item);
	saveDefaultPlaylist();
	makeRandomPlaylist();
	notifyPlaylistUpdated();
	return true;
}

bool PlaylistManager::addFolder(const QString& addPath, bool isTopFolder)
{
	QDir addFolderPath(QDir::toNativeSeparators(addPath));
	if(!addFolderPath.exists()){
		return false;
	}
	// 再帰的にフォルダをスキャン
	QStringList nameFilters;
	nameFilters << "*.mdx";
	nameFilters << "*.MDX";

	QMDXPlayer player;
	QFileInfoList fileList = addFolderPath.entryInfoList(nameFilters, QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsLast);

	foreach(QFileInfo info, fileList){
		if(info.isDir()){
			addFolder(addFolderPath.absolutePath() + QDir::separator() + info.fileName(), false);
			continue;
		}

		if(!player.loadSong(false, info.absoluteFilePath(), "", 1, true)){
			continue;
		}
		PlaylistItem* item = new PlaylistItem(player.title(), info.absoluteFilePath());
		playlist_.append(item);
	}

	if(isTopFolder){
		saveDefaultPlaylist();
		makeRandomPlaylist();
		notifyPlaylistUpdated();
	}
	return true;
}

QString PlaylistManager::addFileDialog()
{
	// GTKスタイル使用時にファイル選択ダイアログがフリーズする対策
	QFileDialog::Options opt = 0;
	if (QGuiApplication::platformName() == QLatin1String("xcb")){
		opt |= QFileDialog::DontUseNativeDialog;
	}

	QFileDialog dialog;
	dialog.setOptions(opt);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter("MDX files (*.MDX *.mdx)");
#ifdef ALWAYSFULLSCREEN
	dialog.setWindowState(dialog.windowState() | Qt::WindowMaximized);
#endif
	QString result;
	if (dialog.exec() == QDialog::Accepted) {
		result = dialog.selectedFiles().value(0);
	}
	return result;
}

QString PlaylistManager::addFolderDialog()
{
	// GTKスタイル使用時にファイル選択ダイアログがフリーズする対策
	QFileDialog::Options opt = QFileDialog::ShowDirsOnly;
	if (QGuiApplication::platformName() == QLatin1String("xcb")){
		opt |= QFileDialog::DontUseNativeDialog;
	}

	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOptions(opt);
#ifdef ALWAYSFULLSCREEN
	dialog.setWindowState(dialog.windowState() | Qt::WindowMaximized);
#endif

	QString result;
	if (dialog.exec() == QDialog::Accepted) {
		result = dialog.selectedFiles().value(0).toUtf8();
	}

	return result;
}

int PlaylistManager::nextRandom(bool loop)
{
	if(++randomPlayIndex_ >= randomPlaylist_.size()){
		if(loop){
			randomPlayIndex_ = 0;
		} else {
			// ループ再生でない場合、リストの終端に達したら-1を返す(停止する)
			randomPlayIndex_ = randomPlaylist_.size() - 1;
			return -1;
		}
	}
	return randomPlaylist_.empty()
			? -1
			: randomPlaylist_[randomPlayIndex_];
}

int PlaylistManager::previousRandom(bool loop)
{
	if(--randomPlayIndex_ < 0){
		randomPlayIndex_ = 0;
	}
	return randomPlaylist_.empty()
			? -1
			: randomPlaylist_[randomPlayIndex_];
}

bool PlaylistManager::loadPlaylist(QString path, QString playlistName)
{
	QString playlistFileName = path + QDir::separator() + playlistName;

	QFile playlistFile(playlistFileName);
	QDataStream stream(&playlistFile);
	if(!playlistFile.open(QIODevice::ReadOnly)){
		return false;
	}
	while(!playlist_.isEmpty()){
		playlist_.takeFirst()->deleteLater();
	}
	while(!stream.atEnd()){
		QString title, fileName;
		stream >> title >> fileName;
		playlist_.append(new PlaylistItem(title, fileName));
	}
	makeRandomPlaylist();
	notifyPlaylistUpdated();
	return true;
}

bool PlaylistManager::savePlaylist(QString path, QString playlistName)
{
	QString playlistFileName = path + QDir::separator() + playlistName;

	QDir d(path);
	if(!d.exists()){
		if (!d.mkpath(path))
			return false;
	}
	QFile playlistFile(playlistFileName);
	QDataStream stream(&playlistFile);
	if(!playlistFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
		return false;
	}
	foreach(auto item, playlist_){
		PlaylistItem* playlistItem = qobject_cast<PlaylistItem*>(item);
		stream << playlistItem->title() << playlistItem->fileName();
	}
	return true;
}

void PlaylistManager::makeRandomPlaylist()
{
	randomPlaylist_.clear();
	randomPlayIndex_ = 0;
	for(int i = 0; i < playlist_.size(); i++){
		randomPlaylist_.push_back(i);
	}
	std::shuffle(randomPlaylist_.begin(), randomPlaylist_.end(), std::random_device());
}
void PlaylistManager::notifyPlaylistUpdated()
{
	rootContext_->setContextProperty("playlist", QVariant::fromValue(playlist_));
}
