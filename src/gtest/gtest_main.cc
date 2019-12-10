/* Genode includes */
#include <base/log.h>

/* libc include */
#include <stdio.h>

/* Googletest includes */
#include <gtest/gtest.h>

int main (int argc, const char **argv) {
	FILE *f = tmpfile();
	if (f == nullptr) {
		Genode::error("Tmpfile not working (/tmp present and writeable?).");
		exit(255);
	}
	testing::InitGoogleTest(&argc, (char **)argv);
	return RUN_ALL_TESTS();
}
