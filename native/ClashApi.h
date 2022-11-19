#pragma once

#include "fmt/format.h"
#include "httplib.h"
#include "json.hpp"

using namespace nlohmann;
using namespace httplib;

namespace Clash
{
    struct Traffic
    {
        uint32_t up{ 0 };
        uint32_t down{ 0 };
    };

    struct Log
    {
        std::string type;
        std::string protocol;
        std::string local_addr;
        std::string remote_addr;
        std::string proxy_name;
    };
    using Logs = std::vector<Log>;

    void from_json(const json &o, Clash::Traffic &traffic)
    {
        o.at("up").get_to(traffic.up);
        o.at("down").get_to(traffic.down);
    }

    void from_json(const json &o, Clash::Log &log)
    {
        log.type = o.at("type").get<std::string>();
        auto payload = o.at("payload").get<std::string>();
        if (payload.empty())
        {
            return;
        }

        auto split_kv = [](auto &&text) {
            std::string k, v;
            size_t count = 0;
            httplib::detail::split(text.data(), text.data() + text.size(), '=', [&](const char *b, const char *e) {
                switch (count++)
                {
                case 0:
                    k = std::string(b, e);
                    break;
                case 1:
                    v = std::string(b, e);
                    break;
                }
            });
            return std::make_tuple(k, v);
        };

        size_t count = 0;
        httplib::detail::split(payload.data(), payload.data() + payload.size(), ' ', [&](const char *b, const char *e) {
            std::string text(b, e);
            switch (count++)
            {
            case 0:
                log.protocol = text;
                break;
            case 2:
                log.local_addr = std::get<1>(split_kv(text));
                break;
            case 3:
                log.remote_addr = std::get<1>(split_kv(text));
                break;
            case 6:
                log.proxy_name = std::get<1>(split_kv(text));
                break;
            default:
                break;
            }
        });
    }
}  // namespace Clash

struct ClashApi
{
    static void init(int port = 9090)
    {
        ClashApi::port_ = port;
    }

    static void get_traffic(std::function<void(const Clash::Traffic &t)> func)
    {
        httplib::Client cli(host_, port_);
        cli.Get("/traffic", [&func](const char *data, size_t len) {
            if (len > 0)
            {
                auto result = json::parse(std::string(data, len));
                func({ result });
            }
            return true;
        });
    }

    static void get_logs(std::function<void(const Clash::Logs &t)> func)
    {
        httplib::Client cli(host_, port_);
        while (true)
        {
            auto result = cli.Get("/logs", [&func](const char *data, size_t len) {
                if (len > 0)
                {
                    auto body = json::parse(std::string(data, len));
                    auto log = body.get<Clash::Log>();
                    func({ log });
                }
                return true;
            });
            if (!result)
            {
                auto err = httplib::to_string(result.error());
                err = "";
            }
        }
    }

private:
    static const char *host_;
    static int port_;
};

__declspec(selectany) int ClashApi::port_ = 9090;
__declspec(selectany) const char *ClashApi::host_ = "localhost";
