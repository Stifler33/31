#include <iostream>
#include <vector>
#include <memory>
#include <map>
class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() {};
    IGraph(IGraph &_oth) {};

    IGraph& operator=(const IGraph& _oth) {};
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
    void copy(IGraph& _oth) {
        std::vector<int> vertex;
        _oth.GetVertices(vertex);
        for (auto& i : vertex){
            nexVer[i] = std::vector<int>(0);
            prevVer[i] = std::vector<int>(0);
            auto nextItr = nexVer.find(i);
            auto prevItr = prevVer.find(i);
            _oth.GetNextVertices(i, nextItr->second);
            _oth.GetPrevVertices(i, prevItr->second);
        }
    }
public:
    ListGraph(){};
    ListGraph(IGraph& _oth){
        if (this == &_oth) return;
        copy(_oth);
    }
    void AddEdge(int from, int to) override{
        if (auto obj = nexVer.find(from); obj != nexVer.end()){
            for(auto i : obj->second){
                if (i == to) return;
            }
            nexVer[from].push_back(to);
            if (nexVer.count(to) == 0) nexVer[to] = std::vector<int>(0);
            //return;
        }else {
            nexVer[from].push_back(to);
            if (nexVer.count(to) == 0) nexVer[to] = std::vector<int>(0);
            //return;
        }
        //добавляем вершины в обратный список
        if (auto prev = prevVer.find(to); prev != prevVer.end()){
            for (auto i : prev->second){
                if (i == from) return;
            }
            prevVer[to].push_back(from);
            if (prevVer.count(from) == 0) prevVer[from] = std::vector<int>(0);
        }else{
            prevVer[to].push_back(from);
            if (prevVer.count(from) == 0) prevVer[from] = std::vector<int>(0);
        }
    }
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override{
        if (auto ver = nexVer.find(vertex); ver != nexVer.end()){
            for (auto num : ver->second){
                vertices.push_back(num);
            }
            return;
        }else return;
    }
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override{
        if (auto prev = prevVer.find(vertex); prev != prevVer.end()){
            for (auto i : prev->second){
                vertices.push_back(i);
            }
        }
    }
    int VerticesCount() const override{
        return nexVer.size();
    }
    void GetVertices(std::vector<int> &vertices) const override{
        for (auto& vertex : nexVer){
            vertices.push_back(vertex.first);
        }
    }
    ListGraph& operator=(IGraph& _oth) {
        if (this == &_oth) return *this;
        copy(_oth);
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
    void copy(IGraph &_oth){
        std::map<int, std::vector<int>> mapVer;
        std::vector<int> numVer;
        _oth.GetVertices(numVer);
        for (auto i : numVer){
            mapVer[i] = std::vector<int>(0);
            auto itr = mapVer.find(i);
            _oth.GetNextVertices(i, itr->second);
        }
        for (auto itr : mapVer){
            for (auto i : itr.second){
                AddEdge(itr.first, i);
            }
        }
    }
public:
    MatrixGraph(){}
    MatrixGraph(IGraph &_oth) {
        if (this == &_oth) return;
        copy(_oth);
    }
    MatrixGraph& operator=(IGraph& _oth) {
        if (this == &_oth) return *this;
        copy(_oth);
        return *this;
    }

    void AddEdge(int start, int where) override{
        int from = index(start) , to = index(where);
        matrix[from][to] = 1;
    }

    void GetVertices(std::vector<int> &vertices) const override{
        for (auto& vertex : vertexIndex){
            vertices.push_back(vertex.first);
        }
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
    list1.AddEdge(3,1);
    list1.AddEdge(41,33);
    list1.AddEdge(33,3);
    list1.AddEdge(33,1);
    list1.AddEdge(41,1);
    list1.AddEdge(100,100);
    list1.AddEdge(1,100);
    MatrixGraph list2;
    list2 = list1;
    ListGraph list3(list2);
    std::string user;
    while (user != "q"){
        int numVer;
        std::cin >> user;
        if (user == "q") continue;
        numVer = stoi(user);
        std::vector<int> a;
        list3.GetNextVertices(numVer, a);
        for (auto i : a){
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}