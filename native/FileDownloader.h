#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "libs/httplib.h"
#include "libs/json.hpp"
#include <QDebug>
#include <qsystemdetection.h>
#include <string>
#include <tuple>

using json = nlohmann::json;

static std::tuple<std::string, std::string> parse_url(const std::string &url)
{
    auto pos = url.find_first_of("//");
    if (pos == url.npos)
    {
        pos = url.find_first_of('/');
    }
    else
    {
        pos = url.find_first_of('/', pos + 2);
    }
    auto host = url.substr(0, pos);
    auto res = url.substr(pos);
    return { host, res };
}

static httplib::Result http_get(const std::string &url)
{
    auto &&[host, path] = parse_url(url);
    httplib::Client cli(host);
    cli.set_follow_location(true);
    return cli.Get(path);
}

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    void downloadCore(const QString &version = "")
    {

#if defined(Q_OS_WIN)
        std::string platform = "windows-amd64";
#elif defined(Q_OS_LINUX)
        std::string platform = "linux-amd64";
#elif defined(Q_OS_MAC)
        std::string platform = "darwin-amd64";
#else
        std::string platform = "unknown";
#endif

        auto result = http_get("https://api.github.com/repos/Dreamacro/clash/releases/latest");
        if (!result)
        {
            qCritical() << (int)result.error();
            return;
        }
        auto arr = json::parse(result->body);
        for (auto &&it : arr.value<json>("assets", json::array()))
        {
            std::string name = it.value<std::string>("name", "");
            auto url = it.value<std::string>("browser_download_url", "");
            if (name.find(platform) != std::string::npos)
            {
                auto result = http_get(url);
                break;
            }

            //"": "https://github.com/Dreamacro/clash/releases/download/v1.11.8/clash-windows-amd64-v1.11.8.zip"
        }
    }

signals:
    void coreDownloaded();
};
