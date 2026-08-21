#ifndef LOGGERCONFIG_H
#define LOGGERCONFIG_H

// Output destinations
#define LOG_WRITE_TO_FILE              1
#define LOG_WRITE_TO_CONSOLE           1

// Entry fields
#define LOG_INCLUDE_TIMESTAMP          1
#define LOG_INCLUDE_CATEGORY           1
#define LOG_INCLUDE_PROCESS_ID         1
#define LOG_INCLUDE_THREAD_ID          1
#define LOG_INCLUDE_SOURCE_LOCATION    1
#define LOG_INCLUDE_FUNCTION_NAME      1

// Behavior
#define LOG_FLUSH_EVERY_MESSAGE        1
#define LOG_REGISTER_CATEGORIES        1
#define LOG_INDENT_MULTILINE_MESSAGES  1

// Optional verbose-debug category
#define LOG_ENABLE_HEAVY_DEBUG         1

#endif // LOGGERCONFIG_H
