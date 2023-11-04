#pragma once

#include <string>

struct build_info {
    std::string app_name = "big_number";
    int ver_major = 0;
    int ver_minor = 0;
    int ver_build = 136;
    friend std::ostream& operator<< (std::ostream& os, const build_info& bi);
};

extern const build_info bi;
