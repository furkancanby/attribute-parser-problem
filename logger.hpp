/*
 * File: SimpleLogger.hpp
 * Author: Furkancan Bilal Yüce
 * Date: 11.02.2024
 * Description: This file contains a simple logger implementation.
 */

#pragma once

#include <chrono>
#include <iostream>
#include <string>

enum class LogLevel
{
    INFO,
    WARNING,
    OFF
};

class Logger
{
  public:
    static LogLevel currentLogLevel;

    static void info(std::string message)
    {
        if (currentLogLevel > LogLevel::INFO)
            return;

        auto prefix = "[INFO]";
        print(std::move(prefix), std::move(message));
    }
    static void warning(std::string message)
    {
        if (currentLogLevel > LogLevel::WARNING)
            return;

        auto prefix = "[WARNING]";
        print(std::move(prefix), std::move(message));
    }

  private:
    static void print(const std::string&& prefix, const std::string&& message)
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << prefix << " "
                  << ": " << message << std::endl;
    }
};

LogLevel Logger::currentLogLevel = LogLevel::INFO;  // Set default log level
