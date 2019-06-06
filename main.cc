#include <iostream>
#include <unistd.h>
#include <stack>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"

using namespace std;

// create NetworkManager first
NetworkManager *nm = new NetworkManager();

stack<string> find_euler_circuit(NetworkManager* graph){
	
	cout<<"cominggggggggggggggggggg!!!!!!!!!"<<endl;
	stack<string> tempPath, finalPath;
	
	tempPath.push(graph->vlist[0]->name);
	//cout<<"tempPath is "<<tempPath.empty()<<endl;
	while(!tempPath.empty()){
		cout<<"while!!!!!!!!!!!!!!!!!"<<endl;
		cout<<"switch number is "<<graph->switch_num<<endl;
		for(int i=0; i<graph->switch_num; i++){
			cout<<"i = "<<i<<endl;
			if(graph->connected_d(tempPath.top(), graph->vlist[i]->name) == 0){
				cout<<tempPath.top()<<" and "<<graph->vlist[i]->name<<" is connected!"<<endl;
				graph->disconnect(tempPath.top(), graph->vlist[i]->name);
				tempPath.push(graph->vlist[i]->name);
				cout<<"tempPath's top is: "<<tempPath.top()<<endl;
				break;
			}
			else if(graph->connected_d(graph->vlist[i]->name, tempPath.top()) == 0){
				cout<<tempPath.top()<<" and "<<graph->vlist[i]->name<<" is connected!"<<endl;
				graph->disconnect(graph->vlist[i]->name, tempPath.top());
				tempPath.push(graph->vlist[i]->name);
				cout<<"tempPath's top is: "<<tempPath.top()<<endl;
				break;
			}
			else if(i==graph->switch_num-1){
				finalPath.push(tempPath.top());
				cout<<"push push"<<endl;
				tempPath.pop();
				break;
			}
			
		}
		
	}
	
	return finalPath;
	
}

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
	//cout<<odd_nodes.size()<<endl;
	cout<<"odd nodes are:"<<endl;
	for(int i=0; i<odd_nodes.size(); i++){
		cout<<odd_nodes[i]->name<<endl;
	}
	//add edges
	
	/*
	while(!odd_nodes.empty()){
		nm->connect(odd_nodes[0]->name, odd_nodes[1]->name);
		odd_nodes.erase(odd_nodes.begin());
		cout<<"first of odd_nodes:"<<odd_nodes[0]->name<<endl;
		odd_nodes.erase(odd_nodes.begin());
		cout<<"first of odd_nodes:"<<odd_nodes[0]->name<<endl;
	}
	*/
	
	for(int i=0; i<odd_nodes.size()-1; i+=2){
		nm->connect(odd_nodes[i]->name, odd_nodes[i+1]->name);
	}
	nm->print_all_e();
	//cout<<"first of odd_nodes:"<<odd_nodes.front()->name<<endl;
		
	
	//find Euler circuit
	stack<string> finalPath;
	cout<<"NOT cominggggggggggggggggggg!!!!!!!!!"<<endl;
	finalPath = find_euler_circuit(nm);
	cout<<"final path is: ";
	while(!finalPath.empty()){
		cout<<finalPath.top()<<" ";
		finalPath.pop();
	}
	cout<<endl;
	
	

    
    return 0;
}


