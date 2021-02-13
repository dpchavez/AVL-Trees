//
// Lab week 08
// 
// Interactive program for calling AVL insert.  Let's you insert nodes and 
// watch nodes rotate...
//

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "avlt.h"

using namespace std;

int main()
{
  avlt<int, int>  avl;
  int  x;

  cout << "Enter a key to insert into tree (0 to stop)> ";
  cin >> x;

  while (x > 0)
  {
    avl.insert(x, x);
    cout << "Size: " << avl.size() << endl;
    cout << "Height: " << avl.height() << endl;
    avl.dump(cout);

    cout << endl;
    cout << "Enter a key to insert into tree (0 to stop)> ";
    cin >> x;
  }

  //
  // done:
  //
  return 0;
}
