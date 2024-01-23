template<typename T>
graph_list<T>::graph_list(size_t vertex_count)
    : _graph(vertex_count, std::vector<std::pair<int, int>>{})
{
}

template<typename T>
graph_list<T>::graph_list(const graph_list<T>& oth)
{
    _graph = oth._graph;
}

template<typename T>
graph_list<T>::graph_list(graph_list<T>&& oth) noexcept
{
    _graph = std::move(oth._graph);
}

template<typename T>
graph_list<T>& graph_list<T>::operator=(const graph_list<T>& rhs)
{
    if (this != &rhs)
    {
        _graph = rhs._graph;
    }
    return *this;
}

template<typename T>
graph_list<T>& graph_list<T>::operator=(graph_list<T>&& rhs) noexcept
{
    if (this != &rhs)
    {
        _graph = std::move(rhs._graph);
    }
    return *this;
}

template<typename T>
void graph_list<T>::add_vertex()
{
    _graph.push_back({});
}

template<typename T>
void graph_list<T>::add_edge(size_t u, size_t v, size_t w)
{
    if (u >= _graph.size() || v >= _graph.size()) throw std::out_of_range("Vertex index out of range");

    for (auto next : _graph[u])
    {
        if (next.first == v) return;
    }

    _graph[u].push_back({v,w});
 _graph[v].push_back({u, w});   //for undirected graph
}

template<typename T>
void graph_list<T>::transpose()
{
    graph_list<T> tmp(this->_graph.size());

    for (int i = 0; i < _graph.size(); ++i)
    {
        for (int j = 0; j < _graph[i].size(); ++j)
        {
            tmp._graph[_graph[i][j].first].push_back({i, _graph[i][j].second});
        }
    }
    _graph = std::move(tmp._graph);
}

template<typename T>
bool graph_list<T>::has_cycle_undirected() const
{
    visited_type visited(_graph.size(), false);
    for (int i = 0; i < _graph.size(); ++i)
    {
        if (!visited[i])
        {
            if (has_cycle_undirected_helper(i, -1, visited)) return true;
        }
    }
    return false;
}

template<typename T>
bool graph_list<T>::has_cycle_undirected_helper(size_t source, size_t parent, visited_type& visited) const
{
    visited[source] = true;

    for (auto next : _graph[source])
    {
        if (!visited[next.first])
        {
            if (has_cycle_undirected_helper(next.first, source, visited)) return true;
        }

        else if (next.first != parent) return true;
    }
    return false;
}

template<typename T>
bool graph_list<T>::has_cycle_directed() const
{
    visited_type visited(_graph.size(), false);
    visited_type on_rec_stack(_graph.size(), false);

    for (int i = 0; i < _graph.size(); ++i)
    {
        if (!visited[i])
        {
            if (has_cycle_directed_helper(i, visited, on_rec_stack)) return true;
        }
    }
    return false;
}

template<typename T>
bool graph_list<T>::has_cycle_directed_helper(size_t source, visited_type& visited, visited_type& on_rec_stack) const
{
    visited[source] = true;
    on_rec_stack[source] = true;

    for (auto next : _graph[source])
    {
        if (!visited[next.first])
        {
            if (has_cycle_directed_helper(next.first, visited, on_rec_stack)) return true;
        }

        else if (on_rec_stack[next.first]) return true;
    }

    on_rec_stack[source] = false;
    return false;
}

template<typename T>
size_t graph_list<T>::component_count() const
{
    size_t count = 0;
    visited_type visited(_graph.size(), false);
    std::stack<size_t> stack;

    for (int i = 0; i < _graph.size(); ++i)
    {
        if (!visited[i])
        {
            stack.push(i);
            ++count;

            while (!stack.empty())
            {
                size_t curr = stack.top();
                visited[curr] = true;
                stack.pop();

                for(auto next : _graph[curr])
                {
                    if (!visited[next.first])
                    {
                        stack.push(next.first);
                        visited[next.first] = true;
                    }
                }
            }
        }
    }
    return count;
}

