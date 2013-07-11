/* This Program is to create a buffer queue of size 10 that stores
pointers to character arrays of read in strings */
#include <iostream>
using namespace std;

class BufferQueue{
    static const int X = 10;        //the constant size of the buffer
    char* buffer[X];   //declaration of the buffer
    int head;      //tracks the head of the buffer
    int tail;      //tracks the tail of the buffer
  public:
    BufferQueue();
    void add(char sInput[]); //adds an item to the buffer
    void remove();            //removes an item from the buffer
};
/* Constructor */
BufferQueue::BufferQueue(){
  head = 0;
  tail = 0;
}
void BufferQueue::add(char sInput[]){
/* Check to see if the buffer is full, if not then
increment tail. */
  if(tail < 9){
    buffer[tail] = sInput;
    tail++;
  }
/* If tail has reached the end check if the first
index of the buffer is available. */
  else if((tail == 9) && (head != 0)){
    tail = 0;
    buffer[tail] = sInput;
    tail++;
  }
/* If neither then the buffer is full. */
  else{
    cout << "Error: Buffer is full.\n";
  }
}

void BufferQueue::remove(){
/* If head == tail then buffer is empty. */
  if(head == tail){
    cout << "Error: Buffer is empty.\n";
  }
/* If head has reached the end check if the first index of
the buffer is available; if so store in first index. */
  else if((head == 9) && (tail != 0)){
    head = 0;
    buffer[head] = NULL;
    head++;
  }
/* Else remove current head index from buffer, and increment head. */
  else{
    buffer[head] = NULL;
    head++;
  }
}

