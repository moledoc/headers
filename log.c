
#ifndef LOG_H_
#define LOG_H_

enum LEVEL {
	FATAL = 0,
	ERROR,
	WARNING,
	INFO,
	DEBUG,
	TRACE,
}

enum log_level = INFO;

void log_fatal(char *format); // TODO: format and variadic args
void log_err(char *format); // TODO: format and variadic args
void log_warn(char *format); // TODO: format and variadic args
void log_info(char *format); // TODO: format and variadic args
void log_debug(char *format); // TODO: format and variadic args
void log_trace(char *format); // TODO: format and variadic args

#endif // LOG_H_

#ifdef LOG_IMPLEMENTATION

#endif // LOG_IMPLEMENTATION
	