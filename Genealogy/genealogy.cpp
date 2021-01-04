/*
Author: Paul Couch
Class: CS 315
Date: 4/26/2020
*/


#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

const int MAX_SIZE = 100;
const int MAX_CHILDREN = 10;

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
	struct family {
		int husband;
		int wife;
		int children[MAX_CHILDREN] = { 0 };
	};
	family families[MAX_SIZE];
	person people[MAX_SIZE];
};

Graph::Graph() {
	family newFamily;
	person newPerson;
	for (int i = 0; i < MAX_SIZE; i++) {
		families[i] = newFamily;
		people[i] = newPerson;
	}
}//end default constructor

void Graph::insertPerson(int data[]) {
	person newPerson;
	newPerson.parents = data[1];
	newPerson.marriage = data[2];
	people[data[0]] = newPerson;
}//end insertPerson

void Graph::insertFamily(int data[]) {
	family newFamily;
	newFamily.husband = data[1];
	newFamily.wife = data[2];
	for (int i = 0; i < MAX_CHILDREN; i++) {
		newFamily.children[i] = data[i + 3];
	}
	families[data[0]] = newFamily;
}//end insertFamily

string Graph::verify() {
	string output = "";


	for (int i = 1; i < MAX_SIZE; i++)
	{
		int temp = people[i].parents;
		if (temp != 0)
		{
			bool hasChild = false;
			for (int j = 0; j < MAX_CHILDREN; j++) {
				if (families[temp].children[j] == i)
					hasChild = true;
			}
			if (!hasChild)
				output += "Person " + to_string(i) + " points to parent family " + to_string(temp) + " but there is no back-pointer.\n";
		}//Check if the child's parent family points back

		temp = people[i].marriage;
		if (temp != 0) {
			if ((families[temp].husband != i) && (families[temp].wife != i))
				output += "Person " + to_string(i) + " points to marriage family " + to_string(temp) + " but there is no back-pointer.\n";
		}//Check if the parent's child family points back

		temp = families[i].husband;
		if (temp != 0) {
			if (people[temp].marriage != i)
				output += "Family " + to_string(i) + " points to husband person " + to_string(temp) + " but there is no back-pointer.\n";
		}//Check if individual points back to husband family

		temp = families[i].wife;
		if (temp != 0) {
			if (people[temp].marriage != i)
				output += "Family " + to_string(i) + " points to wife person " + to_string(temp) + " but there is no backpointer.\n";
		}//Check if individual points back to wife family

		int j = 0;
		while (families[i].children[j] != 0 && j <= MAX_CHILDREN) {
			temp = families[i].children[j];
			if (people[temp].parents != i)
				output += "Family " + to_string(i) + " points to child person " + to_string(temp) + " but there is no back-pointer.\n";
			j++;
		}//Check if all children point back
	}
	if (output == "") {
		output = "The data are consistent\n";
	}
	return output;
}//end verify

