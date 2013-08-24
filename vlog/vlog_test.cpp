#include <stdio.h>
#include "include/vlog.h"

using namespace suncl;
using namespace std;

int main()
{
	Vlog_module log("vlog_test");
	VLOG_DBG(log, "hello world");
	VLOG_ERR(log, "hello world");
	VLOG_INFO(log, "hello world");
	VLOG_EMER(log, "hello world");
	VLOG_WARN(log, "hello world");
	return 0;
}
