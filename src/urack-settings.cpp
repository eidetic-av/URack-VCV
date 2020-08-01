#include "urack.hpp"

using namespace rack;

namespace URack {

bool Settings::initialised = false;

void Settings::load() {
    std::string settingsFilename = asset::user("URack-settings.json");
    FILE *file = fopen(settingsFilename.c_str(), "r");
    if (!file)
        return;
    DEFER({ fclose(file); });

    json_error_t error;
    json_t *settings = json_loadf(file, 0, &error);
    if (!settings)
        return;

    std::map<int, std::tuple<std::string, int>> hosts;
    json_t *hostsJ = json_object_get(settings, "hosts");
    json_t *host;
    int hi = 0;
    while ((host = json_array_get(hostsJ, hi++)) != NULL) {
        int hostNum = json_integer_value(json_object_get(host, "hostNum"));
        int port = json_integer_value(json_object_get(host, "port"));
        std::string ip = json_string_value(json_object_get(host, "ip"));
        hosts[hostNum] = {ip, port};
    }
    for (unsigned int i = 0; i < hosts.size(); i++)
        Dispatcher::connect_host(std::get<0>(hosts[i]), std::get<1>(hosts[i]));
}

void Settings::save() {
    // save host ip entries
    // to plugin settings json

    json_t *settings = json_object();
    json_t *hosts = json_array();
    for (auto socketInfo : Dispatcher::sockets) {
        json_t *hostOption = json_object();
        json_object_set_new(hostOption, "hostNum",
                            json_integer(socketInfo->hostNum));
        json_object_set_new(hostOption, "ip",
                            json_string(socketInfo->ip.c_str()));
        json_object_set_new(hostOption, "port", json_integer(socketInfo->port));
        json_array_append_new(hosts, hostOption);
    }
    json_object_set_new(settings, "hosts", hosts);

    std::string settingsFilename = asset::user("URack-settings.json");
    FILE *file = fopen(settingsFilename.c_str(), "w");
    if (file) {
        json_dumpf(settings, file, JSON_INDENT(2));
        fclose(file);
    }

    json_decref(settings);
}
} // namespace URack
