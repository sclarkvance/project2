#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "fifo.h"
#include <string>
#include <vector>

using namespace std;

void createmap();
//precondition: takes no arguments
//postcondition: creates a map of all their words in Shakespeare.txt and their positions
vector <int> indexSearch(string);
//precondition: takes a string
//postcondition: returns a vector of positions for that word
void displayLine(int,Fifo);
//precondition: takes an int and a fifo
//postcondition: displays a line and sends it through the fifo
void findPos(vector <int>, Fifo);
//precondition: takes a vector of ints and a fifo
//postcondition: sends each line to display line
void sendMessage(string,Fifo);
//precondition: takes a string and a fifo
//postcondition: sends a message to the fifo


/* Fifo names */
string receive_fifo = "SSrequest";
string send_fifo = "SSreply";

map<string, vector<int> > refs;
    // iterator for find 
    map<string, vector<int> >::iterator it;
  

int main() {
  //  cout << "Server: " <<PATH <<  endl;
    // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);

    createmap();
    char ans;
    string word, outMessage;
    


while(1) {

  /* Get a message from a client */
    recfifo.openread();
    word = recfifo.recv();
    cout << word << endl;
    
    findPos(indexSearch(word),sendfifo);
    }
sendfifo.fifoclose();
	recfifo.fifoclose();
}

void createmap() {
    string line, word;
    ifstream infile("Shakespeare.txt"); // open the file
    //seekg function used to move past the metadata
    infile.seekg(7428, infile.beg);
    int position = 0;
    //while loop to get lines of code
    while (!infile.fail()) {
        position = infile.tellg();
        getline(infile, line); // get the next line of code
        istringstream lineStream(line); // Create a string stream of the line
        //while loop to move to the next word
        while (lineStream >> word) { // get the next word
            refs[word].push_back(position); // push the word and the line position on the vector for this word
        }
        position = infile.tellg(); // get the position of the next line
    }
}

vector <int> indexSearch(string word) {                                                             
    vector<int> blank; // return for no matches                                                                                  
    /* find the word and get the vector of references */
    /* First use find, so as to NOT create a new entry */
    it = refs.find(word);
    //return blank vector if there aren't any matches for the word
    if (it == refs.end()) {
        return (blank);
    } else {
        return (refs[word]);
    }
}

void findPos(vector <int> positions, Fifo ofifo) {
ofifo.openwrite();
    vector<int>::iterator refsIterator;
    //for loop to cycle through the positions of a word
    for (refsIterator = positions.begin(); refsIterator != positions.end(); refsIterator++) {
      displayLine(*refsIterator, ofifo);
    }
    ofifo.send("$END");
}

void displayLine(int linePos, Fifo ofifo) {
    string tempLine;
    ifstream infile("Shakespeare.txt");
    infile.seekg(linePos, infile.beg);
    getline(infile,tempLine);
    cout << tempLine << endl;
    sendMessage(tempLine, ofifo);
} 

void sendMessage(string outMessage, Fifo ofifo) {
cout << "sendMessage running";
	//ofifo.openwrite();
	string newMessage = ("<p>" + outMessage + "</p>");
	ofifo.send(newMessage);
	
}
