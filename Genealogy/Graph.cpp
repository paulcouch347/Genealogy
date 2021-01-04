#include "Graph.h"

Graph::Graph() {
	
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
		output = "The data are consistent";
	}
	return output;
}//end verify

string Graph::relate(int start, int end) {
	int familyToPerson[MAX_SIZE];//arrays to keep backtrace of elements
	int personToFamily[MAX_SIZE];
	bool finished = false;
	string output;
	bool visitedFamily[maxSize] = { false };
	bool visitedPerson[maxSize] = { false };
	Queue familyQueue = Queue();
	Queue personQueue = Queue();
	int current;
	//MODIFIED BFS ALGORITHM
	visitedPerson[start] = true;
	personQueue.add(start);
	if ((people[start].marriage != 0) && (visitedFamily[people[start].marriage] != true)){
		familyQueue.add(people[start].marriage);
		visitedFamily[people[start].marriage] = true;
		familyToPerson[people[start].marriage] = start;
	}//add start's marriage family to queue
	if ((people[start].parents != 0) && (visitedFamily[people[start].parents()] != true)){
		familyQueue.add(people[start].parents);
		visitedFamily[people[start].parents] = true;
		familyToPerson[people[start].parents()] = start;
	}//add start's parent family to queue

	while ((!personQueue.isEmpty() || !familyQueue.isEmpty()) && (finished == false))
	{
		if (!familyQueue.isEmpty()) {
			current = familyQueue.remove();
			if ((families[current].husband != 0) && (visitedPerson[families[current].husband] != true)){
				personQueue.add(families[current].husband);
				visitedPerson[families[current].husband] = true;
				personToFamily[families[current].husband] = current;
			}//add husband to queue if he hasn't been visited
			if ((families[current].wife != 0) && (visitedPerson[families[current].wife] != true)){
				personQueue.add(families[current].wife);
				visitedPerson[families[current].wife()] = true;
				personToFamily[families[current].wife] = current;
			}// add wife to queue if she hasn't been visited
			int i = 0;
			while ((families[current].children[i] != 0) && (i < MAX_CHILDREN)){
				if ((families[current].children[i] != 0) && (visitedPerson[families[current].children[i]] != true)){
					personQueue.add(families[current].children[i]);
					visitedPerson[families[current].children[i]] = true;
					personToFamily[families[current].children[i]] = current;
				}//go through children and add to queue if they haven't been visited
				i++;
			}
		}
		if (!personQueue.isEmpty()) {
			current = personQueue.remove();
			if (current == end){
				finished = true;
			}//check if end has been reached
			else {
				if ((people[current].marriage != 0) && (visitedFamily[people[current].marriage] != true)){
					familyQueue.add(people[current].marriage);
					visitedFamily[people[current].marriage] = true;
					familyToPerson[people[current].marriage] = current;
				}//add current's marriage family to queue
				if ((people[current].parents != 0) && (visitedFamily[people[current].parents] != true)){
					familyQueue.add(people[current].parents);
					visitedFamily[people[current].parents] = true;
					familyToPerson[people[current].parents] = current;
				}//add current's parent family to queue
			}
		}

	}

	//Algorithm to trace and output shortest path between individuals
	if (current == end) {
		int path[20] = { 0 };
		int i = 0;
		path[0] = current;//first element in path
		while (current != start){//fill array with backtrace of elements
			i++;
			path[i] = personToFamily[current];
			i++;
			path[i] = familyToPerson[personToFamily[current]];
			current = familyToPerson[personToFamily[current]];
		}
		while (i != 0) {//go backwards through array to output in correct order
			if (i % 2 == 0) {
				output += "person " + to_string(path[i]) + "->";
			}
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
