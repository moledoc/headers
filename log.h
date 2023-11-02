
#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include <stdio.h>

enum LOG_LEVEL {
	FATAL = 0,
	ERROR,
	WARNING,
	INFO,
	DEBUG,
	TRACE
};

static const char *LOG_LEVEL_NAMES[] = {
	"[FATAL]: ",
	"[ERROR]: ",
	"[WARNING]: ",
	"[INFO]: ",
	"[DEBUG]: ",
	"[TRACE]: "
};

enum LOG_LEVEL log_level = INFO;

void log_fatal(char *format, ...);
void log_err(char *format, ...);
void log_warn(char *format, ...);
void log_info(char *format, ...);
void log_debug(char *format, ...);
void log_trace(char *format, ...);

#endif // LOG_H_

#ifdef LOG_IMPLEMENTATION


void log_log(FILE *stream, enum LOG_LEVEL lvl, char *fmt, va_list args) {
	if (log_level >= lvl) {
		fprintf(stream, LOG_LEVEL_NAMES[lvl]);
		vfprintf(stream, fmt, args);
	}

}

void log_fatal(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(stderr, FATAL, fmt, args);
	va_end(args);
}

void log_err(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(stderr, ERROR, fmt, args);
	va_end(args);
}

void log_warn(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(stdout, WARNING, fmt, args);
	va_end(args);
}

void log_info(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(stdout, INFO, fmt, args);
	va_end(args);
}

void log_debug(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(stdout, DEBUG, fmt, args);
	va_end(args);
}

void log_trace(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(stdout, TRACE, fmt, args);
	va_end(args);
}

#endif // LOG_IMPLEMENTATION
	