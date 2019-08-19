/* Genode includes */
#include <libc/component.h>

/* libc includes */
#include <stdlib.h> /* 'exit'   */

#include <gtest/gtest.h>

extern int          genode_argc;
extern const char **genode_argv;

/* provided by the application */
extern "C" int main(int argc, char const **argv);

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		char const *argv[] = {
			"/bin/test_liblog",
			"--gtest_filter=-liblog.__android_log_buf_print__concurrent64",
			0
		};

		genode_argc = 2;
		genode_argv = argv;

		setprogname (genode_argv[0]);

		testing::InitGoogleTest(&genode_argc, (char **)genode_argv);
		exit(RUN_ALL_TESTS());
	});
}
