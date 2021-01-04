#pragma once
#include <string>
#include "Queue.h"

using namespace std;

const int MAX_SIZE = 100;
const int MAX_CHILDREN = 10;

class Graph {
public:
	Graph();
	void insertPerson(int data[]);
	void insertFamily(int data[]);
	string verify();
	string relate(int start, int end);
private:
	struct person {
		int parents;
		int marriage;
	};
	struct family{
		int husband;
		int wife;
		int children[MAX_CHILDREN] = { 0 };
	};
	family families[MAX_SIZE];
	person people[MAX_SIZE];
};
