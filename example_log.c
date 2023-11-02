#define LOG_IMPLEMENTATION
#include "./log.h"

int main(void) {

	log_level = FATAL;
	log_fatal("%s: test test test: %d %d %d\n",__FILE__, 1, 2, 3);
	log_level = INFO;
	log_fatal("fatal log: should be shown\n");
	log_info("info log: should be shown\n");
	log_debug("debug log: shouldn't be shown\n");

	return 0;
}