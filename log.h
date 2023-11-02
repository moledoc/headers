
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
int LOG_TMP_BUF_SIZE = 1024;

int log_fatal(char *format, ...);
int log_err(char *format, ...);
int log_warn(char *format, ...);
int log_info(char *format, ...);
int log_debug(char *format, ...);
int log_trace(char *format, ...);

int log_ffatal(FILE *f, char *format, ...);
int log_ferr(FILE *f, char *format, ...);
int log_fwarn(FILE *f, char *format, ...);
int log_finfo(FILE *f, char *format, ...);
int log_fdebug(FILE *f, char *format, ...);
int log_ftrace(FILE *f, char *format, ...);

int log_bfatal(char *b, size_t bsize, char *format, ...);
int log_berr(char *b, size_t bsize, char *format, ...);
int log_bwarn(char *b, size_t bsize, char *format, ...);
int log_binfo(char *b, size_t bsize, char *format, ...);
int log_bdebug(char *b, size_t bsize, char *format, ...);
int log_btrace(char *b, size_t bsize, char *format, ...);

#endif // LOG_H_

#ifdef LOG_IMPLEMENTATION


int _log_log(FILE *stream, enum LOG_LEVEL lvl, char *fmt, va_list args) {
	int n = 0;
	if (log_level >= lvl) {
		n += fprintf(stream, LOG_LEVEL_NAMES[lvl]);
		n += vfprintf(stream, fmt, args);
	}
	return n;
}

int _log_blog(char *buf, size_t bsize, enum LOG_LEVEL lvl, char *fmt, va_list args) {
	int n = 0;
	if (log_level >= lvl) {
		char *lvl_name = (char *)LOG_LEVEL_NAMES[lvl];
		size_t i = 0;
		while (*(lvl_name++) != '\0') {
			++i;
		}
		char tmp[LOG_TMP_BUF_SIZE];
		n += snprintf(buf, bsize-i, LOG_LEVEL_NAMES[lvl]);
		int n_tmp = vsnprintf(tmp, LOG_TMP_BUF_SIZE, fmt, args);
		n += vsnprintf(buf+i, bsize-n_tmp, fmt, args);
	}
	return n;
}

int log_fatal(char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(stderr, FATAL, fmt, args);
	va_end(args);
	return n;
}

int log_err(char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(stderr, ERROR, fmt, args);
	va_end(args);
	return n;
}

int log_warn(char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(stdout, WARNING, fmt, args);
	va_end(args);
	return n;
}

int log_info(char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(stdout, INFO, fmt, args);
	va_end(args);
	return n;
}

int log_debug(char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(stdout, DEBUG, fmt, args);
	va_end(args);
	return n;
}

int log_trace(char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(stdout, TRACE, fmt, args);
	va_end(args);
	return n;
}

int log_ffatal(FILE *f, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(f, FATAL, fmt, args);
	va_end(args);
	return n;
}

int log_ferr(FILE *f, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(f, ERROR, fmt, args);
	va_end(args);
	return n;
}

int log_fwarn(FILE *f, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(f, WARNING, fmt, args);
	va_end(args);
	return n;
}

int log_finfo(FILE *f, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(f, INFO, fmt, args);
	va_end(args);
	return n;
}

int log_fdebug(FILE *f, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(f, DEBUG, fmt, args);
	va_end(args);
	return n;
}

int log_ftrace(FILE *f, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	_log_log(f, TRACE, fmt, args);
	va_end(args);
	return n;
}

int log_bfatal(char *b, size_t bsize, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	n += _log_blog(b, bsize, FATAL, fmt, args);
	va_end(args);
	return n;
}

int log_berr(char *b, size_t bsize, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	n += _log_blog(b, bsize, ERROR, fmt, args);
	va_end(args);
	return n;
}

int log_bwarn(char *b, size_t bsize, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	n += _log_blog(b, bsize, WARNING, fmt, args);
	va_end(args);
	return n;
}

int log_binfo(char *b, size_t bsize, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	n += _log_blog(b, bsize, INFO, fmt, args);
	va_end(args);
	return n;
}

int log_bdebug(char *b, size_t bsize, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	n += _log_blog(b, bsize, DEBUG, fmt, args);
	va_end(args);
	return n;
}

int log_btrace(char *b, size_t bsize, char *fmt, ...) {
	int n = 0;
	va_list args;
	va_start(args, fmt);
	n += _log_blog(b, bsize, TRACE, fmt, args);
	va_end(args);
	return n;
}

#endif // LOG_IMPLEMENTATION