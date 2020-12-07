#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <time.h> 
#include <algorithm>
#include <fstream>
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
    float theta;

    Graph(unsigned int number_of_nodes_in, float p)
    {
        float count_theta = log(p/(1-p));
        theta = count_theta;
        number_of_nodes = number_of_nodes_in;
        prob_of_connection = p;
        for(int i = 0; i<number_of_nodes; i++)
        {
            node_array.push_back(Node(i));
        }
    }

    void creat_connection(unsigned int node_i, unsigned int node_j)//creating edge between node number i and node number j
    {
        bool node_i_and_node_j_connected = false;

        for(int j = 0; j<node_array[node_i].neighbor_list.size(); j++)
        {
            if(node_array[node_i].neighbor_list[j] == node_j)
            {
                node_i_and_node_j_connected = true;
            }
        }

        if(node_i != node_j && node_i_and_node_j_connected == false)//if true then edge is created
            {
                node_array[node_i].neighbor_list.push_back(node_j); 
                node_array[node_j].neighbor_list.push_back(node_i);
            }
            else
            {
                cout<<"Edge already exists! Try differnet pair"<<endl;
            }
    }

    std::vector<Node> creat_graph_given_number_of_edges(unsigned int number_of_edges)//creating graph with given number of edges between randomly selected nodes
    {
        
        for(int i = 0; i<number_of_edges; i++)
        {
            srand(time(NULL));
            while(true)//will continue untill last if is satisfied
            {
                unsigned int node_i  = rand() % node_array.size() + 1;
                unsigned int node_j  = rand() % node_array.size() + 1;
                bool node_i_and_node_j_connected = false;

                for(int j = 0; j<node_array[node_i].neighbor_list.size(); j++)// checks if given nodes are already connected
                {
                    if(node_array[node_i].neighbor_list[j] == node_j)
                    {
                        node_i_and_node_j_connected = true;
                    }
                }

                if(node_i != node_j && node_i_and_node_j_connected == false)//if true then edge is created
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
        for(int i = 0; i <node_array.size(); i++)
        {
            for(int j = 0; j < node_array.size(); j++)
            {
                float random = float(rand())/float((RAND_MAX));

                 bool node_i_and_node_j_connected = false;

                for(int k = 0; k<node_array[i].neighbor_list.size(); k++)// checks if given nodes are already connected
                {
                    if(node_array[i].neighbor_list[k] == j)
                    {
                        node_i_and_node_j_connected = true;
                    }
                }

                if(random<prob_of_connection && i != j && node_i_and_node_j_connected == false)
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
        for(int i = 0; i<node_array.size(); i++)
        {
            cout<<i<<"    ";
            for(int j = 0; j<node_array[i].neighbor_list.size(); j++)
            {
                cout<<node_array[i].neighbor_list[j]<<" ";
            }
            if(node_array[i].neighbor_list.size() == 0)
            {
                cout<<"No connections"<<endl;
            }
            else
            {
                cout<<endl;
            }
            
        }
    }

    void show_degree()
    {
        for(int i = 0; i<node_array.size(); i++)
        {
            cout<<i<<"    "<<node_array[i].neighbor_list.size()<<endl;;
        }
    }

    void show_average_max_median_degree()
    {
        float max_degree = 0;
        unsigned int median = 0;
        float average = 0;
        std::vector<unsigned int> degree_array;
        for(int i = 0; i<node_array.size(); i++)
        {
            degree_array.push_back(node_array[i].neighbor_list.size());
            if(node_array[i].neighbor_list.size()>max_degree)
            {
                max_degree = node_array[i].neighbor_list.size();
            }
            average += node_array[i].neighbor_list.size();
        }
        average = average/node_array.size();

        sort(degree_array.begin(), degree_array.end());
        if (degree_array.size() % 2 == 0)
        {
            median =  (degree_array[degree_array.size()/ 2 - 1] + degree_array[degree_array.size()/ 2]) / 2;
        }
        else 
        {
            median = degree_array[degree_array.size() / 2];
        }

        cout<<"average degree:  "<<average<<endl<<"maximal degree:   "<<max_degree<<endl<<"median:    "<<median<<endl; 
    }

    void metropolis_simulation(unsigned int break_value)
    {

        string file_name = "ER_metropolis_p_"+std::to_string(prob_of_connection)+"_nodes_"+std::to_string(number_of_nodes)+".txt";
        //ofstream myfile (file_name);
        srand(time(NULL));
        unsigned int break_counter = 0;
        while(break_value>break_counter)//will continue untill last if is satisfied
        {
            unsigned int node_i  = rand() % node_array.size() + 1;
            unsigned int node_j  = rand() % node_array.size() + 1;
            bool node_i_and_node_j_connected = false;
            float random = float(rand())/float((RAND_MAX));

            for(int j = 0; j<node_array[node_i].neighbor_list.size(); j++)// checks if given nodes are already connected
            {
                if(node_array[node_i].neighbor_list[j] == node_j)
                {
                    node_i_and_node_j_connected = true;
                }
            }

            if(node_i != node_j && node_i_and_node_j_connected == false)//if true then edge is created
            {
                node_array[node_i].neighbor_list.push_back(node_j); 
                node_array[node_j].neighbor_list.push_back(node_i);
            }
            else if(node_i != node_j && (node_i_and_node_j_connected == true && random<exp(-theta)))//if true edge is deleted
            {
                unsigned int node_j_index = 0;
                unsigned int node_i_index = 0;
                
                auto node_j_position = std::find(begin(node_array[node_i].neighbor_list), end(node_array[node_i].neighbor_list), node_j);
                auto node_i_position = std::find(begin(node_array[node_j].neighbor_list), end(node_array[node_j].neighbor_list), node_i);

                //cout <<node_array[node_i].neighbor_list.size() <<"  "<<node_array[node_j].neighbor_list.size()<<endl;
                node_array[node_i].neighbor_list.erase(node_j_position);
                node_array[node_j].neighbor_list.erase(node_i_position);
                //cout <<node_array[node_i].neighbor_list.size() <<"  "<<node_array[node_j].neighbor_list.size()<<endl<<endl;

            }
            break_counter ++;
            
            //kod do testów
            unsigned int number_of_edges = 0;
            for(int i = 0; i<node_array.size(); i++)
            {
                number_of_edges += node_array[i].neighbor_list.size();
            }
            //cout <<node_i_and_node_j_connected<<endl;
            cout<<number_of_edges/2<<"  "<<exp(-theta)<<"   "<<random<<"    "<<theta<<endl;
            //cout<<number_of_edges/2<<"  "<<node_i<<"   "<<node_j<<endl;
            //_sleep(100);
            //myfile<<number_of_edges/2<<endl;
            //koniec kodu do testów
        }
        //myfile.close();

    }

};

int main()
{
    //Graph graph = Graph(1000,0.01);
    //float probabilities[3] = {0.01, 0.001, 0.0001};
    //int sizes[4] = {100,200,500,1000};
    //int iter[4] = {10000, 40000, 1000000, 5000000};
    //graph.metropolis_simulation(1000000);
    /*for(int i = 0; i<3; i++)
    {
        for(int j = 0; j<4; j++)
        {
            Graph graph = Graph(sizes[j],probabilities[i]);
            graph.metropolis_simulation(iter[j]);
            _sleep(1000);
        }
    }
    return 0;*/

    Graph graph1 = Graph(100, 0.05);
    graph1.metropolis_simulation(100000);
    

     /*Graph graph2 = Graph(100, 0.001);
    graph2.metropolis_simulation(10000);
    _sleep(1000);

    Graph graph3 = Graph(100, 0.0001);
    graph3.metropolis_simulation(10000);
    _sleep(1000);*/


    /*Graph graph4 = Graph(200, 0.01);
    graph4.metropolis_simulation(40000);
    _sleep(1000);

    Graph graph5 = Graph(200, 0.001);
    graph5.metropolis_simulation(40000);
    _sleep(1000);

    Graph graph6 = Graph(200, 0.0001);
    graph6.metropolis_simulation(40000);
    _sleep(1000);*/


    /*Graph graph7 = Graph(500, 0.01);
    graph7.metropolis_simulation(1000000);
    _sleep(1000);

    Graph graph8 = Graph(500, 0.001);
    graph8.metropolis_simulation(1000000);
    _sleep(1000);

    Graph graph9 = Graph(500, 0.0001);
    graph9.metropolis_simulation(1000000);
    _sleep(1000);


    Graph graph10 = Graph(1000, 0.01);
    graph10.metropolis_simulation(5000000);
    _sleep(1000);

    Graph graph11 = Graph(1000, 0.001);
    graph11.metropolis_simulation(5000000);
    _sleep(1000);

    Graph graph12 = Graph(1000, 0.0001);
    graph12.metropolis_simulation(5000000);
    _sleep(1000);*/

}