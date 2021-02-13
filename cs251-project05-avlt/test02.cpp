/*test.cpp*/

//
// An AVL unit test based on Catch framework
//

#include <iostream>
#include <vector>
#include <algorithm>

#include "avlt.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("(2) Insert test tree")
{
    avlt<int, int>  tree;
	REQUIRE(tree.size() == 0);
	REQUIRE(tree.height() == -1);
	
	vector<int> keys = {2, 9, 10, 0, 17, 22, 30, 1, 8};
	vector<int> heights = {1, 3, 0, 0, 2, 1, 0, 2, 0};

	for (int key : keys) {
		//cout << "inserting: " << key << endl;
		tree.insert(key, 99);
		//tree.dump(cout);
		//int x;
		//cout << "continue? " << endl;
		//cin >> x;
	}
	tree.dump(cout);
	REQUIRE(tree.height() == 3);
	
	for (int i = 0; i < keys.size(); ++i) {
		REQUIRE(tree.operator%(keys[i]) == heights[i]);
	}
	//cout << "height of node 8 is : " << tree.operator%(8) << endl;
	
	
	avlt<int, int>  tree2 = tree;
	tree.clear();
	REQUIRE(tree.size() == 0);
	REQUIRE(tree2.size() == 9);
	REQUIRE(tree2.height() == 3);
	for (int i = 0; i < keys.size(); ++i) {
		REQUIRE(tree2.operator%(keys[i]) == heights[i]);
	}
	tree2.dump(cout);
	
	


}