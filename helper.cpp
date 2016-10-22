#include <QStandardPaths>
#include <QDir>
#include <QDirIterator>
#include <QDataStream>
#include <QFileDialog>
#include <QGuiApplication>

#include "helper.h"
#include "playlistitem.h"
#include "qmdxplayer.h"

Helper::Helper(QQmlContext* rootContext, QList<QObject*>* playList, QObject *parent)
    : QObject(parent)
    , rootContext_(rootContext)
    , playList_(playList)
    , randomPlayIndex_(0)
{
    Q_ASSERT(rootContext);
    Q_ASSERT(playList);
}

void Helper::clearPlayList()
{
    while(!playList_->isEmpty()){
        playList_->takeFirst()->deleteLater();
    }
    // クリアした状態をデフォルトとして反映
    saveDefaultPlayList();
    notifyPlayListUpdated();
}

bool Helper::loadPlayList(const QString& playListName)
{
    QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    loadPlayList(loadPath, playListName);
    // 読み込んだプレイリストはデフォルトとして反映
    saveDefaultPlayList();
}

bool Helper::savePlayList(const QString& playListName)
{
    QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    savePlayList(savePath, playListName);
}

bool Helper::loadDefaultPlaylist()
{
    QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    loadPlayList(loadPath, "PlayList");
}

bool Helper::saveDefaultPlayList()
{
    QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    savePlayList(savePath, "PlayList");
}

bool Helper::addFile(const QString& mdxFile)
{
    QMDXPlayer player;
    if(!player.loadSong(false, mdxFile, "", 1, true)){
        return false;
    }
    PlayListItem* item = new PlayListItem(player.title(), mdxFile);
    playList_->append(item);
    saveDefaultPlayList();
    makeRandomPlayList();
    notifyPlayListUpdated();
    return true;
}

bool Helper::addFolder(const QString& addPath, bool isTopFolder)
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
        PlayListItem* item = new PlayListItem(player.title(), info.absoluteFilePath());
        playList_->append(item);
    }

    if(isTopFolder){
        saveDefaultPlayList();
        makeRandomPlayList();
        notifyPlayListUpdated();
    }
    return true;
}

QString Helper::addFileDialog()
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

QString Helper::addFolderDialog()
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

int Helper::nextRandom(bool loop)
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

int Helper::previousRandom(bool loop)
{
    if(--randomPlayIndex_ < 0){
        randomPlayIndex_ = 0;
    }
    return randomPlayList_.empty()
            ? -1
            : randomPlayList_[randomPlayIndex_];
}

bool Helper::loadPlayList(QString path, QString playListName)
{
    QString playListFileName = path + QDir::separator() + playListName;

    QFile playListFile(playListFileName);
    QDataStream stream(&playListFile);
    if(!playListFile.open(QIODevice::ReadOnly)){
        return false;
    }
    while(!playList_->isEmpty()){
        playList_->takeFirst()->deleteLater();
    }
    while(!stream.atEnd()){
        QString title, fileName;
        stream >> title >> fileName;
        playList_->append(new PlayListItem(title, fileName));
    }
    makeRandomPlayList();
    notifyPlayListUpdated();
    return true;
}

bool Helper::savePlayList(QString path, QString playListName)
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
    foreach(auto item, *playList_){
        PlayListItem* playListItem = qobject_cast<PlayListItem*>(item);
        stream << playListItem->title() << playListItem->fileName();
    }
    return true;
}

void Helper::makeRandomPlayList()
{
    randomPlayList_.clear();
    randomPlayIndex_ = 0;
    for(int i = 0; i < playList_->size(); i++){
        randomPlayList_.push_back(i);
    }
    std::random_shuffle(randomPlayList_.begin(), randomPlayList_.end());
}

void Helper::notifyPlayListUpdated()
{
    rootContext_->setContextProperty("playList", QVariant::fromValue(*playList_));
}
