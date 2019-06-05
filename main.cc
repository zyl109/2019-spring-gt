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
		nm->interpret(argv[1]);
		//set switch_num
		for(int i=0; nm->vlist[i]!=NULL; i++){
			nm->switch_num++;
		}
		cout << "======================================================" << endl;
		cout << "input file: " << argv[1] << endl;
		cout << "switch number: " << nm->switch_num << endl;
		//cout << "switch number: " << nm->get_sw_name() << endl;
		cout << "======================================================" << endl;
		/*nm->add_switch("e");
		nm->add_switch("f");
		nm->add_switch("g");
		nm->add_switch("h");*/
		nm->print_all_v();
		nm->print_all_e();
	}
	
	
	
	//find out all odd nodes
	vector<Vertex*> odd_nodes;
	for(int i=0; i<nm->switch_num; i++){
		int degree=0;
		for(int j=0; j<nm->switch_num; j++){
			cout<<nm->vlist[i]->name;
			cout<<" and ";
			cout<<nm->vlist[j]->name;
			cout<<" are ";
			cout<<nm->connected(nm->vlist[i]->name, nm->vlist[j]->name)<<endl;
			if(nm->connected(nm->vlist[i]->name, nm->vlist[j]->name) == 0)
				degree++;
		}
		cout<<"degree = "<<degree<<endl;
		if(degree%2 != 0){
			//cout<<nm->vlist[i]->name<<" is odd node"<<endl;
			odd_nodes.push_back(nm->vlist[i]);
		}
		
	}
	cout<<odd_nodes.size()<<endl;
	cout<<"odd nodes are:"<<endl;
	for(int i=0; i<odd_nodes.size(); i++){
		cout<<odd_nodes[i]->name<<endl;
	}
	
	
	//calculate all degrees
	

    
    return 0;
}