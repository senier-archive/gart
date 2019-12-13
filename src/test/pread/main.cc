#include <libc/component.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define TMPFILE "/tmp/foobar"

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {
		char buf[256];
		int fd = open(TMPFILE, O_RDWR|O_CREAT);
		if (fd != -1)
			(void)unlink(TMPFILE);
		exit (read(fd, buf, 100));
	});
}