template<typename T>
void graph_list<T>::dfs(size_t start, callback_type callback) const
{
    if (start >= _graph.size()) throw std::out_of_range("vertex index is great");

    visited_type visited(_graph.size(), false);
    dfs_helper_recrusive(start, visited, callback);
//    dfs_helper_iterative(start, visited, callback);
}

template<typename T>
void graph_list<T>::dfs_extra_case(size_t start, callback_type callback) const
{
    visited_type visited(_graph.size(), false);
    for (int i = 0; i < _graph.size(); ++i)
    {
        if (!visited[i])
        {
            dfs_helper_recrusive(i, visited, callback);
        }
       std::cout << std::endl;
    }
}

template<typename T>
void graph_list<T>::dfs_helper_recrusive(size_t current, visited_type& visited,
                                                         callback_type callback) const
{
    visited[current] = true;
    callback(current);

    for (auto next : _graph[current])
    {
        if (!visited[next.first])
        {
            dfs_helper_recrusive(next.first, visited, callback);
        }
    }
}

template<typename T>
void graph_list<T>::dfs_helper_iterative(size_t start, visited_type& visited,
                                                       callback_type callback) const
{
    std::stack<size_t> rec_stack;
    rec_stack.push(start);
    visited[start] = true;

    while (!rec_stack.empty())
    {
        size_t current = rec_stack.top();
        rec_stack.pop();
        callback(current);

        for (auto next : _graph[current])
        {
            if (!visited[next.first])
            {
                rec_stack.push(next.first);
                visited[next.first] = true;
            }
        }
    }
}

template<typename T>
void graph_list<T>::bfs(size_t start, callback_type callback) const
{
    if (start >= _graph.size()) throw std::out_of_range("vertex index is great");

    visited_type visited(_graph.size(), false);
    bfs_helper(start, visited, callback);
}

template<typename T>
void graph_list<T>::bfs_extra_case(size_t start, callback_type callback) const
{
    if (start >= _graph.size()) throw std::out_of_range("vertex index is great");
    
    visited_type visited(_graph.size(), false);
    for (int i = 0; i < _graph.size(); ++i)
    {
        if (!visited[i])
        {
            bfs_helper(i, visited, callback);
        }
       std::cout << std::endl;
    }
}

template<typename T>
void graph_list<T>::bfs_helper(size_t start, visited_type& visited, callback_type callback) const
{
    std::queue<size_t> QUEUE;
    QUEUE.push(start);
    visited[start] = true;

    while (!QUEUE.empty())
    {
        size_t current = QUEUE.front();
        QUEUE.pop();
        callback(current);

        for (auto next : _graph[current])
        {
            if (!visited[next.first])
            {
                QUEUE.push(next.first);
                visited[next.first] = true;
            }
        }
    }
}

template<typename T>
size_t  graph_list<T>::vertex_count_in_level(size_t vertex, size_t level) const
{
    if (vertex >= _graph.size() || level >= _graph.size()) throw std::invalid_argument("args is not valid");

    visited_type visited(_graph.size(), false);
    std::queue<size_t> queue;
    queue.push(vertex);
    visited[vertex] = true;

    while (!queue.empty())
    {
        int size = queue.size();
        if (!level) return size;
        while (size--)
        {
            size_t current = queue.front();
            queue.pop();
            for(auto next : _graph[current])
            {
                if (!visited[next.first])
                {
                    queue.push(next.first);
                    visited[next.first] = true;
                }
            }
        }
        --level;
    }
    return 0;
}

template<typename T>
size_t graph_list<T>::shortest_path_two_vertex(size_t source, size_t destination) const
{
    if (source >= _graph.size() || destination >= _graph.size()) throw std::invalid_argument("args is not valid");
    if (source == destination) return 0;
    
    visited_type visited(_graph.size(), false);
    std::queue<size_t> queue;
    size_t count = 0;
    queue.push(source);
    visited[source] = true;

    while (!queue.empty())
    {
        int size = queue.size();
        ++count;
        while (size--)
        {
            size_t current = queue.front();
            queue.pop();

            for (auto next : _graph[current])
            {
                if (next.first == destination) return count;

                if (!visited[next.first])
                {
                    queue.push(next.first);
                    visited[next.first] = true;
                }
            }
        }
    }

    std::cout << "There is no path " << "from " << source << " to " << destination << "\n";
    return INT_MAX;
}

