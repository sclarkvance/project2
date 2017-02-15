#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "fifo.h"

using namespace std;

string receive_fifo = "SSreply";
string send_fifo = "SSrequest";

int main() {
  string message, word;


//while loop to continuously run program
  while (1) {
  string reply;
	cout << "Enter a word: ";
	cin >>  word;
	//transform(name.begin(), name.end(), name.begin(), ::toupper);

		// create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);


	cout << "Send:" << word << endl;
		sendfifo.openwrite();
		sendfifo.send(word);

	while (reply!="$END") {
	/* Get a message from a server */
	recfifo.openread();
	reply = recfifo.recv();
	
	//
	if (reply=="$END") {
	sendfifo.fifoclose();
	break;
	}
	cout << reply << endl;
	recfifo.fifoclose();
	}

  }


}
