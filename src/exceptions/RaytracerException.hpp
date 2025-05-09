#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace Raytracer {

  class RaytracerError : public std::runtime_error {
    public:
      explicit RaytracerError(const std::string& message)
          : std::runtime_error("[RaytracerError] " + message) {
      }
  };

  class ParseError : public RaytracerError {
    public:
      explicit ParseError(const std::string& message)
          : RaytracerError("ParseError: " + message) {
      }
  };

  class ConfigError : public RaytracerError {
    public:
      explicit ConfigError(const std::string& message,
                          const std::string& file = "", int line = -1)
          : RaytracerError(formatMessage(message, file, line)) {
      }

    private:
      static std::string formatMessage(const std::string& msg,
                                       const std::string& file, int line_num) {
        std::ostringstream oss;
        oss << "ConfigError: " << msg;
        if (!file.empty()) {
          oss << " in file \'" << file << "\'";
          if (line_num != -1) {
            oss << " at line " << line_num;
          }
        }
        return oss.str();
      }
  };

  class PluginError : public RaytracerError {
    public:
      explicit PluginError(const std::string& message)
          : RaytracerError("PluginError: " + message) {
      }
  };
}  // namespace Raytracer
