#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <time.h> 
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
//#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
	unsigned int index;
	std::vector<unsigned int> neighbor_list;
	std::vector<unsigned int> road_list;
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
    float gamma = 0.57722;
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

    
    // 
    //
    // CZĘŚĆ KODU ODPOWIADAJĄCA ZA LICZENIE ŚREDNIEJ DROGI. BFS JEST NAJWAŻNIEJSZĄ FUNKCJĄ RESZTA TO OZDOBNIKI
    //
    //
    //
    bool BFS(unsigned int source,std::vector<int> &dist) //breath first search function
    {
        if(node_array[source].neighbor_list.size() < 1)
        {
            //przydzielanie kary
			//dist[source] = number_of_nodes;
            return false;
        }
        else 
        {
            list<int> queue;
			std::vector<bool> visited(node_array.size());
            //bool visited[node_array.size()];
            //int pred[node_array.size()], dist[node_array.size()]

            for(int i = 0; i<node_array.size(); i++)
            {
                visited[i] = false;
                dist[i] = 0;
            }

            visited[source] = true;
            dist[source] = 0;
            queue.push_back(source);

            while(!queue.empty())
            {
                int u = queue.front();
                queue.pop_front();
                for(int i = 0; i < node_array[u].neighbor_list.size(); i++)
                {
                    if(visited[node_array[u].neighbor_list[i]] == false)
                    {
                        visited[node_array[u].neighbor_list[i]] = true;
                        dist[node_array[u].neighbor_list[i]] = dist[u] + 1;
                        queue.push_back(node_array[u].neighbor_list[i]);

                    }
                }
            }
            return false;
        }
        
    }

    float averageShortestPath_no_file(bool check_for_inf)
    {
        double sum_of_path = 0;
        double number_of_paths = 0;
		std::vector<int> dist(node_array.size());
        for(int i = 0; i < node_array.size(); i++)
        {   
			BFS(i, dist);
			for (int j = 0; j < node_array.size(); j++)
				if (dist[j] != 0)
				{
					number_of_paths++;
					sum_of_path += dist[j];
				}
				else
				{
					if(i!=j) sum_of_path += number_of_nodes;
				}
        }
        //cout<<"Average path: "<<sum_of_path/number_of_paths<<" Number of paths: "<<number_of_paths<<" Sum of path: "<<sum_of_path<<endl;
		if(check_for_inf == true)
		{
			return (sum_of_path/1);
		}
		else
		{
			return (sum_of_path/number_of_paths);
		}
				
    }
    //
    //
    //KONIEC KODU ODPOWIADAJĄCEGO ZA RZECZY ZWIAZANE ZE SREDNIA DROGA
    //
    //
	unsigned int return_random()
	{
		srand(time(NULL));
		return (rand() % node_array.size());
	}
	float metropolis_simulation_average_path(float averag_path, float T, int numer)
	{

		string file_name = "Metropolis_average_path_"+std::to_string((int)averag_path)+ "_nodes_" + std::to_string(number_of_nodes) +"_nr_"+std::to_string(numer) +"_Temp_"+std::to_string(T) +".txt";
		ofstream myfile(file_name);
		float expected_value = (prob_of_connection*(float)number_of_nodes*(float)number_of_nodes)/2; // N squared for all nodes are taken into consideration
        int starting_average_path = ((number_of_nodes*(number_of_nodes-1))/2)*number_of_nodes;//lemat o uścisku dłoni
        double start_energy = (1/T)*((starting_average_path - averag_path)*(starting_average_path - averag_path));
		double old_energy = start_energy;
		std::vector<float> first_hundred_steps;
		std::vector<float> above_hundred_steps;
		std::vector<float> current_average_array;
		std::vector<double> energy_array;
		std::vector<int> hundred_edge_array;
		std::vector<int> edge_array;
		float base_variance;
		unsigned int break_counter = 0;
		int edges = 0;
		int average_counter = 0;
		int energy_counter = 0;
		bool sequence_check = false;
		int acceptance = 0;
		int positive_delta = 0;
		srand(time(NULL));
		while (true)//will continue untill break
		{
			
			float current_average = starting_average_path;
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
			if(node_i != node_j && node_i_and_node_j_connected == false)//if true edge is created  float(rand()) / float((RAND_MAX)) < exp(theta) (current_average > averag_path)
			{
				node_array[node_i].neighbor_list.push_back(node_j);
				node_array[node_j].neighbor_list.push_back(node_i);
				edges++;
				node_i_and_node_j_connected = true;

			}
			else if(node_i != node_j && node_i_and_node_j_connected == true)
			{
				auto node_j_position = std::find(begin(node_array[node_i].neighbor_list), end(node_array[node_i].neighbor_list), node_j);
				auto node_i_position = std::find(begin(node_array[node_j].neighbor_list), end(node_array[node_j].neighbor_list), node_i);

				node_array[node_i].neighbor_list.erase(node_j_position);
				node_array[node_j].neighbor_list.erase(node_i_position);
				node_i_and_node_j_connected = false;
				edges--;
			}
			if(break_counter > 0)
			{
				current_average =  Graph::averageShortestPath_no_file(false);
			}
			
			double energy = (1/T)*((current_average - averag_path)*(current_average - averag_path));
			double delta_energy = energy - old_energy;
			old_energy = energy;

			double random_number = (double) rand()/RAND_MAX;
			if(delta_energy >= 0)
			{
				positive_delta++;
				
				if((random_number > exp(-delta_energy)) && node_i_and_node_j_connected == true)
				{
					auto node_j_position = std::find(begin(node_array[node_i].neighbor_list), end(node_array[node_i].neighbor_list), node_j);
					auto node_i_position = std::find(begin(node_array[node_j].neighbor_list), end(node_array[node_j].neighbor_list), node_i);

					node_array[node_i].neighbor_list.erase(node_j_position);
					node_array[node_j].neighbor_list.erase(node_i_position);
					acceptance++;
					edges--;
					old_energy = energy - delta_energy;
				}
				else if((random_number > exp(-delta_energy)) && node_i_and_node_j_connected == false)
				{
					node_array[node_i].neighbor_list.push_back(node_j);
					node_array[node_j].neighbor_list.push_back(node_i);
					acceptance++;
					edges++;
					old_energy = energy - delta_energy;
				}
				
			}
			
			//cout<<break_counter<<"\t"<<edges<<"\t"<<current_average<<"\t"<<energy<<'\t'<< delta_energy<<"\t"<<random_number<<"\t"<<random_number-delta_energy <<endl;
			int break_check = 10;
            if( break_counter%break_check == 0)
			{
				cout<<break_counter<<"\t"<<edges<<"\t"<<current_average<<"\t"<<energy<<endl;//'\t'<< delta_energy<<"\t"<<random_number<<"\t"<<random_number-delta_energy <<endl;
                current_average_array.push_back(current_average); 
                hundred_edge_array.push_back(edges);
				energy_array.push_back(energy);
			}
			edge_array.push_back(edges);
			

			if(energy < 0.01)//if(energy > -0.01 && energy < 0.01)
			{
				energy_counter++;
			}
			else
			{
				energy_counter = 0;
			}

			if(energy_counter > 300)
			{
				sequence_check = true;
			}

			if( (sequence_check == true  && break_counter%break_check == 0) || break_counter > 10000)
			{
				
				break;
			}

            break_counter++;
            
		}	

		//variance part of code
		int break_check = 10;
		std::vector<float> variance_fractorial;
			float variance;
			
			//counting average
			double sum_of_edges = 0;
			for(auto it: edge_array)
			{
				sum_of_edges += it;
			}

			double average = sum_of_edges/break_counter;
			//counts first time window variance 
			
			float sum_base_variance = 0;
			for(int i = 0; i<100; i++)
			{
				sum_base_variance += (edge_array[i]-average)*(edge_array[i]-average);
			}
			base_variance = sqrt(0.01*sum_base_variance);				
			//counts time winodw varaince every 100 steps
			for(int i = 0; i<break_counter; i++)
			{
				above_hundred_steps.push_back(edge_array[i]);
				if (i % 100 == 0)
				{

					float sum = 0;
					for(int i = 0; i<above_hundred_steps.size(); i++)
					{
						sum += (above_hundred_steps[i]-average)*(above_hundred_steps[i]-average);
					}
					variance = sqrt(0.01*sum);	
					variance_fractorial.push_back(variance/base_variance);
					above_hundred_steps.clear();
				}
			}
			
		myfile<<"iteracja"<<"\t"<<"krawedzie"<<"\t"<<"srednia_chwil"<<"\t"<<"energia"<<'\t'<<"variancja"<<endl;
		std::vector <float> temp_curent;
		std::vector <float> temp_edges;
		for(int i = 0; i<break_counter; i++)
		{
	
			if (i % break_check == 0)
			{

                //cout<<i<<"\t"<<edge_array[(int)((i/break_check))]<<"\t"<<current_average_array[(int)((i/break_check))]<<"\t"<<energy_array[(int)((i/break_check))]<<'\t'<<variance_fractorial[(int)((i/100))]<<endl;
				myfile<<i<<"\t"<<edge_array[(int)((i/break_check))]<<"\t"<<current_average_array[(int)((i/break_check))]<<"\t"<<energy_array[(int)((i/break_check))]<<'\t'<<variance_fractorial[(int)((i/100))]<<endl;
			
			}
		}
		myfile.close();

		//zapisywanie par do pliku do wizualizacji
		string file_name_2 = "Metropolis_average_path_"+std::to_string((int)averag_path)+ "_nodes_" + std::to_string(number_of_nodes) +"_nr_"+std::to_string(numer) +"_Temp_"+std::to_string(T) +"_pary.txt";
		ofstream myfile_2(file_name_2);
		for(int i  = 0; i<node_array.size(); i++)
		{
			for(int j = 0; j<node_array[i].neighbor_list.size(); j++)
			{
				myfile_2<<node_array[i].index<<"\t"<< node_array[i].neighbor_list[j] <<endl;
			}
		}
		myfile_2.close();

		return break_counter;
	}
};

