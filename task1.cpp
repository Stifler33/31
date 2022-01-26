#include <iostream>
#include <memory>

using namespace std;
class Toy
{
private:
    string name;
public:
    Toy(string _name): name(_name){
    }
    Toy():name("ToyName"){
    }
    Toy(const Toy& obj){
        *this = obj;
    }
    string getName(){
        return name;
    }
    void setName(string newName){
        name = newName;
    }
    ~Toy(){
    }
};

template<class T>
class shared_ptr_toy
{
private:
    T* toy_ptr = nullptr;
    int* count = nullptr;
public:
    //конструктор по умолчанию
    shared_ptr_toy()
    {
        count = new (int)(0);
        toy_ptr = new T;
    }
    //Конструктор от имени игрушки
    shared_ptr_toy(string name)
    {
        count = new (int)(0);
        toy_ptr = new T(name);
    }
    //конструктор копированием
    shared_ptr_toy(shared_ptr_toy& obj)
    {
        count = obj.count;
        *count += 1;
        toy_ptr = obj.toy_ptr;
    }
    //перегрузка оператора " = " копированием объекта shared_ptr_toy
    shared_ptr_toy& operator=(const shared_ptr_toy& obj){
        if (this == &obj){
            return *this;
        }
        if(*count != 0){
            *count -= 1;
        }else if (*count == 0){
            delete count;
        }
        toy_ptr = obj.toy_ptr;
        count = obj.count;
        *count += 1;
        return *this;
    }
    //перегрузка оператора " -> " для прямого доступа к указателю toy_ptr
    T* operator-> (){
        return toy_ptr;
    }
    void listCount(){
        cout << *count << endl;
    }
    //деструктор
    ~shared_ptr_toy(){
        if (*count == 0){
            delete count;
            delete toy_ptr;
        }else {
            *count -= 1;
        }
    }
};

Toy* make_shared_toy(Toy& obj){
    return new Toy(obj);
}

Toy* make_shared_toy(string _name){
    Toy* out = new Toy(_name);
    return out;
}

int main() {
    Toy a("ball");
    shared_ptr_toy<Toy> x("gaf");
    shared_ptr_toy<Toy> g;
    shared_ptr_toy<Toy> kj;
    shared_ptr_toy<Toy> h;
    shared_ptr_toy<Toy> j;
    shared_ptr_toy<Toy> k("gloo");

    return 0;
}
