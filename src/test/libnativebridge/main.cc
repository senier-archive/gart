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
			"/bin/test_libnativehelper",
			0
		};

		genode_argc = 1;
		genode_argv = argv;

		setprogname (genode_argv[0]);

		testing::InitGoogleTest(&genode_argc, (char **)genode_argv);
		int rv = chdir("/tmp");
		if (rv < 0) {
			Genode::log ("Error changing to /tmp");
			exit (1);
		}
		exit(RUN_ALL_TESTS());
	});
}
