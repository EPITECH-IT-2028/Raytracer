#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

/**
 * @brief Namespace for Raytracer specific components.
 */
namespace Raytracer {

  /**
   * @brief Base class for custom exceptions in the Raytracer application.
   * Inherits from std::runtime_error.
   */
  class RaytracerError : public std::runtime_error {
    public:
      /**
       * @brief Constructs a RaytracerError object.
       * @param message The error message.
       */
      explicit RaytracerError(const std::string& message)
          : std::runtime_error("[RaytracerError] " + message) {
      }
  };

  /**
   * @brief Exception class for errors occurring during parsing.
   * Inherits from RaytracerError.
   */
  class ParseError : public RaytracerError {
    public:
      /**
       * @brief Constructs a ParseError object.
       * @param message The parsing error message.
       */
      explicit ParseError(const std::string& message)
          : RaytracerError("ParseError: " + message) {
      }
  };

  /**
   * @brief Exception class for errors related to configuration file processing.
   * Inherits from RaytracerError. Can include file name and line number.
   */
  class ConfigError : public RaytracerError {
    public:
      /**
       * @brief Constructs a ConfigError object.
       * @param message The configuration error message.
       * @param file The file in which the error occurred (optional).
       * @param line The line number where the error occurred (optional).
       */
      explicit ConfigError(const std::string& message,
                          const std::string& file = "", int line = -1)
          : RaytracerError(formatMessage(message, file, line)) {
      }

    private:
      /**
       * @brief Formats the error message to include file and line number if available.
       * @param msg The base error message.
       * @param file The file name.
       * @param line_num The line number.
       * @return std::string The formatted error message.
       */
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

  /**
   * @brief Exception class for errors related to plugin loading or execution.
   * Inherits from RaytracerError.
   */
  class PluginError : public RaytracerError {
    public:
      /**
       * @brief Constructs a PluginError object.
       * @param message The plugin error message.
       */
      explicit PluginError(const std::string& message)
          : RaytracerError("PluginError: " + message) {
      }
  };
}  // namespace Raytracer
