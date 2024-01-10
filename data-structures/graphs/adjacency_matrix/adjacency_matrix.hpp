#ifndef  AJ_MATRIX
#define  AJ_MATRIX

#include <iostream>
#include <vector>
#include <utility>

template<typename T>
class graph_matrix 
{
public:
    graph_matrix                        () = default;
    graph_matrix                            (size_t);
    graph_matrix               (const graph_matrix&);
    graph_matrix           (graph_matrix&&) noexcept;
    graph_matrix&     operator=(const graph_matrix&);
    graph_matrix& operator=(graph_matrix&&) noexcept;

public:
    void                   print_matrix();
    void     add_edge(size_t u, size_t v);
    void                     add_vertex();
    void                      transpose();
    void                      dfs() const;   // iterative, recursive
    void                      bfs() const;
    void          component_count() const;
    void     all_paths_two_vertex() const;
    void    vertex_count_in_level() const;
    void shortest_path_two_vertex() const;

private:
    std::vector<std::vector<int>> _graph;
};

#include "adjacency_matrix.tpp"

#endif // AJ_MATRIX



