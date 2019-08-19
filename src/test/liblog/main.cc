#include <libc/component.h>
#include <stdlib.h>
#include <gtest/gtest.h>

/* provided by the application */
extern "C" int main(int argc, char const **argv);

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		char *argv[] = {
			"/bin/test_liblog",
			"--gtest_filter=-"
         "liblog.SLOG"
         ":liblog.__android_log_write__android_logger_list_read"
         ":liblog.android_logger_get_"
         ":liblog.RLOG"
         ":liblog.RLOG"
         ":liblog.__android_log_buf_print"
         ":liblog.__android_log_buf_print__concurrent64"
         ":liblog.__android_log_buf_write"
			,0
		};

      int argc = 2;
		setprogname (argv[0]);
		testing::InitGoogleTest(&argc, argv);
		exit(RUN_ALL_TESTS());
	});
}
