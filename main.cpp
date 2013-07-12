#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "BufferQueue.cpp"

using namespace std;

/****************
* SETUP         *
****************/
pthread_t producer,
          cruncher,
          gobbler,
          consumer;
pthread_mutex_t amutex;
bool producer_is_done = false;
BufferQueue * produced;
BufferQueue * crunched;
BufferQueue * gobbled;
const char * FILENAME = "test.txt";
int numLines = 0;

/****************
* PRODUCER      *
****************/
void * go_producer(void * arg) {
  /* SETUP */
  cout << "GO_PRODUCER METHOD, before pthread_mutex_lock.\n";
  /* L */ pthread_mutex_lock(&amutex);
  cout << "GO_PRODUCER METHOD, at the top of pthread_mutex_lock.\n";
  
  /* Initialization */
  ifstream infile;
  infile.open(FILENAME);
  string line;

  /* Queue allocation */
  produced = new BufferQueue();
  crunched = new BufferQueue();
  gobbled = new BufferQueue();
  
  /* U */ pthread_mutex_unlock(&amutex);
  /* END SETUP */
  
  /* Reading in from file. */
  while (!infile.eof()) {
    getline(infile, line);
    numLines++;
    char * str = new char[line.length() + 1];
    strcpy(str, line.c_str());
    cout << "Waiting.";
    while (produced->isFull()) { cout << "."; }
    cout << endl;
    
    /* L */ pthread_mutex_lock(&amutex);
    produced->add(str);
    /* U */ pthread_mutex_unlock(&amutex);
  }
  
  cout << "Number of lines: " << numLines << endl;
  producer_is_done = true;
  pthread_exit(NULL);
}

void setup_producer() {
  cout << "SETUP_PRODUCER METHOD, before pthread_create.\n";
  if (pthread_create(&producer, NULL, &go_producer, NULL)) {
    printf("ERROR: Thread creation failed (THREAD: producer).\n");
    exit(-1);
  }
}

/****************
* CRUNCHER      *
****************/
void * go_cruncher(void * arg) {
  while (!producer_is_done && !produced -> isEmpty()) {
    char * crunchee;
    
    /* Reading in from queue. */
    while(produced->isEmpty()) { }
    
    /* L */ pthread_mutex_lock(&amutex);
    crunchee = produced->remove();
    /* U */ pthread_mutex_unlock(&amutex);
    
    if (crunchee != NULL) {
      /* Swapping spaces for asterisks. */
      for (int x = 0; x < strlen(crunchee); x++) { 
        crunchee[x] = (crunchee[x] == ' ') ? '*' : crunchee[x];
      }

      /* Writing to queue. */
      /* L */ pthread_mutex_lock(&amutex);
      cout << crunchee << endl;
      /* U */ pthread_mutex_unlock(&amutex);
    }
  } 
  return NULL;
}

void setup_cruncher() {
  if (pthread_create(&cruncher, NULL, &go_cruncher, NULL)) {
    printf("ERROR: Thread creation failed (THREAD: cruncher).\n");
    exit(-1);
  }
}

/****************
* GOBBLER       *
****************/
void * go_gobbler(void * arg) {
  /* Go go gadget gobbler. */
  return NULL;
}

void setup_gobbler() {
  if (pthread_create(&gobbler, NULL, &go_gobbler, NULL)) {
    printf("ERROR: Thread creation failed (THREAD: gobbler).\n");
    exit(-1);
  }
}

/****************
* CONSUMER      *
****************/
void * go_consumer(void * arg) {
  while(!producer_is_done) {}
  return NULL;
}

void setup_consumer() {
  if (pthread_create(&consumer, NULL, &go_consumer, NULL)) {
    printf("ERROR: Thread creation failed (THREAD: consumer).\n");
    exit(-1);
  }
//  if (pthread_join(consumer, NULL)) {
//    printf("ERROR: Thread joining failed (THREAD: consumer). \n");
//    exit(-1);
//  }
}

/****************
* MAIN          *
****************/
int main() {
  cout << "MAIN METHOD, before setup_producer.\n";
  setup_producer();
  cout << "MAIN METHOD, after setup_producer.\n";
//  cout << "MAIN METHOD, before setup_crucher.\n";
//  setup_cruncher();
//  cout << "MAIN METHOD, before setup_gobbler.\n";
//  setup_gobbler();
//  cout << "MAIN METHOD, before setup_consumer.\n";
//  setup_consumer();

  return 0;
}
