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

		char const *argv[] = {
			"/bin/test_libbase",
			"--gtest_filter=-"
         "logging.LOG_STREAM_FATAL_WITHOUT_ABORT_*"
         ":logging.LOG_STREAM_*"
         ":logging.LOG_FATAL_WITHOUT_ABORT_*"
         ":logging.LOG_*_*"
         ":logging.*LOG_does_not_clobber_errno"
         ":logging.PLOG_FATAL_WITHOUT_ABORT_*"
         ":logging.PLOG_*_*"
         ":logging.UNIMPLEMENTED"
         ":logging.SetDefaultTag"
         ":file.WriteStringToFile2"
         ":file.ReadFileToString_capacity",
			0
		};

		int argc = 2;
		setprogname (argv[0]);
      testing::InitGoogleTest(&argc, (char **)argv);
      exit(RUN_ALL_TESTS());
	});
}
