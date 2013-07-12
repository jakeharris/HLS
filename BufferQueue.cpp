/* This Program is to create a buffer queue of size 10 that stores
pointers to character arrays of read in strings. */

#include <iostream>

using namespace std;

class BufferQueue { 
    static const int X = 10; // the constant size of the buffer
    char* buffer[X];         // declaration of the buffer
    int head;                // tracks the head of the buffer
    int tail;                // tracks the tail of the buffer
  
  public:
    BufferQueue();
    void add(char sInput[]); // adds an item to the buffer
    char* remove();          // removes an item from the buffer
    bool isFull();           // returns true if buffer array is full
    bool isEmpty();          // returns true if buffer array is empty
    char* getHead();         // returns the head of the buffer
};

/* Constructor */
BufferQueue::BufferQueue(){
  head = -1;
  tail = -1;
}

void BufferQueue::add(char sInput[]) {
  /* Check to see if the buffer is full, if not then increment tail. */
  if (isEmpty()) {
    head = 0;
    tail = 0;
    buffer[head] = sInput;
    tail++;
  }
  
  else if (tail <= 9 && head != tail) {
    buffer[tail] = sInput;
    tail++;
  }
  
  /* If tail has reached the end check if the first index of the buffer is available. */
  else if ((tail > 9) && (head != 0)) {
    tail = 0;
    buffer[tail] = sInput;
    tail++;
  }
  
  /* If neither then the buffer is full. */
  else {
    printf("Error: Buffer is full.\n");
  }
}

char* BufferQueue::remove() {
  /* If head == tail then buffer is empty. */
  char* output;
  if (buffer[head] == NULL) {
    printf("Error: Buffer is empty.\n");
    return (char *) "";
  }
  
  /* If head has reached the end check if the first index of the buffer is available; if so store in first index. */
  else if ((head == 9) && (tail != 0)) {
    output = buffer[head];
    buffer[head] = NULL;
    head = 0;
  }
  
  /* Else remove current head index from buffer, and increment head. */
  else {
    output = buffer[head];
    buffer[head] = NULL;
    head++;
  }
  
  if (head == tail + 1) { head = -1; tail = -1; }
  return output;
}

bool BufferQueue::isFull() {
  for (int x = 0; x < 10; x++) {
    if(buffer[x] == NULL) return false;
  }
  return true;
}

bool BufferQueue::isEmpty() {
  return (head == -1 || tail == -1);
}

char * BufferQueue::getHead() {
  return buffer[head];
}
