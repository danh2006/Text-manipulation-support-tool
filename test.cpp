#include <bits/stdc++.h>
using namespace std;
#include"macro.h"
#include"test.h"
#include"io.h"

template<class T>
auto testAdd(string& s, T c){
   auto x = AddElement(s, specialChar, c);
   assert((int)x.size() >= 0);
   return x;
}

void testInit(){
    vector<Info> a(2);
    a[0].repeatString.push_back("<<");
    a[0].repeatTimes.push_back(1);
    a[0].step = {2};
    a[1].ignore.push_back("html");

    print(a, "\n");
    swap(a[0], a[1]);
    print(a, "\n");
    a[0].reset();
    a[1].reset();
    print(a, "\n");
}

auto testBracket(){ // [1 -> 8]
   int ptr = -1;
   string bracketStr;
   while(getline(cin, bracketStr)){
       auto element = Bracket(bracketStr, ptr);
       cout << element;
    }
}
//ignore is same with bracket

auto testRepeat(){ 
   string repeatStr;
   while(getline(cin, repeatStr)){
       auto [repeatString, repeatTimes] = Repeat(repeatStr);
       print(repeatString,repeatTimes);
    }
}
auto testStep(){ 
   string stepStr;
   while(getline(cin, stepStr)){
       auto element = Step(stepStr);
       print(element);
    }
}

auto testUpdate(){ //[11 -> 20]
   Info a;
   string UpdateStr;
   while(getline(cin, UpdateStr)){
       if(UpdateStr == "END") break;
       a.update(UpdateStr);
       print(a, "\n");
    }
}

auto testErase(){
   testUpdate();
   Info a;
   string EraseStr;
   while(getline(cin, EraseStr)){
      a.erase(EraseStr);
      print(a, "\n");
   }
}

int main() {
   // testBracket();
//    testRepeat();
   // testStep();
   // testInit();
   // testUpdate();
}
