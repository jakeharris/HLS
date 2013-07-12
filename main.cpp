#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "BufferQueue.cpp"

using namespace std;

/* Initialize variables. */
pthread_mutex_t m;
pthread_t a, b, c, d;
BufferQueue * produced;
BufferQueue * crunched;
BufferQueue * gobbled;
int numLines = 0;
bool producer_is_done = false;
bool cruncher_is_done = false;
bool gobbler_is_done = false;

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
  gobbled = new BufferQueue();
  
  pthread_mutex_unlock(&m);
  /* END SETUP */
  
  /* Reading in from file. */
  while (getline(cin, line)) {
    numLines++;
    char * str = new char[line.length() + 1];
    strcpy(str, line.c_str());
    str[min((int) strlen(str), 63)] = '\0';
    // printf("Waiting.");
    while (produced -> isFull()) { /* printf("."); */ }
    // printf("\n");
    
    pthread_mutex_lock(&m);
    produced -> add(str);
    pthread_mutex_unlock(&m);
  }

  printf("Number of lines: %u\n", numLines);
  producer_is_done = true;
  return NULL;
}

/* Crunch
 * Replace whitespace with stars. */
void * crunch(void * arg) {
  while (!producer_is_done || !produced -> isEmpty()) {
    char * crunchee;
    
    /* Reading in from queue. */
    while (produced -> isEmpty()) { if(producer_is_done) break; }
    
    pthread_mutex_lock(&m);
    crunchee = produced -> remove();
    pthread_mutex_unlock(&m);
    
    if (crunchee != NULL) {
      /* Swapping spaces for asterisks. */
      for (int x = 0; x < strlen(crunchee); x++) { 
        crunchee[x] = (crunchee[x] == ' ') ? '*' : crunchee[x];
      }

      /* Writing to queue. */
      pthread_mutex_lock(&m);
      crunched -> add(crunchee);
      pthread_mutex_unlock(&m);
    }
  }
  cruncher_is_done = true;
  return NULL;
}

/* Gobble
 * Uppercase everything. */
void * gobble(void * arg) {
  while (!cruncher_is_done || !crunched -> isEmpty()) {
    char * gobblee;
    
    /* Reading in from queue. */
    while (crunched -> isEmpty()) { if(cruncher_is_done) break; }
    
    pthread_mutex_lock(&m);
    gobblee = crunched -> remove();
    pthread_mutex_unlock(&m);
    
    if (gobblee != NULL) {
      /* Swapping lower-case letters for upper-case ones. */
      for (int x = 0; x < strlen(gobblee); x++) { 
        gobblee[x] = toupper(gobblee[x]);
      }

      /* Writing to queue. */
      pthread_mutex_lock(&m);
      gobbled -> add(gobblee);
      pthread_mutex_unlock(&m);
    }
  }
  gobbler_is_done = true;
  return NULL;
}

/* Consumer
 * Write buffer to file. */
void * consumer(void * arg) {
  while (!gobbler_is_done || !gobbled -> isEmpty()) {
    char * consumee;
    
    /* Reading in from queue. */
    while (gobbled -> isEmpty()) { if(gobbler_is_done) break; }
    
    pthread_mutex_lock(&m);
    consumee = gobbled -> remove();
    pthread_mutex_unlock(&m);
    
    if (consumee != NULL) {
      /* Print to stdout. */
      printf("%s\n", consumee);
    }
  }
  return NULL;
}

/* Main
 * Create and join threads. */
int main() {
  /* Init mutex. */
  pthread_mutex_init(&m, NULL);
  
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

  /* Destroy mutex. */
  pthread_mutex_destroy(&m);
  
  return 0;
}
