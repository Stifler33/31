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
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;

    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;

    class Vertex
    {
        int name;
        std::vector<int> edgeTo;
    public:
        Vertex(){};
        Vertex(int _name, int _edgeTo): name(_name){
            edgeTo.push_back(_edgeTo);
        };
        Vertex(int _name): name(_name){};
        int getStart(){
            return name;
        }
        void addEdgeTo(int _edgeTo){
            edgeTo.push_back(_edgeTo);
        }
        std::vector<int> &GetEdgeTo(){
            return edgeTo;
        }
    };
    class MatrixGraph{
    public:
        virtual std::vector<std::shared_ptr<Vertex>> getFrom() = 0;
        virtual std::vector<std::shared_ptr<Vertex>> getTo() = 0;
    };
    class ListGraph{
    public:
        virtual std::vector<std::shared_ptr<Vertex>> getVer() = 0;
    };
};

class ListGraph: public IGraph
{
    std::vector<std::shared_ptr<Vertex>> listVer;
public:
    ListGraph(){};
    ListGraph(ListGraph* obj){
        listVer = obj->listVer;
    };
    ListGraph(IGraph::MatrixGraph* obj){
        listVer = obj->getFrom();
    };
    virtual void AddEdge(int from, int to) {
        for (auto const &ver : listVer){
            if (ver->getStart() == from){
                for (auto w : ver->GetEdgeTo()){
                    if (w == to){
                        std::cout << "such an edge already exists\n";
                        return;
                    }
                }
                ver->addEdgeTo(to);
                return;
            }
        }
        listVer.push_back(std::make_shared<Vertex>(from, to));
        listVer.push_back(std::make_shared<Vertex>(to));
    }
    std::vector<std::shared_ptr<Vertex>> getVer(){
        return listVer;
    }
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const{
        for (auto const &i : listVer){
            if (i->getStart() == vertex){
                if (!i->GetEdgeTo().empty()){
                    vertices = i->GetEdgeTo();
                    return;
                }else {
                    std::cout << "vertices empty\n";
                    return;
                }
            }
        }
        std::cout << "no vertex\n";
    }
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const{
        for (auto ptr : listVer){
            for (auto ver : ptr->GetEdgeTo()){
                if (ver == vertex){
                    vertices.push_back(ptr->getStart());
                }
            }
        }
        if (vertices.empty()) std::cout << "you can't get here\n";
    }
    virtual  int VerticesCount() const{
        return (int)listVer.size();
    }
    virtual ~ListGraph(){};
};

class MatrixGraph: public IGraph
{
    std::vector<std::shared_ptr<Vertex>> from;
    std::vector<std::shared_ptr<Vertex>> to;
public:
    MatrixGraph(){}
    MatrixGraph(MatrixGraph* obj){
        from = obj->from;
        to = obj->to;
    }
    MatrixGraph(IGraph::ListGraph* obj){
        from = obj->getVer();
        for (auto const &f : from){
            for (auto const &t : f->GetEdgeTo()){
                to.push_back(std::make_shared<Vertex>(t, f->getStart()));
            }
        }
    }
    virtual void AddEdge(int start, int where){
        for (auto const &a : from){
            if (a->getStart() == start){
                for (auto b : a->GetEdgeTo()){
                    if (b == where){
                        std::cout << "such an edge already exists\n";
                        return;
                    }
                }
                a->addEdgeTo(where);
                return;
            }
        }
        from.push_back(std::make_shared<Vertex>(start, where));
        to.push_back(std::make_shared<Vertex>(where, start));
    }
    std::vector<std::shared_ptr<Vertex>> getFrom(){
        return from;
    }
    std::vector<std::shared_ptr<Vertex>> getTo(){
        return to;
    }
    virtual int VerticesCount() const {
        return (int)from.size();
    }
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const{
        for (auto const &i : from){
            if (i->getStart() == vertex){
                if (!i->GetEdgeTo().empty()){
                    vertices = i->GetEdgeTo();
                    return;
                }else {
                    std::cout << "vertices empty\n";
                    return;
                }
            }
        }
        std::cout << "no vertex\n";
    }
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const{
        for (auto const &ptr : to){
           if (ptr->getStart() == vertex){
               if (!ptr->GetEdgeTo().empty()){
                   vertices = ptr->GetEdgeTo();
                   return;
               }else {
                   std::cout << "vertices empty\n";
                   return;
               }
           }
        }
        std::cout << "no vertex\n";
    }
    virtual ~MatrixGraph(){}
};

int main(){
    ListGraph graph;
    MatrixGraph mgraph(graph);
    graph.AddEdge(1,2);
    graph.AddEdge(3,4);
    graph.AddEdge(7,1);
    graph.AddEdge(1,6);
    ListGraph b(graph);
    graph.AddEdge(7,6);
    graph.AddEdge(6,1);
    std::cout << b.VerticesCount() << std::endl;
    std::cout << graph.VerticesCount() << std::endl;

    return 0;
}