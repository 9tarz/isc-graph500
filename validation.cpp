/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: validation.cpp
  * 
  *	Note: All contain in this file from Boston University High Performance Computing's implementation
  */

#include <algorithm>
#include <deque>
#include <string>
#include <sstream>
#include <vector>

#include <assert.h>

#include <iostream>
#include "validation.h"

/**
 * Simple implementation of Graph to validate parent_arrays against.
 */
class Graph
{
public:
	Graph(Edges& edges, long long int VERTICIES)
    {
        // Allocating space
        this->VERTICIES = VERTICIES;
        this->adjacencyMatrix.resize(VERTICIES);
        for (int i = 0; i < this->adjacencyMatrix.size(); i++)
        {
            this->adjacencyMatrix[i].resize(VERTICIES);
        }

        // Undirected Graph
        for (int i = 0; i<edges.getSize(); i++)
        {
            VertexId v1 = edges.getEdges()[i], v2 = edges.getEdges()[i + edges.getSize()];
            this->adjacencyMatrix[v1][v2] = true;
            this->adjacencyMatrix[v2][v1] = true;
        }
    }

	bool edge_exists(VertexId v1, VertexId v2)
	{
        return this->adjacencyMatrix[v1][v2];
	}

	std::vector<VertexId> connected_component(VertexId v1)
	{
		std::vector<VertexId> component;
    std::vector<bool> visited_bits(VERTICIES);

    component.push_back(v1);
    visited_bits[v1] = true;

	  VertexId old_size;
    do
    {
      old_size = component.size();
      std::vector<VertexId> my_neighbors = this->neighbors(component.back());
      for (int i = 0; i < my_neighbors.size(); i++)
      {
        if (!visited_bits[my_neighbors[i]])
        {
          component.push_back(my_neighbors[i]);
          visited_bits[my_neighbors[i]] = true;
        }
      }
    } while (old_size != component.size());
		return component;
	}

	std::vector<int> graph_levels(VertexId root_id)
	{
	std::vector<int> levels(VERTICIES, -1);
        std::deque<VertexId> queue;

        levels[root_id] = 0;
        queue.push_back(root_id);

        while(!queue.empty())
        {
            VertexId vertex = queue.front();
            queue.pop_front();

            std::vector<VertexId> neighbors = this->neighbors(vertex);
            for (int i = 0; i < neighbors.size(); i++)
            {
                //Level=-1 implies not visited
                if (levels[neighbors[i]] < 0)
                {
                    levels[neighbors[i]] = levels[vertex] + 1;
                    queue.push_back(neighbors[i]);
                }
            }
        }
		return levels;	
	}

private:
    std::vector<VertexId> neighbors(VertexId v1)
    {
        std::vector<VertexId> myNeighbors;
        for (long long int i = 0; i < VERTICIES; i++)
        {
            if (this->adjacencyMatrix[v1][i] && i != v1)
                myNeighbors.push_back(i);
        }
        return myNeighbors;
    }

    std::vector< std::vector<bool> > adjacencyMatrix;
    long long int VERTICIES;
};

/**
 * Checks the following criteria:
 *
 * 1) The BFS tree is a tree and does not contain cycles
 *
 * @returns VALIDATION_SUCCESS(0) on success and ERR_HAS_CYCLE(1) on failure.
 */
RetType validate_cycles(VertexId* parent_array, VertexId root_id, long long int VERTICIES)
{
	VertexId vertex_id;
	std::vector<bool> visited(VERTICIES);

	// Cycle check
	for (long long int i = 0; i < VERTICIES; i++)
	{
		// If a node was already visited, it was part of another node's path
		// which was not cyclic
		if (visited[i] || parent_array[i] < 0) continue;

		std::vector<bool> vertex_bits(VERTICIES, false); //< Tracks which verticies
		                                          // are visited when
		                                          // traversing path i
		vertex_id = i; //< Tracks which vertex we are on in traversal
		do
		{

			//visiting a vertex twice in one path traversal implies a cycle
			if (vertex_bits[vertex_id])
			{
				return ERR_HAS_CYCLE;
			}

			// Track traversal and iterate
			vertex_bits[vertex_id] = true;
      visited[vertex_id] = true;

      if (parent_array[vertex_id] < 0)
      {
        std::cout << vertex_id << "->" << parent_array[vertex_id] << std::endl;
        assert(false);
      }
			vertex_id = parent_array[vertex_id];
		}
		//Do until we hit root
		while (vertex_id != root_id);
	}

  return VALIDATION_SUCCESS;
}

static std::vector<int> calc_expected_levels(VertexId* parent_array, VertexId root_id, long long int VERTICIES)
{
    //Level check
	std::vector<int> expected_levels(VERTICIES, -1); //< Stores the levels implied by parent_array
	expected_levels[root_id] = 0; //< root_id level always is zero
	std::deque<long long int> to_visit; 

	//Inital population of to_visit
	for (long long int i = 0; i < VERTICIES; i++)
	{

		//Level 1 can be assigned to here
		if (parent_array[i] == root_id)
			expected_levels[i] = 1;
		// If parent_array[i] < 0, not part of bfs tree
		else if (parent_array[i] >= 0)
			to_visit.push_back(i);
	}

	// Brute force is best force
	while (!to_visit.empty())
	{
		int id = to_visit.front();
    to_visit.pop_front();

		if (expected_levels[parent_array[id]] >= 0)
		{
			expected_levels[id] = expected_levels[parent_array[id]] + 1;
		}
    else
    {
      to_visit.push_back(id);
    }
	}

    return expected_levels;
}

