#include <libc/component.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {
      int fd = open ("/tmp/stderr-1", O_CREAT|O_RDWR);
      dup2(fd, 2);
      close (fd);
      fd = open ("/tmp/stderr-2", O_CREAT|O_RDWR);
      dup2(fd, 2);
      exit(0);
	});
}
