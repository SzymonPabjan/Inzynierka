#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <time.h> 
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Node
{
public:
	unsigned int index;
	std::vector<unsigned int> neighbor_list;
	Node(unsigned int index_from_out)
	{
		index = index_from_out;
	}

};

class Graph
{
public:
	unsigned int number_of_nodes;
	std::vector<Node> node_array;
	float prob_of_connection;
	float theta = log(prob_of_connection / (1 - prob_of_connection));

	Graph(unsigned int number_of_nodes_in, float p)
	{
		number_of_nodes = number_of_nodes_in;
		prob_of_connection = p;
		theta = log(prob_of_connection / (1 - prob_of_connection));
		for (int i = 0; i < number_of_nodes; i++)
		{
			node_array.push_back(Node(i));
		}
	}

	void creat_connection(unsigned int node_i, unsigned int node_j)//creating edge between node number i and node number j
	{
		bool node_i_and_node_j_connected = false;

		for (int j = 0; j < node_array[node_i].neighbor_list.size(); j++)
		{
			if (node_array[node_i].neighbor_list[j] == node_j)
			{
				node_i_and_node_j_connected = true;
			}
		}

		if (node_i != node_j && node_i_and_node_j_connected == false)//if true then edge is created
		{
			node_array[node_i].neighbor_list.push_back(node_j);
			node_array[node_j].neighbor_list.push_back(node_i);
		}
		else
		{
			cout << "Edge already exists! Try differnet pair" << endl;
		}
	}

	std::vector<Node> creat_graph_given_number_of_edges(unsigned int number_of_edges)//creating graph with given number of edges between randomly selected nodes
	{

		for (int i = 0; i < number_of_edges; i++)
		{
			srand(time(NULL));
			while (true)//will continue untill last if is satisfied
			{
				unsigned int node_i = rand() % node_array.size() + 1;
				unsigned int node_j = rand() % node_array.size() + 1;
				bool node_i_and_node_j_connected = false;

				for (int j = 0; j < node_array[node_i].neighbor_list.size(); j++)// checks if given nodes are already connected
				{
					if (node_array[node_i].neighbor_list[j] == node_j)
					{
						node_i_and_node_j_connected = true;
					}
				}

				if (node_i != node_j && node_i_and_node_j_connected == false)//if true then edge is created
				{
					node_array[node_i].neighbor_list.push_back(node_j);
					node_array[node_j].neighbor_list.push_back(node_i);
					break;
				}

			}

		}
		return node_array;
	}

	std::vector<Node> creat_graph_on_probabilty()//creates edges based on probabilty
	{
		//int count = 0;
		//cout<<count<<endl;
		srand((unsigned int)time(NULL));
		for (int i = 0; i < node_array.size(); i++)
		{
			for (int j = 0; j < node_array.size(); j++)
			{
				float random = float(rand()) / float((RAND_MAX));

				bool node_i_and_node_j_connected = false;

				for (int k = 0; k < node_array[i].neighbor_list.size(); k++)// checks if given nodes are already connected
				{
					if (node_array[i].neighbor_list[k] == j)
					{
						node_i_and_node_j_connected = true;
					}
				}

				if (random < prob_of_connection && i != j && node_i_and_node_j_connected == false)
				{
					node_array[i].neighbor_list.push_back(j);
					node_array[j].neighbor_list.push_back(i);
					//count ++;
					//cout<<count<<endl;
				}
			}
		}
		return node_array;
	}

	void show_graph_neighbors()
	{
		for (int i = 0; i < node_array.size(); i++)
		{
			cout << i << "    ";
			for (int j = 0; j < node_array[i].neighbor_list.size(); j++)
			{
				cout << node_array[i].neighbor_list[j] << " ";
			}
			if (node_array[i].neighbor_list.size() == 0)
			{
				cout << "No connections" << endl;
			}
			else
			{
				cout << endl;
			}

		}
	}

	void show_degree()
	{
		for (int i = 0; i < node_array.size(); i++)
		{
			cout << i << "    " << node_array[i].neighbor_list.size() << endl;;
		}
	}

	void show_average_max_median_degree()
	{
		float max_degree = 0;
		unsigned int median = 0;
		float average = 0;
		std::vector<unsigned int> degree_array;
		for (int i = 0; i < node_array.size(); i++)
		{
			degree_array.push_back(node_array[i].neighbor_list.size());
			if (node_array[i].neighbor_list.size() > max_degree)
			{
				max_degree = node_array[i].neighbor_list.size();
			}
			average += node_array[i].neighbor_list.size();
		}
		average = average / node_array.size();

		sort(degree_array.begin(), degree_array.end());
		if (degree_array.size() % 2 == 0)
		{
			median = (degree_array[degree_array.size() / 2 - 1] + degree_array[degree_array.size() / 2]) / 2;
		}
		else
		{
			median = degree_array[degree_array.size() / 2];
		}

		cout << "average degree:  " << average << endl << "maximal degree:   " << max_degree << endl << "median:    " << median << endl;
	}

