// SPDX-License-Identifier: LGPL-3.0-or-later

// Basic logging logic.
// Inspired by https://www.drdobbs.com/cpp/logging-in-c/201804215

#pragma once

#include <iomanip>
#include <sstream>

#include <utils.hpp>

namespace nf::Log {
enum Level { DEBUG, INFO, WARNING, ERROR, CRITICAL, SILENT };

// ----------------------------------------------------------------------------
// Classes

template <typename OutputPolicy> class Logger {
  private:
    Level message_level;

    std::string level_str[SILENT] = {
        "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

  public:
    Logger(){};
    virtual ~Logger();
    std::ostringstream &get(Level level = INFO);
    static Level &      reporting_level();

  protected:
    std::ostringstream os;
};

class StreamPolicy {
  public:
    static FILE *&stream();
    static void   output(const std::string &msg, bool error = false);
};

// ----------------------------------------------------------------------------
// Implementation

template <typename OutputPolicy>
Level &Logger<OutputPolicy>::reporting_level() {
    static Level level = INFO;
    return level;
}

template <typename OutputPolicy>
std::ostringstream &Logger<OutputPolicy>::get(Level level) {
    std::time_t time      = Utils::time();
    std::string timestamp = asctime(localtime(&time));
    timestamp.pop_back();

    this->os << "- " << timestamp;
    this->os << " " << this->level_str[level];
    this->os << ": " << std::string(8 - level_str[level].length(), ' ');

    this->message_level = level;
    return os;
}

template <typename OutputPolicy> Logger<OutputPolicy>::~Logger() {
    OutputPolicy::output(this->os.str(), this->message_level >= ERROR);
}

// ----------------------------------------------------------------------------
// Variables

typedef Logger<StreamPolicy> log;

#define LOG(level)                                                           \
    if (level < Log::log::reporting_level() || !Log::StreamPolicy::stream()) \
        ;                                                                    \
    else                                                                     \
        Log::log().get(level)

#define D LOG(Log::DEBUG)
#define I LOG(Log::INFO)
#define W LOG(Log::WARNING)
#define E LOG(Log::ERROR) << "(" << __FILE__ << ":" << __LINE__ << ") "
#define C LOG(Log::CRITICAL) << "(" << __FILE__ << ":" << __LINE__ << ") "
} // namespace nf::Log