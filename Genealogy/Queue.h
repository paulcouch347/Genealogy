#pragma once
#include<stdlib.h>

class Queue
{
public:
	Queue();
	int getCount();
	bool isEmpty();
	void add(int vertex);
	int remove();

private:
	struct node
	{
		int data;
		node* next;
	};
	node* front;
	node* back;
	int count;
};
