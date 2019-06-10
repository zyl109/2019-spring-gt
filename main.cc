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

	stack<string> tempPath, finalPath;
	
	//push first node to tempPath
	tempPath.push(graph->vlist[0]->name);
	
	while(!tempPath.empty()){
		
		for(int i=0; i<graph->switch_num; i++){
			
			//if there are egdes from tempPath's top to its neighborhoods
			//mark this edge and push neighborhood to tempPath
			if(graph->connected_d(tempPath.top(), graph->vlist[i]->name) == 0){
				//cout<<tempPath.top()<<" and "<<graph->vlist[i]->name<<" is connected!"<<endl;
				graph->disconnect(tempPath.top(), graph->vlist[i]->name);
				tempPath.push(graph->vlist[i]->name);
				//cout<<"tempPath's top is: "<<tempPath.top()<<endl;
				//graph->print_all_e();
				break;
			}
			//if there are egdes from tempPath's top to its neighborhoods
			//mark this edge and push neighborhood to tempPath
			else if(graph->connected_d(graph->vlist[i]->name, tempPath.top()) == 0){
				//cout<<graph->vlist[i]->name<<" and "<<tempPath.top()<<" is connected!"<<endl;
				graph->disconnect(graph->vlist[i]->name, tempPath.top());
				tempPath.push(graph->vlist[i]->name);
				//cout<<"tempPath's top is: "<<tempPath.top()<<endl;
				//graph->print_all_e();
				break;
			}
			//if there are NO egdes from tempPath's top to its neighborhoods
			//pop tempPath's top and push it to finalPath
			else if(i==graph->switch_num-1){
				//cout<<"push push: "<<tempPath.top()<<endl;
				finalPath.push(tempPath.top());
				tempPath.pop();
				//if(!tempPath.empty())
					//cout<<"tempPath's top is: "<<tempPath.top()<<endl;
				
				break;
			}
			
		}
		
	}

	//graph->print_all_v();
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
		nm->print_all_v();
		nm->print_all_e();
	}
	
	
	
	/*
	//path exercise
	Path *path;
	path=new Path();
	path->append(nm->elist);
	std::vector<std::vector<Edge *>> avail_paths = path->find_paths(std::string("a"), std::string("d"));
	path->debug();
	*/
	
	//find out all odd nodes
	vector<Vertex*> odd_nodes;
	for(int i=0; i<nm->switch_num; i++){
		int degree=0;
		for(int j=0; j<nm->switch_num; j++){
			
			cout<<nm->vlist[i]->name
				<<" and "
				<<nm->vlist[j]->name
				<<" are "
				<<nm->connected(nm->vlist[i]->name, nm->vlist[j]->name)<<endl;
				
			if(nm->connected(nm->vlist[i]->name, nm->vlist[j]->name) == 0)
				degree++;
		}
		cout<<"degree = "<<degree<<endl;
		if(degree%2 != 0){
			odd_nodes.push_back(nm->vlist[i]);
		}
		
	}

	//print odd nodes
	cout<<"odd nodes are:"<<endl;
	for(int i=0; i<odd_nodes.size(); i++){
		cout<<odd_nodes[i]->name<<endl;
	}
	
	/*
	//BFS
	for(int i=0; i<odd_nodes.size(); i++){
		distance[i] = get_distance(odd_nodes[i]->name, odd_nodes[j]->name, nm);
		}
	}
	*/
	
	//construct a bi-direcational graph
	NetworkManager* tempGraph = new NetworkManager();
	tempGraph->vlist = nm->vlist;
	for(int i=0; i<nm->switch_num; i++){
		for(int j=i+1; j<nm->switch_num; j++){
			if(nm->connected(nm->vlist[i]->name, nm->vlist[j]->name) == 0){
				tempGraph->connect_r(nm->vlist[i]->name, nm->vlist[j]->name);
			}
		}	
	}
	
	Path *path;
	path=new Path();
	path->append(tempGraph->elist);
	vector<vector<Edge *>> avail_paths;
	vector<vector<Edge *>> shortest_paths;
	
	
	
	int shortest_paths_length[odd_nodes.size()][odd_nodes.size()];
	
	for(int i=0; i<odd_nodes.size(); i++){
		for(int j=0; j<odd_nodes.size(); j++){
			shortest_paths_length[i][j]=0;
			
			if(i!=j){
				avail_paths = path->find_paths(odd_nodes[i]->name, odd_nodes[j]->name);
				
				cout<<"From "
					<<odd_nodes[i]->name
					<<" to "
					<<odd_nodes[j]->name
					<<" i = "
					<<i
					<<" j = "
					<<j
					<<endl;
				//path->debug();
	
				for(int k=0; k<avail_paths.back().size(); k++){
					cout<<"head: "
						<<avail_paths.back().at(k)->head->name
						<<" ,tail: "
						<<avail_paths.back().at(k)->tail->name
						<<endl;
					shortest_paths_length[i][j]++;
				}
			}
		}

	}
	
	for(int i=0; i<odd_nodes.size(); i++){
		for(int j=0; j<odd_nodes.size(); j++){
			cout<<" i = "
				<<i
				<<" ,j = "
				<<j
				<<" ,path length = "
				<<shortest_paths_length[i][j]
				<<endl;
		}
	}


	


	

/*	
	//add edges
	for(int i=0; i<odd_nodes.size()-1; i+=2){
		nm->connect(odd_nodes[i]->name, odd_nodes[i+1]->name);
	}
	nm->print_all_e();
	

	

	//find Euler circuit
	stack<string> finalPath;
	finalPath = find_euler_circuit(nm);
	
	cout<<"final path is: ";
	while(!finalPath.empty()){
		cout<<finalPath.top()<<" ";
		finalPath.pop();
	}
	cout<<endl;
	
	
	
	
	
*/
    
    return 0;
}


