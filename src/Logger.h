#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <LittleFS.h>
#include <stdarg.h>

#ifndef DEFAULT_LOG_SEVERITY
    #define DEFAULT_LOG_SEVERITY INFO
#endif

#ifndef DEFAULT_LOG_FILE
    #define DEFAULT_LOG_FILE "/log"
#endif

#ifndef LOG_MAX_FILE_SIZE
    #define LOG_MAX_FILE_SIZE 10000
#endif

#define LOGGER_PRINTF_MAX_BUFF 256
#define ONE_HOUR 3600
#define ONE_MINUTE 60

enum LogSeverity {
    DEBUG   = 0x0,
    INFO    = 0x1,
    WARNING = 0x10,
    ERROR   = 0x11
};

class Logger {
    private:
        LogSeverity logSeverity;
        String logFile;

        Logger();                             // No external 
        Logger( const Logger& );              // No copy-constructor
        Logger & operator = (const Logger &); // No copy
    public:
        String getLogFile();
        void log(const LogSeverity severity, const String tag, const String message);
        void logTime(File &file);

        static Logger& instance();

        static void debug(const String tag, const String message);
        static void debug(const String tag, const char *fmt, ...);

        static void info(const String tag, const String message);
        static void info(const String tag, const char *fmt, ...);

        static void warning(const String tag, const String message);
        static void error(const String tag, const String message);
};

#endif