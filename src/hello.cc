#include "hello.h"
#include <glog/logging.h>

void HelloFunc() {
	printf("hello world\n");
	LOG(INFO) << "Found 6 cookies";
}
