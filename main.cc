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


vector<int> find_optimal_pairs(vector<vector<int >> cost_matrix, int size){
	vector<int> optimal_pairs;
	for(int i=0; i<size; i++){
		
		optimal_pairs.push_back(i);
		
		
	}
	
	return optimal_pairs;
	
	
	
	
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
	int odd_nodes_number = odd_nodes.size();
	cout<<"odd nodes are:"<<endl;
	for(int i=0; i<odd_nodes_number; i++){
		cout<<odd_nodes[i]->name<<endl;
	}
	
	
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
	
	
	//construct a table of shortest paths length between odd nodes
	Path *path;
	path=new Path();
	path->append(tempGraph->elist);
	vector<vector<Edge *>> avail_paths;
	
	
	//int shortest_paths_length[odd_nodes_number][odd_nodes_number];
	vector<vector<int> >shortest_paths_length;
	for(int i=0; i<odd_nodes_number; i++){
		vector<int> tempVector;
		for(int j=0; j<odd_nodes_number; j++){
			if(i==j)
			tempVector.push_back(0);
			//shortest_paths_length[i][j]=0;
			//shortest_paths_length.at(i).at(j)=0;
			
			else{
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
				
				tempVector.push_back(avail_paths.back().size());
				//shortest_paths_length[i][j]=avail_paths.back().size();
				//shortest_paths_length.at(i).at(j)=avail_paths.back().size();
				
				/*
				for(int k=0; k<avail_paths.back().size(); k++){
					cout<<"head: "
						<<avail_paths.back().at(k)->head->name
						<<" ,tail: "
						<<avail_paths.back().at(k)->tail->name
						<<endl;
					shortest_paths_length[i][j]++;
				}
				*/
				
			}
		}
		shortest_paths_length.push_back(tempVector);

	}
	for(int i=0; i<odd_nodes_number; i++){
		for(int j=0; j<odd_nodes_number; j++){
			cout<<" i = "
				<<i
				<<" ,j = "
				<<j
				<<" ,path length = "
				<<shortest_paths_length[i][j]
				<<endl;
		}
	}


	

	//find optimal pairwise matchings
	vector<int> final_pairs;
	
	final_pairs=find_optimal_pairs(shortest_paths_length, odd_nodes_number);
	
	//add edges
	for(int i=0; i<odd_nodes_number/2; i++){
		string node1=odd_nodes[final_pairs.at(0)]->name;
		string node2=odd_nodes[final_pairs.at(1)]->name;
		avail_paths = path->find_paths(node1, node2);
		
		for(int k=0; k<avail_paths.back().size(); k++){
			nm->connect( avail_paths.back().at(k)->head->name
						,avail_paths.back().at(k)->tail->name
						);
			}
		
		final_pairs.erase(final_pairs.begin());
		final_pairs.erase(final_pairs.begin());
	}
		
		
	
	//find Euler circuit
	stack<string> finalPath;
	stack<string> finalPath2;
	finalPath = find_euler_circuit(nm);
	
	
	//generate final result graph
	while(!finalPath.empty()){
		string head=finalPath.top();
		finalPath2.push(head);
		finalPath.pop();
		if(!finalPath.empty()){
			nm->connect(head, finalPath.top());
		}
	}
	
	while(!finalPath2.empty()){
		finalPath.push(finalPath2.top());
		finalPath2.pop();
	}
	
	Gplot *gp = new Gplot();
	gp->gp_add(nm->elist);
	gp->gp_dump(true);
	gp->gp_export("plot");
	
	
	//output final result
	cout<<"final path is: ";
	while(!finalPath.empty()){
		cout<<finalPath.top()<<" ";
		finalPath.pop();
		if(!finalPath.empty()){
			cout<<"-> ";
		}
	}
	cout<<endl;
	
	
	
	


    
    return 0;
}


