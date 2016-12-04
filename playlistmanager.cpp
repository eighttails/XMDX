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

// currentIndexが有効範囲にない場合(またはプレイリストが空の場合)、-1とする。
constexpr int INDEX_INVALID = -1;

PlaylistManager::PlaylistManager(QQmlContext* rootContext, QObject *parent)
	: QObject(parent)
	, rootContext_(rootContext)
	, currentIndex_(INDEX_INVALID)
	, randomPlayIndex_(0)
{
	if(rootContext){
		rootContext_->setContextProperty("playlist", QVariant::fromValue(playlist_));
	}
}

void PlaylistManager::clearPlaylist()
{
	while(!playlist_.isEmpty()){
		playlist_.takeFirst()->deleteLater();
	}
	// クリアした状態をデフォルトとして反映
	saveDefaultPlaylist();
	notifyPlaylistUpdated();
	resetCurrentIndex();
}

bool PlaylistManager::loadPlaylist(const QString& playlistName)
{
	QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	if(!loadPlaylist(loadPath, playlistName)) return false;
	// 読み込んだプレイリストはデフォルトとして反映
	return saveDefaultPlaylist();
}

bool PlaylistManager::savePlaylist(const QString& playlistName)
{
	QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	return savePlaylist(savePath, playlistName);
}

bool PlaylistManager::loadDefaultPlaylist()
{
	QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	return loadPlaylist(loadPath, "Playlist");
}

bool PlaylistManager::saveDefaultPlaylist()
{
	QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	return savePlaylist(savePath, "Playlist");
}

bool PlaylistManager::addFile(const QString& mdxFile)
{
	QMDXPlayer player(nullptr);
	if(!player.loadSong(false, mdxFile, "", 1, true)){
		return false;
	}
	PlaylistItem* item = new PlaylistItem(player.title(), mdxFile);
	playlist_.append(item);
	saveDefaultPlaylist();
	makeRandomPlaylist();
	notifyPlaylistUpdated();
	resetCurrentIndex();
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
		emit songAddedToPlaylist(player.title());
	}

	if(isTopFolder){
		saveDefaultPlaylist();
		makeRandomPlaylist();
		notifyPlaylistUpdated();
		resetCurrentIndex();
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

QString PlaylistManager::next(bool random, bool loop)
{
	if(currentIndex_ == INDEX_INVALID) return "";
	if(random){
		if(++randomPlayIndex_ >= static_cast<int>(randomPlaylist_.size())){
			if(loop){
				randomPlayIndex_ = 0;
			} else {
				// ループ再生でない場合、リストの終端に達したらINDEX_NONEを返す(停止する)
				randomPlayIndex_ = randomPlaylist_.size() - 1;
				currentIndex_ = INDEX_INVALID;
			}
		}
		currentIndex_ = randomPlaylist_.empty()
				? INDEX_INVALID
				: randomPlaylist_[randomPlayIndex_];
	} else {
		if(++currentIndex_ >= static_cast<int>(playlist_.size())){
			if(loop){
				currentIndex_ = 0;
			} else {
				// ループ再生でない場合、リストの終端に達したらINDEX_NONEを返す(停止する)
				currentIndex_ = INDEX_INVALID;
			}
		}
	}
	if(playlist_.isEmpty()) currentIndex_ = INDEX_INVALID;
	emit currentIndexChanged(currentIndex_);
	return currentIndex_ == INDEX_INVALID ? "" : playlist_[currentIndex_]->property("fileName").toString();
}

QString PlaylistManager::previous(bool random, bool loop)
{
	if(currentIndex_ == INDEX_INVALID) return "";
	if(random) {
		if(--randomPlayIndex_ < 0){
			randomPlayIndex_ = 0;
		}
		currentIndex_ = randomPlaylist_.empty()
				? INDEX_INVALID
				: randomPlaylist_[randomPlayIndex_];
	} else {
		if(--currentIndex_ < 0){
			if(loop){
				currentIndex_ = playlist_.size() - 1;
			} else {
				currentIndex_ = 0;
			}
		}
	}
	if(playlist_.isEmpty()) currentIndex_ = INDEX_INVALID;
	emit currentIndexChanged(currentIndex_);
	return currentIndex_ == INDEX_INVALID ? "" : playlist_[currentIndex_]->property("fileName").toString();
}

const PlaylistItem* PlaylistManager::currentPlaylistItem()
{
	return dynamic_cast<PlaylistItem*>(playlist_[currentIndex_]);
}

void PlaylistManager::setCurrentIndex(int index)
{
	int prevIndex = currentIndex_;
	if(index < 0 || index >= playlist_.size()) return;
	currentIndex_ = index;
	if(prevIndex != currentIndex_) {
		emit currentIndexChanged(currentIndex_);
	}
}

QByteArray PlaylistManager::getPlaylist()
{
	QByteArray array;
	QDataStream stream(&array, QIODevice::WriteOnly);
	writePlaylist(stream);
	return array;
}

int PlaylistManager::currentIndex() const
{
	return currentIndex_;
}

bool PlaylistManager::loadPlaylist(QString path, QString playlistName)
{
	QString playlistFileName = path + QDir::separator() + playlistName;

	QFile playlistFile(playlistFileName);
	if(!playlistFile.open(QIODevice::ReadOnly)){
		return false;
	}
	QDataStream stream(&playlistFile);
	return readPlaylist(stream);
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
	if(!playlistFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
		return false;
	}
	QDataStream stream(&playlistFile);
	return writePlaylist(stream);
}

bool PlaylistManager::readPlaylist(QDataStream &stream)
{
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
	resetCurrentIndex();
	return true;
}

bool PlaylistManager::writePlaylist(QDataStream &stream)
{
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

void PlaylistManager::resetCurrentIndex()
{
	if(playlist_.isEmpty()){
		//　プレイリストが空の場合はINDEX_INVALIDに設定
		setCurrentIndex(INDEX_INVALID);
	} else if(playlist_.size() <= currentIndex_){
		// インデックスがプレイリストの範囲外になった場合はプレイリストの先頭に設定
		setCurrentIndex(0);
	} else {
		// 上記に該当しない場合は何もしない
	}
}

void PlaylistManager::notifyPlaylistUpdated()
{
	if(rootContext_){
		rootContext_->setContextProperty("playlist", QVariant::fromValue(playlist_));
	}
	emit playlistChanged(getPlaylist());

	resetCurrentIndex();
}
