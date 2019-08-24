#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int posix_memalign(void **memptr, size_t alignment, size_t size) {
   fprintf(stderr, "FIXME: posix_memalign implemented without alignment\n");
   *memptr = malloc(size);
   return *memptr ? 0 : ENOMEM;
}