/**
 * Checks the following criteria:
 *
 * 2) Each tree edge connects vertices whose BFS levels differ by exactly one
 *
 * @returns VALIDATION_SUCCESS(0) on success and ERR_INVALID_LEVEL(2) on failure.
 */
RetType validate_levels(VertexId* parent_array, VertexId root_id, Graph graph, long long int VERTICIES)
{
	std::vector<int> expected_levels = calc_expected_levels(parent_array, root_id, VERTICIES);
	std::vector<int> graph_levels = graph.graph_levels(root_id);

	for(long long int i = 0; i < VERTICIES; i++)
	{
		if (parent_array[i] > 0 && expected_levels[i] != graph_levels[i])
		{
			return ERR_INVALID_LEVEL;
		}
	}

	return VALIDATION_SUCCESS;
}


/**
 * Checks the following criteria:
 *
 * 3) Every edge in the input list has vertices with levels that differ by
 *    at most one or that both are not in the BFS tree
 *
 * @returns VALIDATION_SUCCESS(0) on success and ERR_INVALID_GRAPH(4) on failure.
 */
RetType validate_graph(VertexId* parent_array, VertexId root_id, Edges& edges, long long int VERTICIES)
{
    std::vector<int> levels = calc_expected_levels(parent_array, root_id, VERTICIES);
    for (long long int i = 0; i<edges.getSize(); i++)
    {
        if (parent_array[edges.getEdges()[i]] < 0) continue;

        int diff = levels[edges.getEdges()[i]] - levels[edges.getEdges()[i + edges.getSize()]];
        if (diff > 1 or diff < -1)
        {
            return ERR_INVALID_GRAPH;
        }
    }
    return VALIDATION_SUCCESS;
}

/**
 * Checks the following criteria:
 *
 * 4) The BFS tree spans an entire connected component's vertices
 *
 * @returns VALIDATION_SUCCESS(0) on success and ERR_DOESNT_SPAN(8) on failure.
 */
RetType validate_span(VertexId* parent_array, VertexId root_id, Graph graph, long long int VERTICIES)
{
	//Gets all vertex_ids in the connected component
	std::vector<VertexId> vertices = graph.connected_component(root_id);

	//Iterate through connected component vertecies
	for (auto it = vertices.begin(); it != vertices.end(); it++)
  {
		//root_id is taken to always be visited, other -1's imply not in connected component
		if (!(*it == root_id or parent_array[*it] >= 0))
	  {
      return ERR_DOESNT_SPAN;
    }
  }

	return VALIDATION_SUCCESS;

}

/**
 * Checks the following criteria:
 *
 * 5) A node and its parent are joined by an edge of the original graph
 *
 * @returns VALIDATION_SUCCESS(0) on success and ERR_UNREAL_EDGE(16) on failure.
 */
RetType validate_edges(VertexId* parent_array, Graph graph, long long int VERTICIES)
{
	for (long long int i = 0; i < VERTICIES; i++)
	{
		// -1 denotes no edge exists
		if (parent_array[i] >= 0 && !graph.edge_exists(i, parent_array[i]))
				return ERR_UNREAL_EDGE;
	}
	return VALIDATION_SUCCESS;
}

// Ors together all validation flags to return 
RetType validate(VertexId* parent_array, VertexId root_id, Edges& edges, long long int VERTICIES)
{
  Graph graph(edges, VERTICIES);

	RetType code = VALIDATION_SUCCESS;
	//std::cout << "validate_cycles" << std::endl;
  	code |= validate_cycles(parent_array, root_id, VERTICIES);
  	//std::cout << "validate_levels" << std::endl;
	code |= validate_levels(parent_array, root_id, graph, VERTICIES);
	//std::cout << "validate_graph" << std::endl;
	code |= validate_graph(parent_array, root_id, edges, VERTICIES);
	//std::cout << "validate_span" << std::endl;
	code |= validate_span(parent_array, root_id, graph, VERTICIES);
	//std::cout << "validate_edges" << std::endl;
	code |= validate_edges(parent_array, graph, VERTICIES);
         ;
	if (code == VALIDATION_SUCCESS)
		return 0;
	else
	{
		std::ostringstream oss;
		oss << "Validation failed(";

		bool prev = false;
		if (code & ERR_HAS_CYCLE)
		{
			oss << (prev?"|":"") << "ERR_HAS_CYCLE";
			prev = true;
		}
		if (code & ERR_INVALID_LEVEL)
		{
			oss << (prev?"|":"") << "ERR_INVALID_LEVEL";
			prev = true;
		}
		if (code & ERR_INVALID_GRAPH)
		{
			oss << (prev?"|":"") << "ERR_INVALID_GRAPH";
			prev = true;
		}
		if (code & ERR_DOESNT_SPAN)
		{
			oss << (prev?"|":"") << "ERR_DOESNT_SPAN";
			prev = true;
		}
		if (code & ERR_UNREAL_EDGE)
		{
			oss << (prev?"|":"") << "ERR_UNREAL_EDGE";
			prev = true;
		}
		oss << ").";
		//throw Error(oss.str());
	}
}

long long int getNumEdges(VertexId* parent_array, long long int VERTICIES)
{
	long long int edgeCount = 0;
	for (long long int i = 0; i < VERTICIES; i++)
	{
		if (parent_array[i] >= 0)
		{
				edgeCount++;
		}
	}
	return edgeCount;
}