string Graph::relate(int start, int end) {
	int familyToPerson[MAX_SIZE];//arrays to keep backtrace of elements
	int personToFamily[MAX_SIZE];
	bool finished = false;
	string output;
	bool visitedFamily[MAX_SIZE + 1] = { false };
	bool visitedPerson[MAX_SIZE + 1] = { false };
	Queue familyQueue = Queue();
	Queue personQueue = Queue();
	int current;
	visitedPerson[start] = true;
	personQueue.add(start);
	if ((people[start].marriage != 0) && (visitedFamily[people[start].marriage] != true)) {
		familyQueue.add(people[start].marriage);
		visitedFamily[people[start].marriage] = true;
		familyToPerson[people[start].marriage] = start;
	}//add start's marriage family to queue
	if ((people[start].parents != 0) && (visitedFamily[people[start].parents] != true)) {
		familyQueue.add(people[start].parents);
		visitedFamily[people[start].parents] = true;
		familyToPerson[people[start].parents] = start;
	}//add start's parent family to queue

	while ((!personQueue.isEmpty() || !familyQueue.isEmpty()) && (finished == false))
	{
		if (!familyQueue.isEmpty()) {
			current = familyQueue.remove();
			if ((families[current].husband != 0) && (visitedPerson[families[current].husband] != true)) {
				personQueue.add(families[current].husband);
				visitedPerson[families[current].husband] = true;
				personToFamily[families[current].husband] = current;
			}//add husband to queue if he hasn't been visited
			if ((families[current].wife != 0) && (visitedPerson[families[current].wife] != true)) {
				personQueue.add(families[current].wife);
				visitedPerson[families[current].wife] = true;
				personToFamily[families[current].wife] = current;
			}// add wife to queue if she hasn't been visited
			int i = 0;
			while ((families[current].children[i] != 0) && (i < MAX_CHILDREN)) {
				if ((families[current].children[i] != 0) && (visitedPerson[families[current].children[i]] != true)) {
					personQueue.add(families[current].children[i]);
					visitedPerson[families[current].children[i]] = true;
					personToFamily[families[current].children[i]] = current;
				}//go through children and add to queue if they haven't been visited
				i++;
			}
		}
		if (!personQueue.isEmpty()) {
			current = personQueue.remove();
			if (current == end) {
				finished = true;
			}//check if end has been reached
			else {
				if ((people[current].marriage != 0) && (visitedFamily[people[current].marriage] != true)) {
					familyQueue.add(people[current].marriage);
					visitedFamily[people[current].marriage] = true;
					familyToPerson[people[current].marriage] = current;
				}//add current's marriage family to queue
				if ((people[current].parents != 0) && (visitedFamily[people[current].parents] != true)) {
					familyQueue.add(people[current].parents);
					visitedFamily[people[current].parents] = true;
					familyToPerson[people[current].parents] = current;
				}//add current's parent family to queue
			}
		}

	}

	//trace and output shortest path between individuals
	if (current == end) {
		int path[20] = { 0 };
		int i = 0;
		path[0] = current;//first element in path
		while (current != start) {//go backwards through path and fill the array
			i++;
			path[i] = personToFamily[current];
			i++;
			path[i] = familyToPerson[personToFamily[current]];
			current = familyToPerson[personToFamily[current]];
		}
		while (i != 0) {//go backwards through array to output in correct order
			if (i % 2 == 0) {
				output += "person " + to_string(path[i]) + "->";
			}//individuals are always on even index
			else {
				output += "family " + to_string(path[i]) + "->";
			}
			i--;
		}
		output += "person " + to_string(end);
		return output;
	}
	else {
		return "Individuals " + to_string(start) + " and " + to_string(end) + " are not connected.\n";
	}
}//end relate

int main() {
	string line, word; // string to take in commands
	Graph familyTree = Graph(); // main bipartite graph
	int data[12]; //array to hold values from command
	while (getline(cin, line)) {
		for (int i = 0; i < 12; i++) {
			data[i] = 0;
		}//clear data array
		stringstream input(line);
		input >> word;
		if (word == "Person"){
			//slpits line and adds data to array
			input >> data[0];
			input >> word;
			input >> data[1];
			input >> word;
			input >> data[2];
			familyTree.insertPerson(data);
			cout << "Person " << data[0] << " has parents " << data[1] << " and is married in family " << data[2] << "." << endl;
		}
		else if (word == "Family") {
			input >> data[0];
			input >> word;
			input >> data[1];
			input >> word;
			input >> data[2];
			int i = 2;
			while (input >> word){
				i++;
				input >> data[i];
			}
			familyTree.insertFamily(data);
			//Output
			cout << "Family " << data[0] << " has husband " << data[1] << ", wife " << data[2];
			i = 3;
			while (data[i] != 0 && i < 12){
				if (i == 3) {
					cout << ", and children";
				}
				cout << " " << data[i];
				i++;
			}
			if (i == 3) {
				cout << ", and no children." << endl;
			}
			else {
				cout << "." << endl;
			}

		}
		else if (word == "Verify") {
			cout << familyTree.verify();
		}
		else if (word == "Relate") {
			input >> data[0];
			input >> data[1];
			cout << familyTree.relate(data[0], data[1]) << endl; //outputs relate command
		}
	}
	return 0;
}//end main