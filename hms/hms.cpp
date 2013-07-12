#include "hms.h"

using namespace hms;

pthread_mutex_t m;
pthread_t a, b, c, d;
const char * buffer = "";

void * hms::producer(void * arg) {
  pthread_mutex_lock(&m);
  buffer = "hello world";
  pthread_mutex_unlock(&m);

  return NULL;
}

void * hms::crunch(void * arg) {
  pthread_mutex_lock(&m);
  buffer = "hello*world";
  pthread_mutex_unlock(&m);
  
  return NULL;
}

void * hms::gobble(void * arg) {
  pthread_mutex_lock(&m);
  buffer = "HELLO*WORLD";
  pthread_mutex_unlock(&m);
  
  return NULL;
}

void * hms::consumer(void * arg) {
  pthread_mutex_lock(&m);
  buffer = "GOODBYE!";
  pthread_mutex_unlock(&m);
  
  return NULL;
}

int main(int argc, char ** argv) {
  for (int i = 0; i < 8; i++) {
    /* Make threads. */
    pthread_create(&a, NULL, &producer, NULL);
    pthread_create(&b, NULL, &crunch, NULL);
    pthread_create(&c, NULL, &gobble, NULL);
    pthread_create(&d, NULL, &consumer, NULL);
    
    /* Join threads. */
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    pthread_join(c, NULL);
    pthread_join(d, NULL);
  }
  
  printf("I hate C++.\n");
  return 0;
}