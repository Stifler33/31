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
};

class ListGraph: public IGraph
{
    std::map<int, std::vector<int>> nexVer;
    std::map<int, std::vector<int>> prevVer;
public:
    ListGraph(){};
    ListGraph(IGraph* _oth){

    };
    void AddEdge(int from, int to) override{
        if (auto obj = nexVer.find(from); obj != nexVer.end()){
            for(auto i : obj->second){
                if (i == to) return;
            }
            nexVer[from].push_back(to);
            if (nexVer.count(to) == 0) nexVer[to] = std::vector<int>(0);
            return;
        }else {
            nexVer[from].push_back(to);
            if (nexVer.count(to) == 0) nexVer[to] = std::vector<int>(0);
            return;
        }
    }
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override{

    }
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override{

    }
    int VerticesCount() const override{
        return nexVer.size();
    }
    void GetVertices(std::vector<int> &vertices) const override{

    }
    ListGraph& operator=(const IGraph& _oth) override{

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
            if (matrix[from][t] == 1)vertices.push_back(indexVertex[t]);
        }
    }
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override{
        int indexVer;
        if (auto i = vertexIndex.find(vertex); i != vertexIndex.end()){
            indexVer = i->second;
        }else {
            std::cout << "vertex non\n";
            return;
        }
        for (int j = 0; j < matrix.size(); j++){
            if (matrix[j][indexVer] == 1){
                vertices.push_back(indexVertex[j]);
            }
        }
    }
    ~MatrixGraph() {}
};

int main(){
    ListGraph list1;
    list1.AddEdge(1,2);
    list1.AddEdge(1,2);
    list1.AddEdge(3,1);
    list1.AddEdge(41,33);
    list1.AddEdge(33,3);
    list1.AddEdge(100,100);
    list1.AddEdge(1,100);
    std::cout << list1.VerticesCount();
    return 0;
    std::vector<int> a;
    std::vector<int> b;
    list1.GetPrevVertices(1,a);
    list1.GetNextVertices(1, b);
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