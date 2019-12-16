#include <libc/component.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define TMPFILE "/tmp/foobar"

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {
		char buf[256];
		int fd = open(TMPFILE, O_RDWR|O_CREAT);
		ssize_t rc = pread(fd, buf, 100, -1000);
		printf("rc=%ld\n", rc);
	});
}
