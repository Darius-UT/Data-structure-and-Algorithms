#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main()
{
    list<int> List;
    List.push_back(1);
    List.push_back(2);
    List.push_back(3);
    List.push_back(4);
    List.push_back(5);
    List.push_back(6);

    for (auto i = List.begin(); i != List.end(); i++) {
        cout << *i << " "; 
    }
    cout << endl;

    auto it = List.begin();
    advance(it, 2);
    List.splice(List.end(), List, it);

    for (auto i = List.begin(); i != List.end(); i++) {
        cout << *i << " "; 
    }
    cout << endl;

}