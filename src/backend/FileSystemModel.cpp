#include "FileSystemModel.hpp"

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStandardPaths>
#include <QUrl>

FileSystemModel::FileSystemModel(QObject *parent)
    : QAbstractListModel(parent)
{
    setCurrentPath(QDir::homePath());
}

int FileSystemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_entries.size();
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) {
        return {};
    }

    const FileEntry &entry = m_entries.at(index.row());

    switch (role) {
    case NameRole:
        return entry.name;
    case PathRole:
        return entry.path;
    case IsDirRole:
        return entry.isDir;
    case SizeRole:
        return entry.size;
    case LastModifiedRole:
        return entry.lastModified.toString("yyyy-MM-dd HH:mm");
    case SuffixRole:
        return entry.suffix;
    case IconRole:
        return iconNameFor(entry);
    default:
        return {};
    }
}

QHash<int, QByteArray> FileSystemModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {PathRole, "path"},
        {IsDirRole, "isDir"},
        {SizeRole, "size"},
        {LastModifiedRole, "lastModified"},
        {SuffixRole, "suffix"},
        {IconRole, "iconName"}
    };
}

QString FileSystemModel::currentPath() const
{
    return m_currentPath;
}

QString FileSystemModel::parentPath() const
{
    QDir dir(m_currentPath);

    if (dir.cdUp()) {
        return dir.absolutePath();
    }

    return m_currentPath;
}

void FileSystemModel::setCurrentPath(const QString &path)
{
    const QFileInfo info(path);

    if (!info.exists()) {
        emit errorOccurred("La ruta no existe: " + path);
        return;
    }

    if (!info.isDir()) {
        emit errorOccurred("La ruta no es una carpeta: " + path);
        return;
    }

    loadDirectory(info.absoluteFilePath());
}

void FileSystemModel::open(int row)
{
    if (row < 0 || row >= m_entries.size()) {
        return;
    }

    const FileEntry &entry = m_entries.at(row);

    if (entry.isDir) {
        setCurrentPath(entry.path);
        return;
    }

    const bool opened = QDesktopServices::openUrl(QUrl::fromLocalFile(entry.path));

    if (!opened) {
        emit errorOccurred("No se pudo abrir el archivo: " + entry.name);
    }
}

void FileSystemModel::goUp()
{
    QDir dir(m_currentPath);

    if (dir.cdUp()) {
        setCurrentPath(dir.absolutePath());
    }
}

void FileSystemModel::refresh()
{
    setCurrentPath(m_currentPath);
}

bool FileSystemModel::createFolder(const QString &folderName)
{
    const QString cleanName = folderName.trimmed();

    if (cleanName.isEmpty()) {
        emit errorOccurred("El nombre de la carpeta no puede estar vacío.");
        return false;
    }

    if (cleanName.contains("/") || cleanName.contains("\\")) {
        emit errorOccurred("El nombre de la carpeta no puede contener separadores de ruta.");
        return false;
    }

    QDir dir(m_currentPath);

    if (!dir.mkdir(cleanName)) {
        emit errorOccurred("No se pudo crear la carpeta: " + cleanName);
        return false;
    }

    refresh();
    return true;
}

QString FileSystemModel::homePath() const
{
    return QDir::homePath();
}

QString FileSystemModel::downloadsPath() const
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    if (path.isEmpty()) {
        return QDir::homePath() + "/Downloads";
    }

    return path;
}

QString FileSystemModel::desktopPath() const
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    if (path.isEmpty()) {
        return QDir::homePath() + "/Desktop";
    }

    return path;
}

void FileSystemModel::loadDirectory(const QString &path)
{
    QDir dir(path);

    if (!dir.exists()) {
        emit errorOccurred("No se pudo cargar la carpeta: " + path);
        return;
    }

    const QFileInfoList list = dir.entryInfoList(
        QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System,
        QDir::DirsFirst | QDir::Name | QDir::IgnoreCase
    );

    beginResetModel();

    m_entries.clear();
    m_currentPath = dir.absolutePath();

    for (const QFileInfo &info : list) {
        FileEntry entry;
        entry.name = info.fileName();
        entry.path = info.absoluteFilePath();
        entry.isDir = info.isDir();
        entry.size = info.size();
        entry.lastModified = info.lastModified();
        entry.suffix = info.suffix().toLower();

        m_entries.append(entry);
    }

    endResetModel();

    emit currentPathChanged();
}

QString FileSystemModel::iconNameFor(const FileEntry &entry) const
{
    if (entry.isDir) {
        return "folder";
    }

    if (entry.suffix == "png" || entry.suffix == "jpg" || entry.suffix == "jpeg" || entry.suffix == "webp") {
        return "image";
    }

    if (entry.suffix == "mp4" || entry.suffix == "mkv" || entry.suffix == "mov") {
        return "video";
    }

    if (entry.suffix == "mp3" || entry.suffix == "wav" || entry.suffix == "flac") {
        return "audio";
    }

    if (entry.suffix == "cpp" || entry.suffix == "hpp" || entry.suffix == "h" || entry.suffix == "py" || entry.suffix == "java" || entry.suffix == "js") {
        return "code";
    }

    if (entry.suffix == "pdf") {
        return "pdf";
    }

    return "file";
}
