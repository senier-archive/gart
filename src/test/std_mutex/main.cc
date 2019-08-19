/* Genode includes */
#include <libc/component.h>

/* stdcxx includes */
#include <mutex>

/* libc includes */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> /* 'exit'   */

/* provided by the application */
extern "C" int main(int argc, char const **argv);

volatile unsigned long long counter = 0;
std::mutex counter_mutex;

struct params {
   unsigned long long count;
   unsigned int add;
};

static
void yield (void) {
   // What is a good way to yield?
   printf("\e");
}

static
void *thread (void *arg) {
   unsigned long long c = 0;
   struct params *p = (struct params *)arg;

   for (unsigned long long i = 0; i < p->count; i++) {
      counter_mutex.lock();
      c = counter;
      yield();
      counter = c + p->add;
      counter_mutex.unlock();
   }

   return NULL;
}

void Libc::Component::construct(Libc::Env &env __attribute__((unused)))
{
   int rv;

	Libc::with_libc([&] {

      const struct params p1 = {.count = 10000, .add = 7};
      const struct params p2 = {.count = 20000, .add = 3};
      const unsigned long long expected = p1.count * p1.add + p2.count * p2.add;

      void *rv1, *rv2;
      pthread_attr_t tattr;
      pthread_t t1, t2;

      pthread_attr_init(&tattr);

      rv = pthread_create(&t1, &tattr, thread, (void *)&p1);
      if (rv) {
         printf("Error creating thread #1\n");
         exit (1);
      };

      rv = pthread_create(&t2, &tattr, thread, (void *)&p2);
      if (rv) {
         printf("Error creating thread #1\n");
         exit (1);
      };

      pthread_join(t1, &rv1);
      pthread_join(t2, &rv2);

      if (expected != counter) {
         printf("Invalid shared counter: expected %llu, got %llu\n", expected, counter);
         exit (1);
      }

      exit (0);
	});
}
