#define LOG_IMPLEMENTATION
#include "./log.h"

int main(void) {

	printf("standard logging---------------\n");
	log_level = FATAL;
	log_fatal("%s: test test test: %d %d %d\n",__FILE__, 1, 2, 3);
	log_level = INFO;
	log_fatal("fatal log: should be shown\n");
	log_info("info log: should be shown\n");
	log_debug("debug log: shouldn't be shown\n");

	printf("stream logging------------------\n");
	FILE *fptr = fopen("example_log.out", "w");
	log_level = TRACE;
	log_ftrace(fptr, "%s:%d: trace should be shown\n", __FILE__, __LINE__);
	log_fwarn(fptr, "warning should be shown\n");
	fclose(fptr);
	log_finfo(stdout, "stream logging to stdout\n");

	printf("buffered logging-----------------------\n");
	size_t bsize = 1024*5;
	int len = 0;
	char buf[bsize];
	log_level = INFO;
	len += log_binfo(buf+len, bsize,"should be shown\n");
	len += log_bdebug(buf+len, bsize, "shouldn't be shown\n");
	len += log_berr(buf+len, bsize, "should be shown\n");
	printf("%s", buf);

	return 0;
}