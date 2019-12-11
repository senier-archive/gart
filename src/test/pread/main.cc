#include <libc/component.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {
		char buf[256];
		int fd = fileno(tmpfile());
		int rc = pread(fd, buf, 100, 0);
		printf("rc=%d\n", rc);
      	exit(0);
	});
}
