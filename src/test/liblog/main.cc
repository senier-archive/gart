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
            ":liblog.__android_log_write__android_logger_list_read" // Reading logs is unsupported
            ":liblog.android_logger_get_"                           // Reading logs is unsupported
#ifdef __i386__
			":liblog.__android_log_buf_print__concurrent64"         // Error on seL4/x86_32, investigate
#endif
			,0
		};

      int argc = 2;
		setprogname (argv[0]);
		testing::InitGoogleTest(&argc, argv);
		exit(RUN_ALL_TESTS());
	});
}
