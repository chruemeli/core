#include "Logger.h"

Logger::Logger() {
    this->logFile = DEFAULT_LOG_FILE;
    this->logSeverity = DEFAULT_LOG_SEVERITY;
}

String Logger::getLogFile() {
    return this->logFile;
}

void Logger::log(LogSeverity severity, const String tag, const String message) {
    if(severity >= this->logSeverity) {
        char * method = "w"; // write to log file

        if(LittleFS.exists(this->logFile)) {
            File f = LittleFS.open(this->logFile, "r");
            if(f) {
                if(f.size() <= LOG_MAX_FILE_SIZE) {
                    method = "a"; // append to logfile
                }
                f.close();
            }
        }

        File f = LittleFS.open(this->logFile, method);
        if(f) {
            this->logTime(f);

            f.print(F(" - "));

            switch(severity) {
                case DEBUG:
                    f.print(F("DEBUG"));
                    break;
                case INFO:
                    f.print(F("INFO"));
                    break;
                case WARNING:
                    f.print(F("WARNING"));
                    break;
                case ERROR:
                    f.print(F("ERROR"));
                    break;
            }

            f.print(F(" - "));
            f.print(tag);
            f.print(F(": "));
            f.println(message);

            f.close();
        }
    }
}

void Logger::logTime(File &file) {
    /*unsigned long time = millis() / 1000;

    uint16_t seconds = time % 60;

    time = (time - seconds) / 60;
    uint8_t minutes = time % 60;

    time = (time - minutes) / 60;
    uint8_t hours = time;

    if(hours < 10) file.print("0");
    file.print(hours);  

    // separator
    file.print(F(":"));

    if(minutes < 10) file.print("0");
    file.print(minutes);  

    // separator
    file.print(F(":"));

    if(seconds < 10) file.print("0");
    file.print(seconds);  */
}

/** STATIC METHODS **/

Logger& Logger::instance() {
    static Logger _instance;
    return _instance;
}

void Logger::debug(const String tag, const String message) {
    Logger& logger = Logger::instance();

    logger.log(DEBUG, tag, message);
}

void Logger::debug(const String tag, const char *fmt, ...) {
    Logger& logger = Logger::instance();
    char buff[LOGGER_PRINTF_MAX_BUFF];

    /* pointer to the variable arguments list */
    va_list pargs;
    
    /* Initialise pargs to point to the first optional argument */
    va_start(pargs, fmt);  
    
    /* create the formatted data and store in buff */
    vsnprintf(buff, LOGGER_PRINTF_MAX_BUFF, fmt, pargs);
    va_end(pargs);

    logger.log(DEBUG, tag, buff);
}

void Logger::info(const String tag, const String message) {
    Logger& logger = Logger::instance();

    logger.log(INFO, tag, message);
}

void Logger::info(const String tag, const char *fmt, ...) {
    Logger& logger = Logger::instance();
    char buff[LOGGER_PRINTF_MAX_BUFF];

    /* pointer to the variable arguments list */
    va_list pargs;
    
    /* Initialise pargs to point to the first optional argument */
    va_start(pargs, fmt);  
    
    /* create the formatted data and store in buff */
    vsnprintf(buff, LOGGER_PRINTF_MAX_BUFF, fmt, pargs);
    va_end(pargs);

    logger.log(INFO, tag, buff);
}

void Logger::warning(const String tag, const String message) {
    Logger& logger = Logger::instance();

    logger.log(WARNING, tag, message);
}

void Logger::error(const String tag, const String message) {
    Logger& logger = Logger::instance();

    logger.log(ERROR, tag, message);
}