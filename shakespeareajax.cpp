#include <iostream>
// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include "fifo.h"

using namespace std;
using namespace cgicc; // Needed for AJAX functions.




// fifo for communication
string receive_fifo = "SSreply";
string send_fifo = "SSrequest";
string sentWord;

int main() {
  Cgicc cgi;    // Ajax object
  char *cstr;
  // Create AJAX objects to recieve information from web page.
  form_iterator word = cgi.getElement("word");
  sentWord = **word;

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);
  
  string reply;
  
sendfifo.openwrite();
		sendfifo.send(sentWord);
cout << "Content-Type: text/plain\n\n";
//while loop to end when ending message is sent
	while (reply!="$END") {
	/* Get a message from a server */
	recfifo.openread();
	reply = recfifo.recv();
	
	//if statement to break if ending message is sent
	if (reply=="$END") {
	sendfifo.fifoclose();
	break;
	}
		recfifo.fifoclose();


	cout << reply;
	}

  
return 0;
}
