#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#include <iostream>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool checkPL(Node* node, int depth, int& leaf_depth) {

	if(!node) {
		return true;
	}

	if(!node->left && !node->right) {
		if(leaf_depth == -1) {
			leaf_depth = depth;
		}

		return depth == leaf_depth;
	}

	return checkPL(node->left, depth + 1, leaf_depth) &&
					checkPL(node->right, depth + 1, leaf_depth);
}

bool equalPaths(Node * root)
{
    // Add your code below

	int leaf_depth = -1;

	return checkPL(root, 0, leaf_depth);
}

