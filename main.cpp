#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "BufferQueue.cpp"

using namespace std;

/* Initialize variables. */
pthread_mutex_t m;
pthread_t a, b, c, d;
bool producer_is_done = false;
BufferQueue * produced;
BufferQueue * crunched;
// BufferQueue * gobbled;
int numLines = 0;

/* Producer
 * Read lines from stdin. */
void * producer(void * arg) {
  /* SETUP */
  pthread_mutex_lock(&m);
  
  /* Initialization */
  string line;

  /* Queue allocation */
  produced = new BufferQueue();
  crunched = new BufferQueue();
//  gobbled = new BufferQueue();
  
  pthread_mutex_unlock(&m);
  /* END SETUP */
  
  /* Reading in from file. */
  while (getline(cin, line)) {
    numLines++;
    char * str = new char[line.length() + 1];
    strcpy(str, line.c_str());
    printf("Waiting.");
    int x = 0;
    while (produced -> isFull() && x < 10 ) { printf("."); x++; }
    printf("\n");
    
    pthread_mutex_lock(&m);
    produced -> add(str);
    pthread_mutex_unlock(&m);
  }

  printf("Number of lines: %u\n", numLines);
  producer_is_done = true;
  pthread_exit(NULL);
  return NULL;
}

/* Crunch
 * Replace whitespace with stars. */
void * crunch(void * arg) {
//  while (!producer_is_done && !produced -> isEmpty()) { // <-- THIS LINE CAUSES A SEGFAULT!!!
  while (!producer_is_done) {
    char * crunchee;
    
    /* Reading in from queue. */
//    while (produced -> isEmpty()) {}
    
//    pthread_mutex_lock(&m);
//    crunchee = produced -> remove();
//    pthread_mutex_unlock(&m);
//    
//    if (crunchee != NULL) {
//      /* Swapping spaces for asterisks. */
//      for (int x = 0; x < strlen(crunchee); x++) { 
//        crunchee[x] = (crunchee[x] == ' ') ? '*' : crunchee[x];
//      }
//
//      /* Writing to queue. */
//      pthread_mutex_lock(&m);
//      printf("%s\n", crunchee);
//      pthread_mutex_unlock(&m);
//    }
  }
  return NULL;
}

/* Gobble
 * Uppercase everything. */
//void * gobble(void * arg) {
//  return NULL;
//}

/* Consumer
 * Write buffer to file. */
//void * consumer(void * arg) {
//  while(!producer_is_done) {}
//  return NULL;
//}

/* Main
 * Create and join threads. */
int main() {
  /* Make threads. */
  pthread_create(&a, NULL, &producer, NULL);
  pthread_create(&b, NULL, &crunch, NULL);
//  pthread_create(&c, NULL, &gobble, NULL);
//  pthread_create(&d, NULL, &consumer, NULL);
  
  /* Join threads. */
  pthread_join(a, NULL);
  pthread_join(b, NULL);
//  pthread_join(c, NULL);
//  pthread_join(d, NULL);

  return 0;
}
