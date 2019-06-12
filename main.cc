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
			/*	if there are egdes from tempPath's top to its neighborhoods
				mark this edge and push neighborhood to tempPath	*/
			if(graph->connected_d(tempPath.top(), graph->vlist[i]->name) == 0){
				graph->disconnect(tempPath.top(), graph->vlist[i]->name);
				tempPath.push(graph->vlist[i]->name);
				break;
			}
			/*	if there are egdes from tempPath's top to its neighborhoods
				mark this edge and push neighborhood to tempPath	*/
			else if(graph->connected_d(graph->vlist[i]->name, tempPath.top()) == 0){
				graph->disconnect(graph->vlist[i]->name, tempPath.top());
				tempPath.push(graph->vlist[i]->name);
				break;
			}
			/*	if there are NO egdes from tempPath's top to its neighborhoods
				pop tempPath's top and push it to finalPath		*/
			else if(i==graph->switch_num-1){
				finalPath.push(tempPath.top());
				tempPath.pop();
				break;
			}
		}
	}


	return finalPath;	
}


vector<int> all_pairs;

//find all permutation of odd nodes
void Perm(int start, int end, int a[]) {
    
    if(start == end){
		for (int i = 0; i < end; i++){
			//cout << a[i] << " ";
			all_pairs.push_back(i);
		}
        //cout << endl;
	    return;
    }
    for (int i = start; i < end; i++) {
        swap(a[start], a[i]);      
        Perm(start + 1, end, a);   
        swap(a[i], a[start]);      
    }
}

//find optimal pairwise matchings of odd nodes
vector<int> find_optimal_pairs(vector<vector<int >> cost_matrix, int size){

	vector<int> optimal_pairs;
	int list[size];
	int cost;
	int temp_cost, row, col;
	for(int i=0; i<size; i++){
		list[i]=i;	
	}

	Perm(0, size, list);
	
	//firs time initialization
	cost=0;
	for(int i=0; i<size; i+=2){
		row=all_pairs.at(0);
		col=all_pairs.at(1);
		cost=cost+cost_matrix.at(row).at(col);
		all_pairs.erase(all_pairs.begin());
		all_pairs.erase(all_pairs.begin());
		optimal_pairs.push_back(row);
		optimal_pairs.push_back(col);
	}
	//find min-cost matchings
	while(!all_pairs.empty()){
		for(int i=0; i<size; i+=2){
			row=all_pairs.at(0);
			col=all_pairs.at(1);
			temp_cost=temp_cost+cost_matrix.at(row).at(col);
			all_pairs.erase(all_pairs.begin());
			all_pairs.erase(all_pairs.begin());
			optimal_pairs.push_back(row);
			optimal_pairs.push_back(col);
		}
		
		if(cost>temp_cost){
			cost=temp_cost;
			for(int i=0; i<size; i++){
				optimal_pairs.erase(optimal_pairs.begin());
			}
		}
		else{
			for(int i=0; i<size; i++){
				optimal_pairs.pop_back();
			}
		}
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
			if(nm->connected(nm->vlist[i]->name, nm->vlist[j]->name) == 0){
				degree++;
			}
		}
		if(degree%2 != 0){
			odd_nodes.push_back(nm->vlist[i]);
		}
	}
	int odd_nodes_number = odd_nodes.size();
	
	
	
	//generate original graph
	Gplot *gp = new Gplot();
	gp->gp_add(nm->elist);
	gp->gp_dump(true);
	gp->gp_export("origin_graph");
	gp->~Gplot();


	
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
	vector<vector<int> >shortest_paths_length;
	for(int i=0; i<odd_nodes_number; i++){
		vector<int> tempVector;
		for(int j=0; j<odd_nodes_number; j++){
			if(i==j){
				tempVector.push_back(0);
			}
			else{
				avail_paths = path->find_paths(odd_nodes[i]->name, odd_nodes[j]->name);
				tempVector.push_back(avail_paths.back().size());
			}
		}
		shortest_paths_length.push_back(tempVector);

	}


	
	//find optimal pairwise matchings
	vector<int> final_pairs;
	final_pairs=find_optimal_pairs(shortest_paths_length, odd_nodes_number);
	
	
	
	//add edges to eulerize the graph
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
	
	
	
	//generate result graph
	Gplot *gp2 = new Gplot();
	gp2->gp_add(nm->elist);
	gp2->gp_dump(true);
	gp2->gp_export("result_path");
	gp2->~Gplot();
	
	
	
	//output final result
	cout << "======================================================" << endl;
	cout<<"final path is: ";
	while(!finalPath.empty()){
		cout<<finalPath.top()<<" ";
		finalPath.pop();
		if(!finalPath.empty()){
			cout<<"-> ";
		}
	}
	cout<<endl;
	cout << "======================================================" << endl;
	
	
	
	
	
	
	
	
	
    
    return 0;
}


