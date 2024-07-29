#include <iostream>
using namespace std;
class MyClass {
  public:
  int a=1;
  MyClass(){
    std::clog << "0 "<< 0 << std::endl;
  }
  ~MyClass(){
    std::clog << "1 "<< 0 << std::endl;
  } 
};
int main()
{
  MyClass obj;
  std::clog << "obj.a "<< obj.a << std::endl;
  return 0;
}
