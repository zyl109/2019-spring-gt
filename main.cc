#include <iostream>
#include <unistd.h>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"

using namespace std;

// create NetworkManager first
NetworkManager *nm = new NetworkManager();

int main(int argc, char** argv){
	
	//read input file
	if(argc!=2){
		cerr<<"worng format"<<endl;
		return 1;
	}
	else{
		//string inFile(argv[1]);
		nm->interpret(argv[1]);
		cout << "======================================================" << endl;
		cout << "input file: " << argv[1] << endl;
		//cout << "switch number: " << nm->get_sw_name() << endl;
		cout << "======================================================" << endl;
		nm->print_all_v();
		nm->print_all_e();
	}

	
	//calculate all degrees
	
	

	
	//find all nodes whose degree belong to odd
	//Vertex *odd_node;
	//odd_node = nm->get_all_nodes();

    
    return 0;
}