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
         ":logging.LOG_*_*"                                    // Componolit/Workarounds#4
         ":logging.PLOG_*_*"                                   // Componolit/Workarounds#4
         ":logging.UNIMPLEMENTED"                              // Componolit/Workarounds#4
         ":file.WriteStringToFile2"                            // Testing UNIX permissions, pointless on Genode
         ":file.ReadFileToString_capacity"                     // When reading a string, the test expects the result to
                                                               // only have a capacity only slightly larger than the
                                                               // string (16 bytes). On Genode some exponential
                                                               // algorithm seems to be in place violating this
                                                               // assumption (33554432 vs 16789577 bytes). We don't
                                                               // care for now.
         ":file.ReadFileToString_WriteStringToFile_symlink"    // Componolit/gart#34
			,0
		};

		int argc = 2;
		setprogname (argv[0]);
      exit(main(argc, argv));
	});
}