	float metropolis_simulation(long long int break_value)
	{

		string file_name = "ER_metropolis_p_" + std::to_string(prob_of_connection) + "_nodes_" + std::to_string(number_of_nodes) + ".txt";
		ofstream myfile(file_name);
		srand(time(NULL));
		float expected_value = (prob_of_connection*(double)number_of_nodes*(double)number_of_nodes)/2; // N squared for all nodes are taken into consideration
		std::vector<double> first_hundred_steps;
		std::vector<double> above_hundred_steps;
		double base_variance;
		long long int break_counter = 0;
		int edges = 0;
		while (break_value > break_counter)//will continue untill last if is satisfied break_value > break_counter
		{
			//cout <<expected_value<<endl;
			
			unsigned int node_i = rand() % node_array.size();
			unsigned int node_j = rand() % node_array.size();

			bool node_i_and_node_j_connected = false;

			for (int j = 0; j < node_array[node_i].neighbor_list.size(); j++)// checks if given nodes are already connected
			{
				if (node_array[node_i].neighbor_list[j] == node_j)
				{
					node_i_and_node_j_connected = true;
				}
			}

			if (node_i != node_j && node_i_and_node_j_connected == true)//if true then edge is created
			{
				unsigned int node_j_index = 0;
				unsigned int node_i_index = 0;

				auto node_j_position = std::find(begin(node_array[node_i].neighbor_list), end(node_array[node_i].neighbor_list), node_j);
				auto node_i_position = std::find(begin(node_array[node_j].neighbor_list), end(node_array[node_j].neighbor_list), node_i);

				node_array[node_i].neighbor_list.erase(node_j_position);
				node_array[node_j].neighbor_list.erase(node_i_position);

				edges--;


			}
			else if (node_i != node_j && node_i_and_node_j_connected == false && float(rand()) / float((RAND_MAX)) < exp(theta))//if true edge is deleted
			{
				node_array[node_i].neighbor_list.push_back(node_j);
				node_array[node_j].neighbor_list.push_back(node_i);
				edges++;

			}
			break_counter++;

			//variance part of code
			double variance;
			above_hundred_steps.push_back(edges);
			if(break_counter<100)
			{
				first_hundred_steps.push_back(edges);
			}
			

			//counts first time window variance 
			if(break_counter == 100)
			{	
				double sum = 0;
				for(int i = 0; i<first_hundred_steps.size(); i++)
				{
					sum += (first_hundred_steps[i]-expected_value)*(first_hundred_steps[i]-expected_value);
				}
				base_variance = sqrt(0.01*sum);				
			}	
			//counts time winodw varaince every 100 steps
			if (break_counter % 100 == 0)
			{
				double sum = 0;
				for(int i = 0; i<above_hundred_steps.size(); i++)
				{
					sum += (above_hundred_steps[i]-expected_value)*(above_hundred_steps[i]-expected_value);
				}
				variance = sqrt(0.01*sum);	
				above_hundred_steps.clear();
			}


			//test code
			unsigned int number_of_edges = 0;

			// if (break_counter % 1000 == 0)
			// {
			// 	//cout << break_counter << "\t" << edges << "\t" << variance/base_variance << endl;
			// 	if(variance/base_variance <= 0.01)
			// 	{
			// 		cout << break_counter << "\t" << edges << "\t" << variance/base_variance << endl;
			// 		break;
			// 	}
			// 	//myfile << break_counter << "\t" << edges << endl;
			// }

			if (break_counter % 100000 == 0)
			{
				//cout << break_counter << "\t" << edges << "\t" << variance/base_variance << endl;
				myfile << break_counter << "\t" << edges << "\t" << variance/base_variance << endl;
			}

			if (break_counter % 100000000 == 0)
			{
				cout << 100.00*(float(break_counter)/float(break_value))<<"%"<<"\t"<<break_counter << "\t" << edges << "\t" << variance/base_variance << endl;
				//myfile << break_counter << "\t" << edges << "\t" << variance/base_variance << endl;
			}

	

		}	
		//_sleep(100);
		myfile.close();
		cout<<"koniec"<<endl;
		return break_counter;
	}

};

int main()
{
	
float probabilities[3] = { 5, 2, 1};
int sizes[10] = {100,200,500,1000, 2000, 5000, 10000, 20000, 50000, 100000};
int size_2[4] = {500, 2000, 5000, 10000};
Graph graph1 = Graph(100000, 0.00001);
float stability = graph1.metropolis_simulation(40000000000);

// for(int i = 0; i<4; i++)
// {
// 	float prob = 1/(float)size_2[i];
// 	Graph graph1 = Graph(size_2[i], prob);
// 	float stability = graph1.metropolis_simulation(10000000);
// }

//ofstream myfile("stability_50k.txt");
// for(int i = 0; i<10; i++)
// {
// 	cout<<endl;
// 	cout<<sizes[i]<<endl;
// 	myfile<<endl;
// 	myfile<<sizes[i]<<endl;
// 	for(int j = 0; j< 3; j++)
// 	{
// 		cout<<endl;
// 		cout<<probabilities[j]<<endl;
// 		myfile<<endl;
// 		myfile<<probabilities[j]<<endl;
// 		for(int k = 0; k < 5; k++)
// 		{
// 			float prob = (1/(float)sizes[i])*probabilities[j];
// 			//cout<<"Prob:   "<<prob<<endl;
// 			Graph graph1 = Graph(sizes[i], prob);//(1/sizes[i])*probabilities[j])
// 			float stability = graph1.metropolis_simulation(1);
// 			myfile << stability<< endl;
// 		}
// 	}
// }

// for(int j = 0; j< 3; j++)
// 	{
// 		cout<<endl;
// 		cout<<probabilities[j]<<endl;
// 		myfile<<endl;
// 		myfile<<probabilities[j]<<endl;
// 		float prob = (1/(float)50000)*probabilities[j];
// 		//cout<<"Prob:   "<<prob<<endl;
// 		Graph graph1 = Graph(50000, prob);//(1/sizes[i])*probabilities[j])
// 		float stability = graph1.metropolis_simulation(1);
// 		myfile << stability<< endl;
	
// 	}
// myfile.close();
	//graph.metropolis_simulation(5000000);//5000000

	
}