template<typename T>
typename graph_list<T>::matrix_type graph_list<T>::all_paths_two_vertex(size_t source, size_t destination) const
{
    if (source >= _graph.size() || destination >= _graph.size()) throw std::invalid_argument("args is not valid");

    matrix_type paths;
    std::vector<int> raw_path(_graph.size(), -1);
    visited_type visited(_graph.size(), false);
    all_paths_two_vertex_helper(source, destination, visited, raw_path, paths);

    return paths;
}

template<typename T>
void graph_list<T>::all_paths_two_vertex_helper(size_t source,
                                                size_t destination,
                                                visited_type& visited,
                                                std::vector<int>& raw_path,
                                                matrix_type& paths) const
{
    visited[source] = true;

    for (auto next : _graph[source])
    {
        if (!visited[next.first])
        {
            raw_path[next.first] = source;
            if (next.first == destination)
            {
              paths.push_back(reconstruct(source, destination, raw_path));
            }
            else
            {
                all_paths_two_vertex_helper(next.first, destination, visited, raw_path, paths);
            }
        }
    }

    visited[source] = false;
}

template<typename T>
std::vector<int> graph_list<T>::reconstruct(size_t source, size_t destination, std::vector<int>& raw_path) const
{
    std::vector<int> path;
    size_t tmp = destination;
    while (raw_path[destination] != -1)
    {
        path.insert(path.begin(), raw_path[destination]);
        destination = raw_path[destination];
    }

    path.push_back(tmp);
    return path;
}

template<typename T>
std::vector<int> graph_list<T>::topological_sort() const // This is NOT RECOMMENDED algorithm !!!!!!!!!!! instead use Kahn's algorithm
{
    

   
}

template<typename T>
void graph_list<T>::topological_sort_helper(size_t source, visited_type& visited, std::vector<int>& result) const
{
   
}

template<typename T>
std::vector<int> graph_list<T>::topological_sort_Kahns_algorithm() const        // Use this algorithm !!!
{
   

}


//-------------------SCC-------------------//

template<typename T>
typename graph_list<T>::matrix_type graph_list<T>::scc_Kosarajus_algorithm() const
{
    

    // 1. first pass, fill the stack with finishing time of vertices
   

    // 2. second pass, strongly connected component detecting
   
}

template<typename T>
void graph_list<T>::scc_Kosarajus_algorithm_helper_first_pass(size_t source,  
                                                              visited_type& visited, 
                                                              std::stack<size_t>& finish_time) const
{
   
}

template<typename T>
void graph_list<T>::scc_Kosarajus_algorithm_helper_second_pass(size_t source,
                                                               visited_type& visited,
                                                               std::vector<int>& scc,
                                                               matrix_type& transposed_list) const
{
    
}

template<typename T>
typename graph_list<T>::matrix_type graph_list<T>::scc_Tarjans_algorithm() const
{
  
}

template <typename T>
void graph_list<T>::scc_Tarjans_algorithm_helper(size_t source,
                                                 visited_type& on_stack,
                                                 std::stack<size_t>& stack,
                                                 std::vector<int>& ids,
                                                 std::vector<int>& low_links,
                                                 matrix_type& result) const
{
   
}

template<typename T>
void graph_list<T>::default_operation(size_t vertex)
{
    std::cout << vertex << " ";
}

template<typename T>
void graph_list<T>::print_list() const
{
    for (size_t i = 0; i < _graph.size(); ++i)
    {
        std::cout << i << " -> ";
        for (size_t j = 0; j < _graph[i].size(); ++j)
        {
            std::cout << "\033[0;34m " << " "<< _graph[i][j].second  // edge weight color blue
                      << "\033[1;32m" << " " << _graph[i][j].first  // edge color green
                      <<  "\033[0m" << " |";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}