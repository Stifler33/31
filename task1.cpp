#include <iostream>
#include <memory>
#define INFO

using namespace std;
class Toy
{
private:
    string name;
public:
    Toy(string _name): name(_name){
#ifdef INFO
        cout << "new Toy " << this << endl;
#endif
    }
    Toy():name("ToyName"){
#ifdef INFO
        cout << "new Toy " << this << endl;
#endif
    }
    Toy(const Toy& obj){
#ifdef INFO
        cout << "new Toy " << this << endl;
#endif
            *this = obj;
    }
    string getName(){
        return name;
    }
    void setName(string newName){
        name = newName;
    }
    ~Toy(){
#ifdef INFO
        cout << "delete Toy " << this << endl;
#endif
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
#ifdef INFO
        cout << "added shared_ptr_toy " << this << endl;
        cout << "added ptr toy " << toy_ptr << endl;
        cout << "adde ptr count " << count << endl;
#endif
    }
    //конструктор от объекта Toy
    shared_ptr_toy(Toy* obj){
        toy_ptr = obj;
    }
    //Конструктор от имени игрушки
    shared_ptr_toy(string name)
    {
        count = new (int)(0);
        toy_ptr = new T(name);
#ifdef INFO
        cout << "added shared_ptr_toy " << this << endl;
        cout << "added ptr toy " << toy_ptr << endl;
        cout << "adde ptr count " << count << endl;
#endif
    }
    //конструктор копированием
    shared_ptr_toy(shared_ptr_toy& obj)
    {
        count = obj.count;
        *count += 1;
        toy_ptr = new T(*obj.toy_ptr);
#ifdef INFO
        cout << "copy shared_ptr_toy " << this << endl;
        cout << "copy ptr toy " << toy_ptr << endl;
        cout << "copy ptr count " << count << endl;
#endif
    }
    //перегрузка оператора " = " копированием объекта shared_ptr_toy
    shared_ptr_toy& operator=(const shared_ptr_toy& obj){
        if (this == &obj){
            return *this;
        }
        toy_ptr = new T(*obj.toy_ptr);
        count = obj.count;
        *count += 1;
#ifdef INFO
        cout << "copy shared_ptr_toy " << this << endl;
        cout << "added ptr toy " << toy_ptr << endl;
        cout << "copy ptr count " << count << endl;
#endif
        return *this;
    }
    //перегрузка оператора " = " копированием объекта Toy
    shared_ptr_toy& operator=(Toy* obj){
        if (toy_ptr == obj){
            return *this;
        }
        toy_ptr = obj;
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
#ifdef INFO
        cout << "delete shared_ptr_toy " << this << endl;
#endif
        if (*count == 0){
#ifdef INFO
            cout << "delete ptr count " << count << endl;
#endif
            delete count;
        }else {
#ifdef INFO
            cout << "reduce count " << count << endl;
#endif
            *count -= 1;
        }
#ifdef INFO
        cout << "delete ptr toy " << toy_ptr << endl;
#endif
        delete toy_ptr;
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
    Toy* b = make_shared_toy("max");
    shared_ptr_toy<Toy> x;
    x = b;
    Toy* c = make_shared_toy(a);
    cout << "toy a " << a.getName() << endl;
    cout << "toy b " << b->getName() << endl;
    cout << "toy c " << c->getName() << endl;
    cout << "toy x " << x->getName() << endl;
    delete b;
    delete c;
    return 0;
}
