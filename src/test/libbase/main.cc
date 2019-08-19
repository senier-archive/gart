/* Genode includes */
#include <libc/component.h>

/* Googletest includes */
#include <gtest/gtest.h>

/* libc includes */
#include <stdio.h>
#include <stdlib.h>

/* provided by the application */
extern "C" int main(int argc, char const **argv);

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

      // FIXME: genodelabs/genode#3476 prevents some tests from running. Retry when fixed.
		char const *argv[] = {
			"/bin/test_libbase",
			"--gtest_filter=-"
         ":logging.LOG_*_*"
         ":logging.PLOG_*_*"
         ":logging.UNIMPLEMENTED"
         ":file.WriteStringToFile2"
         ":file.ReadFileToString_capacity",
			0
		};

		int argc = 2;
		setprogname (argv[0]);
      exit(main(argc, argv));
	});
}
