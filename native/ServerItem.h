#pragma once

#include "libs/json.hpp"
#include <QString>

using json = nlohmann::json;

struct ServerItem
{
    QString name;
    QString server;
    QString protocol;
    QString subscription;
    enum class Status
    {
        Disconnected,
        Connected
    } status;
    int latency{ 0 };

    operator bool() const
    {
        return !name.isEmpty() && !server.isEmpty();
    }
};

template <>
struct nlohmann::adl_serializer<ServerItem>
{
    static void from_json(const json &o, ServerItem &it)
    {
        it.name = o.value<std::string>("name", "").c_str();
        it.server = QString("%1://%2:%3")
                        .arg(o.value<std::string>("network", "tcp").c_str())
                        .arg(o.value<std::string>("server", "").c_str())
                        .arg(o.value<int>("port", 0));
        it.protocol = o.value<std::string>("type", "").c_str();
        it.subscription = o.value<std::string>("subscription", "").c_str();
        it.status = o.value<bool>("connected", false) ? ServerItem::Status::Connected : ServerItem::Status::Disconnected;
        it.latency = 0;
    }
};
