#pragma once

#include "ServerItem.h"
#include <QAbstractTableModel>
#include <QStringList>

class ServersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ServersModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent)
        , headers_({ tr("Name"), tr("Server"), tr("Status"), tr("Latency") })
    {
    }

public:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole)
        {
            return {};
        }
        switch (orientation)
        {
        case Qt::Horizontal:
            return headers_.value(section);
        case Qt::Vertical:
            return section + 1;
        default:
            return {};
        }
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return servers_.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return headers_.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return display(index);
        default:
            break;
        }
        return {};
    }

public:
    QString display(const QModelIndex &index) const
    {
        auto si = servers_.value(index.row());
        switch (index.column())
        {
        case 0:
            return si.name;
        case 1:
            return si.server;
        case 2:
            return si.status == ServerItem::Status::Connected ? tr("Connected") : tr("Disconnected");
        case 3:
            return si.latency <= 0 ? QString("--") : QString("%1ms").arg(si.latency);
        default:
            return {};
        }
    }

    std::optional<ServerItem> item(const QModelIndex &index)
    {
        if (!index.isValid())
        {
            return std::nullopt;
        }
        return servers_.value(index.row());
    }

    void updateItemStatus(const QString &name, bool isConnected)
    {
        for (auto i = 0; i < servers_.size(); ++i)
        {
            auto &it = servers_[i];
            if (it.name == name)
            {
                it.status = isConnected ? ServerItem::Status::Connected : ServerItem::Status::Disconnected;
                emit dataChanged(index(i, 2), index(i, 3), { Qt::DisplayRole });
                break;
            }
        }
    }

    int width(int col) const
    {
        static QList<int> widths{ 190, 240, 120 };
        return widths.value(col, 100);
    }

    void setupData(QList<ServerItem> &&arr)
    {
        beginResetModel();
        servers_.swap(arr);
        endResetModel();
    }

private:
    QStringList headers_;
    QList<ServerItem> servers_;
};
