#pragma once
#include <fstream>

namespace FileLogger {
    void Log(const std::string& message) {
        std::ofstream os("Log.txt", std::ios::app);
        os << message;
        os.close();
    }
};
