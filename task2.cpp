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
            for (auto i : edgeTo){
                if (i == _edgeTo) return;
            }
            edgeTo.push_back(_edgeTo);
        };
        Vertex(int _name): name(_name){};
        int getStart(){
            return name;
        }
        void addEdgeTo(int _edgeTo){
            for (auto i : edgeTo){
                if (i == _edgeTo) return;
            }
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
    ListGraph(IGraph* _oth){
        int count = _oth->VerticesCount();
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
    int countVertex = 0;
public:
    MatrixGraph(){}
    MatrixGraph(IGraph *_oth) {
        int countVertex = _oth->VerticesCount();
    }
    virtual void AddEdge(int start, int where){
        bool foundFrom = false;
        bool foundTo = false;
       for (auto const& f : from){
           if (f->getStart() == start){
               foundFrom = true;
               f->addEdgeTo(where);
           }
       }
        for (auto const& t : to){
            if (t->getStart() == where){
                foundTo = true;
                t->addEdgeTo(start);
            }
        }
        if (!foundFrom && !foundTo) {
            from.push_back(std::make_shared<Vertex>(start, where));
            to.push_back(std::make_shared<Vertex>(where, start));
            from.push_back(std::make_shared<Vertex>( where));
            to.push_back(std::make_shared<Vertex>( start));
        }
        if (foundFrom && !foundTo) {
            from.push_back(std::make_shared<Vertex>(where));
            to.push_back(std::make_shared<Vertex>(where, start));
        }
        if (!foundFrom && foundTo){
            from.push_back(std::make_shared<Vertex>(start, where));
            to.push_back(std::make_shared<Vertex>(start));
        }
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

    ListGraph list;
    list.AddEdge(1,2);
    MatrixGraph graph(&list);

    std::cout << std::endl;
    std::cout << "Count vertex\n";
    std::cout << graph.VerticesCount() << std::endl;

    return 0;
}