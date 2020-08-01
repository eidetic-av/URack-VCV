#pragma once

namespace URack {

struct Settings {
    static bool initialised;
    static void load();
    static void save();
};

} // namespace URack
