#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QString>
#include <QVector>

struct FileEntry
{
    QString name;
    QString path;
    bool isDir = false;
    qint64 size = 0;
    QDateTime lastModified;
    QString suffix;
};

class FileSystemModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged)
    Q_PROPERTY(QString parentPath READ parentPath NOTIFY currentPathChanged)

public:
    explicit FileSystemModel(QObject *parent = nullptr);

    enum Roles
    {
        NameRole = Qt::UserRole + 1,
        PathRole,
        IsDirRole,
        SizeRole,
        LastModifiedRole,
        SuffixRole,
        IconRole
    };

    Q_ENUM(Roles)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString currentPath() const;
    QString parentPath() const;

    Q_INVOKABLE void setCurrentPath(const QString &path);
    Q_INVOKABLE void open(int row);
    Q_INVOKABLE void goUp();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool createFolder(const QString &folderName);

    Q_INVOKABLE QString homePath() const;
    Q_INVOKABLE QString downloadsPath() const;
    Q_INVOKABLE QString desktopPath() const;

signals:
    void currentPathChanged();
    void errorOccurred(const QString &message);

private:
    void loadDirectory(const QString &path);
    QString iconNameFor(const FileEntry &entry) const;

private:
    QVector<FileEntry> m_entries;
    QString m_currentPath;
};
