// SPDX-License-Identifier: LGPL-3.0-or-later

#include <log.hpp>

using namespace nf::Log;

FILE *&StreamPolicy::stream() {
    static FILE *_stream = stdout;
    return _stream;
}

void StreamPolicy::output(const std::string &msg, bool error) {
    FILE *_stream = StreamPolicy::stream();

    if (!_stream)
        return;
    else if (error && _stream == stdout)
        _stream = stderr; // Redirect to stderr if we are not logging to a file

    fprintf(_stream, "%s\n", msg.c_str());
    fflush(_stream);
}