int main()
{

	std::vector<int> N_arr = {100, 200, 300, 400, 1000};
	for(int i = 0; i<7; i++)
	{
		cout<<"Licze dla N: "<<N_arr[i]<<endl;
		Graph graph1 = Graph(N_arr[i], 0.1);
		graph1.metropolis_simulation_average_path(6, (double)(1.0/((double)N_arr[i] * 100.0)), 11);
	}
	
	//int arr_len = 3;
	//int node_arr[arr_len] = {100, 200, 500};
	//float prob_arr[arr_len] = {0.01, 0.005, 0.002, 0.001, 0.0005};// to jest wymagany przez konstruktor klasy Graph. Nie jest używany do symualcji
	// for(int i = 0; i<arr_len; i++)
	// {
	// 	cout<<"Licze dla: "<<node_arr[i]<<endl;
	// 	Graph graph1 = Graph(node_arr[i], prob_arr[i]);
	// 	graph1.metropolis_simulation_average_path(6);
	// }
    // cout<<"Licze dla: "<<100<<endl;
	// Graph graph1 = Graph(500, 0.01);
	// graph1.metropolis_simulation_average_path(20, 0.001, 0);
	// cout<<endl<<endl<<graph1.node_array.size();


	// int node_arr[3] = {100, 200, 500};
	// float temp_arr[3] = {1, 100, 1000};
	// int path_len[3] = {5, 10, 20};
	// int licznik = 0;
	// for(int i = 0; i<3;i++)
	// {
	// 	for(int j = 0; j< 3; j++)
	// 	{
	// 		for(int k = 0; k< 3; k++)
	// 		{
	// 			cout<<((float)i/27)*100<<" % Nodes: "<< node_arr[i]<<" Len: "<<path_len[j] <<" Temp: "<<temp_arr[k] <<" Licznik: "<<licznik<<endl;
	// 			Graph graph1 = Graph(node_arr[i], 0.01);
	// 			graph1.metropolis_simulation_average_path(path_len[j], temp_arr[k], 0);
	// 			licznik ++;
	// 		}
	// 	}
	// }



    return 0;
}