
// {
#ifdef LOG // HEADER
#pragma once

#include <stdarg.h>
#include <stdio.h>

enum LOG_LEVEL { FATAL = 0, ERROR, WARNING, INFO, DEBUG, TRACE };

static const char *LOG_LEVEL_NAMES[] = {"[FATAL]: ", "[ERROR]: ", "[WARNING]: ",
                                        "[INFO]: ",  "[DEBUG]: ", "[TRACE]: "};

// log_level is global variable that allows changing log levels; default is INFO
enum LOG_LEVEL log_level = INFO;

// LOG_TMP_BUF_SIZE is temporary buffer's size where the message is stored
// internally; default size if 1024
int LOG_TMP_BUF_SIZE = 1024;

// log_log logs message with level and format to stderr
int log_log(enum LOG_LEVEL lvl, char *fmt, ...);

// log_flog logs message with level and format to FILE
int log_flog(FILE *stream, enum LOG_LEVEL lvl, char *fmt, ...);

// log_blog logs message with level and format to buffer
int log_blog(char *buf, size_t bsize, enum LOG_LEVEL lvl, char *fmt, ...);

// log_<lvl> logs message with format at <lvl> to stderr
int log_fatal(char *format, ...);
int log_err(char *format, ...);
int log_warn(char *format, ...);
int log_info(char *format, ...);
int log_debug(char *format, ...);
int log_trace(char *format, ...);

// log_f<lvl> logs message with format at <lvl> to FILE
int log_ffatal(FILE *f, char *format, ...);
int log_ferr(FILE *f, char *format, ...);
int log_fwarn(FILE *f, char *format, ...);
int log_finfo(FILE *f, char *format, ...);
int log_fdebug(FILE *f, char *format, ...);
int log_ftrace(FILE *f, char *format, ...);

// log_b<lvl> logs message with format at <lvl> to buffer
int log_bfatal(char *b, size_t bsize, char *format, ...);
int log_berr(char *b, size_t bsize, char *format, ...);
int log_bwarn(char *b, size_t bsize, char *format, ...);
int log_binfo(char *b, size_t bsize, char *format, ...);
int log_bdebug(char *b, size_t bsize, char *format, ...);
int log_btrace(char *b, size_t bsize, char *format, ...);

#endif // LOG // HEADER
// }

// {
#ifdef LOG // IMPLEMENTATION

int _log_flog(FILE *stream, enum LOG_LEVEL lvl, char *fmt, va_list args) {
  int n = 0;
  if (log_level >= lvl) {
    n += fprintf(stream, "%s", LOG_LEVEL_NAMES[lvl]);
    n += vfprintf(stream, fmt, args);
  }
  return n;
}

int _log_log(enum LOG_LEVEL lvl, char *fmt, va_list args) {
  return _log_flog(stderr, lvl, fmt, args);
}

int _log_blog(char *buf, size_t bsize, enum LOG_LEVEL lvl, char *fmt,
              va_list args) {
  int n = 0;
  if (log_level >= lvl) {
    char *lvl_name = (char *)LOG_LEVEL_NAMES[lvl];
    size_t i = 0;
    while (*(lvl_name++) != '\0') {
      ++i;
    }
    char tmp[LOG_TMP_BUF_SIZE];
    n += snprintf(buf, bsize - i, "%s", LOG_LEVEL_NAMES[lvl]);
    int n_tmp = vsnprintf(tmp, LOG_TMP_BUF_SIZE, fmt, args);
    n += vsnprintf(buf + i, bsize - n_tmp, fmt, args);
  }
  return n;
}

int log_flog(FILE *stream, enum LOG_LEVEL lvl, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(stream, lvl, fmt, args);
  va_end(args);
  return n;
}

int log_log(enum LOG_LEVEL lvl, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_log(lvl, fmt, args);
  va_end(args);
  return n;
}

int log_blog(char *buf, size_t bsize, enum LOG_LEVEL lvl, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_blog(buf, bsize, lvl, fmt, args);
  va_end(args);
  return n;
}

int log_fatal(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(stderr, FATAL, fmt, args);
  va_end(args);
  return n;
}

int log_err(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(stderr, ERROR, fmt, args);
  va_end(args);
  return n;
}

int log_warn(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(stderr, WARNING, fmt, args);
  va_end(args);
  return n;
}

int log_info(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(stderr, INFO, fmt, args);
  va_end(args);
  return n;
}

int log_debug(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(stderr, DEBUG, fmt, args);
  va_end(args);
  return n;
}

int log_trace(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(stderr, TRACE, fmt, args);
  va_end(args);
  return n;
}

int log_ffatal(FILE *f, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(f, FATAL, fmt, args);
  va_end(args);
  return n;
}

int log_ferr(FILE *f, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(f, ERROR, fmt, args);
  va_end(args);
  return n;
}

int log_fwarn(FILE *f, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(f, WARNING, fmt, args);
  va_end(args);
  return n;
}

int log_finfo(FILE *f, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(f, INFO, fmt, args);
  va_end(args);
  return n;
}

int log_fdebug(FILE *f, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(f, DEBUG, fmt, args);
  va_end(args);
  return n;
}

int log_ftrace(FILE *f, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_flog(f, TRACE, fmt, args);
  va_end(args);
  return n;
}

int log_bfatal(char *b, size_t bsize, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_blog(b, bsize, FATAL, fmt, args);
  va_end(args);
  return n;
}

int log_berr(char *b, size_t bsize, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_blog(b, bsize, ERROR, fmt, args);
  va_end(args);
  return n;
}

int log_bwarn(char *b, size_t bsize, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_blog(b, bsize, WARNING, fmt, args);
  va_end(args);
  return n;
}

int log_binfo(char *b, size_t bsize, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_blog(b, bsize, INFO, fmt, args);
  va_end(args);
  return n;
}

int log_bdebug(char *b, size_t bsize, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_blog(b, bsize, DEBUG, fmt, args);
  va_end(args);
  return n;
}

int log_btrace(char *b, size_t bsize, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = _log_blog(b, bsize, TRACE, fmt, args);
  va_end(args);
  return n;
}

#endif // LOG // IMPLEMENTATION

// }