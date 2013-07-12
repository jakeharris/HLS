#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "BufferQueue.cpp"

using namespace std;

  pthread_t producer,
            cruncher,
            gobbler,
            consumer;
  pthread_mutex_t mutex;
  bool producer_is_done = false;
  BufferQueue * produced;
  BufferQueue * crunched;
  BufferQueue * gobbled;
  const char * FILENAME = "test.txt";
  int numLines = 0;

void * go_producer(void *arg){
  /* SETUP */
  /* LOCK */ pthread_mutex_lock(&mutex);

  /* Initialization */
  ifstream infile;
  infile.open(FILENAME);
  string line;

  /* Queue allocation */
  produced = new BufferQueue();
  crunched = new BufferQueue();
  gobbled = new BufferQueue();
  /* UNLOCK */ pthread_mutex_unlock(&mutex);
  /* END SETUP */


  /* Reading in from file */
  while(!infile.eof()){
    getline(infile, line);
    numLines++;
    char * str = new char[line.length() + 1];
    strcpy(str, line.c_str());
    cout << "Waiting.";
    while(produced->isFull()){ cout << ".";}
    cout << endl;
    /* LOCK */ pthread_mutex_lock(&mutex);
    produced->add(str);
    /* UNLOCK */ pthread_mutex_unlock(&mutex);
  }
  cout << "Number of lines: " << numLines << endl;
  producer_is_done = true;
  pthread_exit(NULL);
}

void setup_producer(){
  if(pthread_create(&producer, NULL, &go_producer, NULL)){
    printf("ERROR: Thread creation failed (THREAD: producer).\n");
    exit(-1);
  }
/*  if(pthread_join(producer, NULL)){
    printf("ERROR: Thread joining failed (THREAD: producer).\n");
    return -1;
  }
*/
}
void * go_cruncher(void* arg){
  
  while(!producer_is_done && !produced->isEmpty()){
    char* crunchee;
    /* Reading in from queue */
    while(produced->isEmpty()){ }
    /* LOCK */ pthread_mutex_lock(&mutex);
    crunchee = produced->remove();
    /* UNLOCK */ pthread_mutex_unlock(&mutex);
    if(crunchee != NULL){
      /* Swapping spaces for asterisks */
      for(int x = 0; x < strlen(crunchee); x++){ 
        crunchee[x] = (crunchee[x] == ' ') ? '*' : crunchee[x];
      }

      /* Writing to queue */
      /* LOCK */ pthread_mutex_lock(&mutex);
      cout << crunchee << endl;
      /* UNLOCK */ pthread_mutex_unlock(&mutex);
    }
  }
}


void setup_cruncher(){
  if(pthread_create(&cruncher, NULL, &go_cruncher, NULL)){
    printf("ERROR: Thread creation failed (THREAD: cruncher).\n");
    exit(-1);
  }
/*  if(pthread_join(cruncher, NULL)){
    printf("ERROR: Thread joining failed (THREAD: cruncher). \n");
    return -1;
  }
*/
}

void * go_gobbler(void* arg){ /* go go gadget gobbler */

}

void setup_gobbler(){
  if(pthread_create(&gobbler, NULL, &go_gobbler, NULL)){
    printf("ERROR: Thread creation failed (THREAD: gobbler).\n");
    exit(-1);
  }
/*  if(pthread_join(gobbler, NULL)){
    printf("ERROR: Thread joining failed (THREAD: gobbler). \n");
    return -1;
  }
*/
}

void * go_consumer(void* arg){
  while(!producer_is_done){

  }
}

void setup_consumer(){
  if(pthread_create(&consumer, NULL, &go_consumer, NULL)){
    printf("ERROR: Thread creation failed (THREAD: consumer).\n");
    exit(-1);
  }
  if(pthread_join(consumer, NULL)){
    printf("ERROR: Thread joining failed (THREAD: consumer). \n");
    exit(-1);
  }
}


int main() {
  setup_producer();
  setup_cruncher();
  setup_gobbler();
  setup_consumer();

  return 0;
}

