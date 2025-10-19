#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <list>
#include <utility>
#include <iomanip>


//Mitchell McNaney
//CS 300 Project 2
//10-11-2025
//C:\Dev\CS 300 Project 2\\CS 300 ABCU_Advising_Program_Input.csv

std::string fileName;

//Course data type to hold all course information
class Course {
private:
	std::string courseID;
	std::string courseName;
	std::vector<std::string> preRequisites;

public:
	void setID(std::string ID) {
		courseID = ID;
	}
	void setCourseName(std::string name) {
		courseName = name;
	}
	void addPreReq(std::string preReq) {
		preRequisites.push_back(preReq);
	}

	std::string getID() {
		return courseID;
	}
	std::string getCourseName() {
		return courseName;
	}
	std::vector<std::string> getPreReq() {
		return preRequisites;
	}
	
	//Helper print function to print prerequisites
	void printReqs() {
		std::string label;
		for (int i = 0; i < preRequisites.size(); i++) {
			if (i < preRequisites.size() - 1) {
				label = preRequisites[i] + ",";
				std::cout 
					<< std::left
					<< std::setw(9)
					<< label;
			}
			else {
				std::cout 
					<< std::left
					<< std::setw(8)
					<< preRequisites[i];
			}
		}
	}



	Course() {
		courseID = "";
		courseName = "";

	}
	Course(std::string courseID, std::string courseName, std::vector<std::string> preRequisties) {
		this->courseID = courseID;
		this->courseName = courseName;
		this->preRequisites = preRequisites;
	}

};

//Helper Print function
void printCourses(std::vector<Course>& courses) {
	for (int i = 0; i < courses.size(); i++) {
		std::cout << "Course ID: " << courses[i].getID() << " Course Name: " << courses[i].getCourseName() << " Prerequisites: ";
		courses[i].printReqs();
		std::cout << std::endl;
	}
}

//Node Struct to hold course info and pointers to prev and next nodes
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() : left(nullptr), right(nullptr) {}
};

//Node Struct for the hash table. (Two structs to keep logic separate and avoid memory issues when deleting nodes).
struct HashNode {
	Course course;
	HashNode* next = nullptr;

	HashNode(Course c) : course(c), next(nullptr) {}
};

//Function to create new Nodes.
Node* createNode(Course course) {
	Node* newNode = new Node();
	newNode->course = course;
	newNode->left = nullptr;
	newNode->right = nullptr;
	return newNode;
}

//Function to create new HashNodes.
HashNode* createHashNode(Course course) {
	HashNode* newNode = new HashNode(course);

	return newNode;
}

//Binary Search Tree for Alphanumeric sorting
class BST {


private:
	Node* root;


	//Recursive helper
	Node* insertNode(Node* current, Course course) {

		//If tree is empty, we need to return a new node. 
		if (current == nullptr) {
			return createNode(course);
		}
		//If we are trying to insert a duplicate course
		if (course.getID() == current->course.getID()) {
			return current;
		}
		//If courseID is less than root's courseID. Recurse down left and set root->left node to it. 
		if (course.getID() < current->course.getID()) {
			current->left = insertNode(current->left, course);

		}
		//If courseId is greater than root's courseID. Recurse down right and set root->right node to it. 
		else if (course.getID() > current->course.getID()) {
			current->right = insertNode(current->right, course);
		}

		return current;


	}//Insert Node

	//inOrderTraversal internal logic
	void inOrderTraversal(Node* current) {

		if (current != nullptr) {
			inOrderTraversal(current->left);
			std::cout
				<< std::left
				<< std::setw(12)
				<< "Course ID: "
				<< std::setw(8)
				<< current->course.getID() << "||"
				<< " Course Name: "
				<< std::setfill('.') << std::setw(40)
				<< current->course.getCourseName() << "||" << std::setfill(' ')
				<< " Prerequisites: ";
			current->course.printReqs();
			std::cout << std::endl;
			inOrderTraversal(current->right);

		}

	}//inorder

public:
	BST() : root(nullptr) {}

	//Public insert function
	void insert(const Course& course) {
		root = insertNode(root, course);
	}

	void printInOrder() {
		if (root == nullptr) {
			std::cout << "Course list is empty." << std::endl;
			return;
		}
		inOrderTraversal(root);
	}

	//Public getter if access to root is needed. 
	Node* getRoot() {
		return root;
	}


};//BST Class

//Function to open file
std::ifstream openFile(){

	//Prompt user for FileName and open file.
	std::cout << "Please enter the exact name of your file\n";
	std::getline(std::cin, fileName);
	//Keep trying to open the file until successful open. 
	std::ifstream coursesFile(fileName);
	while (!coursesFile.is_open()) {
		std::cout << "Failed to open file: \"" << fileName << "\". \nPlease re - enter your file name(File name must match exactly, case-sensitive).\n" << std::endl;
		std::getline(std::cin, fileName);
		std::cin.clear();
		coursesFile.clear();
		coursesFile.open(fileName);
	}
	//Successful file open. Return file object. 
	std::cout << "File: \"" << fileName << "\" opened successfully.\n" << std::endl;
	//Close the file for now.
	return std::move(coursesFile);		//move semantic saves resources by emptying original file object and transferring ownership to main/whatever else calls this. 
}

