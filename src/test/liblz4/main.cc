/* Genode includes */
#include <libc/component.h>

/* libc includes */
#include <stdlib.h> /* 'exit'   */
#include <string.h>

/* provided by the application */
extern "C" int main(int argc, const char **argv);

Genode::size_t Libc::Component::stack_size() {
	return 500*1024;
}

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		char const *argv[] = {
			"/bin/test_liblz4",
			0
		};
      int argc = 1;

		setprogname (argv[0]);
		main(argc, argv);
		exit(0);
	});
}
