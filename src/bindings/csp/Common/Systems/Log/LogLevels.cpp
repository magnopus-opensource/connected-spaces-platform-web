#include "CSP/Common/Systems/Log/LogLevels.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPLogLevels)
{
    emscripten::enum_<csp::common::LogLevel>("LogLevel", emscripten::enum_value_type::number)
        .value("NoLogging", csp::common::LogLevel::NoLogging)
        .value("Fatal", csp::common::LogLevel::Fatal)
        .value("Error", csp::common::LogLevel::Error)
        .value("Warning", csp::common::LogLevel::Warning)
        .value("Display", csp::common::LogLevel::Display)
        .value("Log", csp::common::LogLevel::Log)
        .value("Verbose", csp::common::LogLevel::Verbose)
        .value("VeryVerbose", csp::common::LogLevel::VeryVerbose)
        .value("All", csp::common::LogLevel::All);
}