//Function to read and store data from from
std::vector<Course> parseFile(std::ifstream& file) {
	std::string currentLine;
	std::string token;
	int i = 0;
	int j = 0;
	std::vector<Course> courseList;
	//Read each line in the file
	while (getline(file, currentLine)) {
		courseList.emplace_back();
		std::stringstream curLine(currentLine);	//Store in a string stream
		i = 0;
		while (getline(curLine, token, ',')) {
			
			switch (i) {

			case 0:	//CourseID
				courseList[j].setID(token);
				break;

			case 1: //CourseName
				courseList[j].setCourseName(token);
				break;

			default: //PreRequisites
				courseList[j].addPreReq(token);
				break;

			}
			i++;
		}
		j++;
	}


	return courseList;
}




//Hash table for efficient lookupt and insertion
class HashTable {

	HashNode* current = nullptr;
	static const unsigned int DEFAULT_SIZE = 20;
	unsigned int tableSize = DEFAULT_SIZE;
	std::vector<HashNode*> buckets;

private:
	static const int hashGroups = 20;

	//Key Hashing function, built for accepting strings. 
	int hashString(std::string courseID) {
		
		//Hash string into a number
		std::hash<std::string> stringHasher;
		size_t stringHash = stringHasher(courseID);
		
		//Convert to key within acceptable range of buckets. 
		return static_cast<int>(stringHash % tableSize);

	}

public:

	//For inserting a new node into the hash table. 
	void Insert(Course course) {

		//Get hashTable index using predefined string hash function
		int index = hashString(course.getID());

		//Get the memory location of the chain which exists at a particular key bucket. 
		HashNode* chain = buckets[index];

		//For an empty bucket
		while (chain != nullptr) {
			if (chain->course.getID() == course.getID()) {
				return;
			}
			chain = chain->next;
		}
		//Create new Node 
		HashNode* newNode = new HashNode(course);

		newNode->next = buckets[index];
		buckets[index] = newNode;

	}

	void printHashTable() {
		HashNode* current = nullptr;

		for (int i = 0; i < buckets.size(); i++) {
			current = buckets[i];
			while (current != nullptr) {
				std::cout << "key: " << i << " " << current->course.getID() << std::endl;
				current = current->next;
			}
		}

	}
	//Search hash table
	void searchTable(std::string courseCode) {

		//find key using hash function
		size_t key = hashString(courseCode);

		//Go to memory address at that index. 
		auto& chain = buckets[key];

		//If empty...
		if (chain == nullptr) {
			std::cout << courseCode << " is not a valid course...\nPlease try another course code:\n" << std::endl; 
			return;
		}
		
		//Otherwise check to see if that courseID exists in that bucket. 
		while (chain->course.getID() != courseCode) {
			chain = chain->next;
			//Go through chain, we will loop until we either find the courseCode or hit the end of the list. If end of list is hit, return with nothing found. 
			if (chain == nullptr) {
				std::cout << courseCode << " is not a valid course...\nPlease try another course code:\n" << std::endl;
				return;
			}
		}
		//Found course. 
		std::cout 
			<< "Found course...\n"
			<< std::left 
			<< "Course ID: "
			<< std::setw(8) 
			<< chain->course.getID() << "||"
			<< " Course Name: " 
			<< std::setfill('.') << std::setw(40)
			<< chain->course.getCourseName() << "||" << std::setfill(' ')
			<< " Prerequisites: ";
		chain->course.printReqs();
		std::cout << std::endl;
		return;
	}

	//Constructor which either takes no argument or a size argument for number of buckets.
	HashTable(unsigned int capacity = DEFAULT_SIZE) {
		tableSize = capacity;
		//Set buckets size equal to specified tableSize, set head of each bucket to nullptr
		buckets.resize(tableSize, nullptr);
	}


};


int main() {
	
	int choice = 0; 
	HashTable hTable;
	BST binarySearchTree;
	std::ifstream coursesFile;
	std::vector<Course> courseList;
	std::string courseCode;

	//Menu
	while (choice != 9) {


		std::cout << "Please select an option:\n" << "1. Load courses\n2. Display full sorted course list.\n3. Search for a course.\n9. Exit program.\n";
		std::cin >> choice;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			choice = 4;
		}

		
		switch (choice){

		case 1:	//Load File
			//Open the File
			std::cin.ignore();
			coursesFile = openFile();

			//Parse the file 
			courseList = parseFile(coursesFile);

			//Populate BST and Hash Table
			for (int i = 0; i < courseList.size(); i++) {

				binarySearchTree.insert(courseList[i]);
				hTable.Insert(courseList[i]);

			}
			break;

		case 2: //Print alphanumeric
			binarySearchTree.printInOrder();
			break;

		case 3: //Search Course Code
			std::cout << "Please enter a course code to search (case-sensitive)\n";
			std::cin >> courseCode;
			hTable.searchTable(courseCode);
			break;

		case 9: //Exit
			std::cout << "Goodbye!\n";
			break;

		default: //Invalid menu choice
			std::cout << "Please enter a selection 1, 2, 3, or 9\n";
		}

		

	}
	return 0;
}