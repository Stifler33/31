#include <iostream>
#include <vector>
#include <memory>

class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() {};
    IGraph(IGraph *_oth) {};
    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин
    //virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    //virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную
};

class ListGraph: public IGraph
{
    class Vertex
    {
        int start;
        std::vector<int> where;
    public:
        Vertex(){};
        Vertex(int _start, int _where): start(_start){
            where.push_back(_where);
        };
        Vertex(int _start): start(_start){};
        int getStart(){
            return start;
        }
        void addWhere(int _where){
            where.push_back(_where);
        }
    };
    std::vector<std::shared_ptr<Vertex>> listVer;
public:
    ListGraph(){};
    ListGraph(ListGraph* obj){
        listVer = obj->listVer;
    };
    virtual void AddEdge(int from, int to) {
        for (auto const &ver : listVer){
            if (ver->getStart() == from){
                ver->addWhere(to);
                return;
            }
        }
        listVer.push_back(std::make_shared<Vertex>(from, to));
        listVer.push_back(std::make_shared<Vertex>(to));
    }
    virtual  int VerticesCount() const{
        return (int)listVer.size();
    }
    virtual ~ListGraph(){};
};

class MatrixGraph
{

};

int main(){
    ListGraph graph;
    graph.AddEdge(1,2);
    graph.AddEdge(3,4);
    graph.AddEdge(5,6);
    graph.AddEdge(1,6);
    ListGraph b(graph);
    graph.AddEdge(7,6);
    std::cout << b.VerticesCount() << std::endl;
    std::cout << graph.VerticesCount() << std::endl;
    return 0;
}