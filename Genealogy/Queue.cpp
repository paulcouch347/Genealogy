#include "Queue.h"

Queue::Queue() {
	count = 0;
	front = NULL;
	back = NULL;
}//end default constructor

bool Queue::isEmpty() {
	return count == 0;
}//end isEmpty

int Queue::getCount() {
	return count;
}//end getCount

void Queue::add(int data) {
	count++;
	node* newNode = new node;
	newNode->data = data;
	newNode->next = NULL;
	if (front == NULL)//if queue is empty put newNode at front
		front = newNode;
	else
		back->next = newNode;
	back = newNode;
}//end add

int Queue::remove() {
	count--;
	if (front == NULL)// check if queue is empty
		return 0;
	node* temp = front;
	front = temp->next;
	if (count == 0)//if queue is now empty set back to NULL
		back == NULL;
	return temp->data;
}//end remove