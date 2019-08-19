/* Genode includes */
#include <libc/component.h>

/* libc includes */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> /* 'exit'   */

static
void yield (void) {
   // What is a good way to yield?
   printf("\e");
}

volatile unsigned long long counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

struct params {
   int id;
   unsigned long long count;
   unsigned int add;
   int depth;
};

void increment_locked (int delta, int depth) {
   if (depth > 0) {
      pthread_mutex_lock (&counter_mutex);
      unsigned long long c = counter;
      yield();
      counter = c + delta;
      increment_locked (delta, depth - 1);
      pthread_mutex_unlock (&counter_mutex);
   }
}

static
void *thread (void *arg) {
   struct params *p = (struct params *)arg;
   for (unsigned long long i = 0; i < p->count; i++) {
      increment_locked (p->add, p->depth);
   }
   return NULL;
}

int main() {
   int rv;

   const struct params p1 = {.id = 1, .count = 10000, .add = 7, .depth = 3};
   const struct params p2 = {.id = 2, .count = 20000, .add = 3, .depth = 7};
   const unsigned long long expected = p1.depth * p1.count * p1.add + p2.depth * p2.count * p2.add;

   void *rv1, *rv2;
   pthread_attr_t tattr;
   pthread_t t1, t2;

   pthread_mutexattr_t attrs;
   pthread_mutexattr_init (&attrs);
   pthread_mutexattr_settype (&attrs, PTHREAD_MUTEX_RECURSIVE);
   pthread_mutex_init (&counter_mutex, &attrs);
   pthread_mutexattr_destroy(&attrs);

   pthread_attr_init(&tattr);

   rv = pthread_create(&t1, &tattr, thread, (void *)&p1);
   if (rv) {
      printf("Error creating thread #1\n");
      return 1;
   };

   rv = pthread_create(&t2, &tattr, thread, (void *)&p2);
   if (rv) {
      printf("Error creating thread #1\n");
      return 1;
   };

   pthread_join(t1, &rv1);
   pthread_join(t2, &rv2);

   if (expected != counter) {
      printf("Invalid shared counter: expected %llu, got %llu\n", expected, counter);
      return 1;
   }

   printf ("\nOK\n");
   return 0;
}

void Libc::Component::construct(Libc::Env &env __attribute__((unused)))
{
	Libc::with_libc([&] {
      exit(main());
	});
}
