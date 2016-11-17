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
	rootContext_->setContextProperty("playList", QVariant::fromValue(playList_));
}

void PlaylistManager::clearPlayList()
{
	while(!playList_.isEmpty()){
		playList_.takeFirst()->deleteLater();
	}
	// クリアした状態をデフォルトとして反映
	saveDefaultPlayList();
	notifyPlayListUpdated();
}

bool PlaylistManager::loadPlayList(const QString& playListName)
{
	QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	loadPlayList(loadPath, playListName);
	// 読み込んだプレイリストはデフォルトとして反映
	saveDefaultPlayList();
}

bool PlaylistManager::savePlayList(const QString& playListName)
{
	QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	savePlayList(savePath, playListName);
}

bool PlaylistManager::loadDefaultPlaylist()
{
	QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	loadPlayList(loadPath, "PlayList");
}

bool PlaylistManager::saveDefaultPlayList()
{
	QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	savePlayList(savePath, "PlayList");
}

bool PlaylistManager::addFile(const QString& mdxFile)
{
	QMDXPlayer player;
	if(!player.loadSong(false, mdxFile, "", 1, true)){
		return false;
	}
	PlaylistItem* item = new PlaylistItem(player.title(), mdxFile);
	playList_.append(item);
	saveDefaultPlayList();
	makeRandomPlayList();
	notifyPlayListUpdated();
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
		playList_.append(item);
	}

	if(isTopFolder){
		saveDefaultPlayList();
		makeRandomPlayList();
		notifyPlayListUpdated();
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
	if(++randomPlayIndex_ >= randomPlayList_.size()){
		if(loop){
			randomPlayIndex_ = 0;
		} else {
			// ループ再生でない場合、リストの終端に達したら-1を返す(停止する)
			randomPlayIndex_ = randomPlayList_.size() - 1;
			return -1;
		}
	}
	return randomPlayList_.empty()
			? -1
			: randomPlayList_[randomPlayIndex_];
}

int PlaylistManager::previousRandom(bool loop)
{
	if(--randomPlayIndex_ < 0){
		randomPlayIndex_ = 0;
	}
	return randomPlayList_.empty()
			? -1
			: randomPlayList_[randomPlayIndex_];
}

bool PlaylistManager::loadPlayList(QString path, QString playListName)
{
	QString playListFileName = path + QDir::separator() + playListName;

	QFile playListFile(playListFileName);
	QDataStream stream(&playListFile);
	if(!playListFile.open(QIODevice::ReadOnly)){
		return false;
	}
	while(!playList_.isEmpty()){
		playList_.takeFirst()->deleteLater();
	}
	while(!stream.atEnd()){
		QString title, fileName;
		stream >> title >> fileName;
		playList_.append(new PlaylistItem(title, fileName));
	}
	makeRandomPlayList();
	notifyPlayListUpdated();
	return true;
}

bool PlaylistManager::savePlayList(QString path, QString playListName)
{
	QString playListFileName = path + QDir::separator() + playListName;

	QDir d(path);
	if(!d.exists()){
		if (!d.mkpath(path))
			return false;
	}
	QFile playListFile(playListFileName);
	QDataStream stream(&playListFile);
	if(!playListFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
		return false;
	}
	foreach(auto item, playList_){
		PlaylistItem* playListItem = qobject_cast<PlaylistItem*>(item);
		stream << playListItem->title() << playListItem->fileName();
	}
	return true;
}

void PlaylistManager::makeRandomPlayList()
{
	randomPlayList_.clear();
	randomPlayIndex_ = 0;
	for(int i = 0; i < playList_.size(); i++){
		randomPlayList_.push_back(i);
	}
	std::shuffle(randomPlayList_.begin(), randomPlayList_.end(), std::random_device());
}
void PlaylistManager::notifyPlayListUpdated()
{
	rootContext_->setContextProperty("playList", QVariant::fromValue(playList_));
}
