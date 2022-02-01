#include <iostream>
#include <vector>
#include <memory>
#include <map>
class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() {};
    IGraph(IGraph *_oth) {};

    virtual IGraph& operator=(const IGraph& _oth) {};
    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин
    virtual void GetVertices(std::vector<int> &vertices) const = 0; //считываем в вектор все имена вершин
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
        Vertex(int _name, std::vector<int> &vec): name(_name), edgeTo(vec){

        }
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
};

class ListGraph: public IGraph
{
    std::vector<std::shared_ptr<Vertex>> listVer;
public:
    ListGraph(){};
    ListGraph(IGraph* _oth){
        std::vector<int> allVer;
        _oth->GetVertices(allVer);
        for (auto i : allVer){
            std::vector<int> buff;
            _oth->GetNextVertices(i, buff);
            listVer.push_back(std::make_shared<Vertex>(i, buff));
        }
    };
    void AddEdge(int from, int to) final{
        for (auto const &ver : listVer){
            if (ver->getStart() == from){
                for (auto w : ver->GetEdgeTo()){
                    if (w == to){
                        std::cout << "such an edge already exists\n";
                        return;
                    }
                }
                ver->addEdgeTo(to);
                for (auto const &i : listVer){
                    if (to == i->getStart()){
                        return;
                    }
                }
                listVer.push_back(std::make_shared<Vertex>(to));
                return;
            }
        }
        listVer.push_back(std::make_shared<Vertex>(from, to));
        listVer.push_back(std::make_shared<Vertex>(to));
    }
    std::vector<std::shared_ptr<Vertex>> getVer(){
        return listVer;
    }
    void GetNextVertices(int vertex, std::vector<int> &vertices) const final{
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
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const final{
        for (auto ptr : listVer){
            for (auto ver : ptr->GetEdgeTo()){
                if (ver == vertex){
                    vertices.push_back(ptr->getStart());
                }
            }
        }
        if (vertices.empty()) std::cout << "Vertex " << vertex << " you can't get here\n";
    }
    int VerticesCount() const final{
        return (int)listVer.size();
    }
    void GetVertices(std::vector<int> &vertices) const final{
        for (auto const &i : listVer){
            vertices.push_back(i->getStart());
        }
    }
    ListGraph& operator=(const IGraph& _oth) final{
        if (this == &_oth){
            return *this;
        }
        std::vector<int> allVer;
        _oth.GetVertices(allVer);
        for (auto i : allVer){
            std::vector<int> buff;
            _oth.GetNextVertices(i, buff);
            listVer.push_back(std::make_shared<Vertex>(i, buff));
        }
        return *this;
    }
    ~ListGraph(){};
};

class MatrixGraph: public IGraph
{
    std::vector<int> indexVertex;//по индексу получаем номер вершины
    std::map<int,int> vertexIndex;//по номеру вершины получаем её индекс
    std::vector<std::vector<int>> matrix;//наша мартица
    int index(int vertex) {
        if (auto pos = vertexIndex.find(vertex); pos != vertexIndex.end()){
            return pos->second;
        }
        int size_indexVertex = indexVertex.size();
        indexVertex.push_back(vertex);
        vertexIndex[vertex] = size_indexVertex;
        for (auto& i: matrix) i.push_back(0);
        matrix.emplace_back(size_indexVertex + 1,0);
        return size_indexVertex;
    }
public:
    MatrixGraph(){}
    MatrixGraph(IGraph *_oth) {

    }
    MatrixGraph& operator=(const IGraph& _oth) final{

    }

    void AddEdge(int start, int where) override{
        int from = index(start) , to = index(where);
        matrix[from][to] = 1;
    }

    void GetVertices(std::vector<int> &vertices) const override{

    }
    int VerticesCount() const override{
        return indexVertex.size();
    }
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override{
        int from;
        if (auto i = vertexIndex.find(vertex); i != vertexIndex.end()){
            from = i->second;
        }else {
            std::cout << "vertex non\n";
            return;
        }
        std::vector<int> to;
        for (int t = 0; t < matrix[from].size(); t++){
            if (matrix[from][t] == 1)to.push_back(t);
        }
        for (auto j : to){
            vertices.push_back(indexVertex[j]);
        }
    }
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override{

    }
    ~MatrixGraph() {}
};

int main(){
    MatrixGraph list1;
    list1.AddEdge(1,2);
    list1.AddEdge(1,3);
    list1.AddEdge(100,100);
    std::vector<int> a;
    list1.GetNextVertices(1,a);

    /*
    list1.AddEdge(1,2);
    list1.AddEdge(2,1);
    list1.AddEdge(3,1);
    list1.AddEdge(41,33);
    list1.AddEdge(33,3);
    list1.AddEdge(100,100);
     */
    return 0;
}