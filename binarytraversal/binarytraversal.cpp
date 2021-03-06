/*
 Binary tree traversal class with in-order, pre-order and post-order traversal methods.
*/
#include <cstddef>  // Needed for NULL definition
#include <iostream>  // Needed for cout
using namespace std;

int depthBelow = 0;

class Node {
private:
	int val;
	Node *pLeft;
	Node *pRight;

public:
	Node(int inVal) {
		val = inVal;
		pLeft=NULL;
		pRight=NULL;
	}

	void insertLeftChild(Node *pChildNode) {
		if (pLeft == NULL) {
			pLeft = pChildNode;
		} else {
			cout << "Left child already exists" << endl;
		}
	}

	void insertRightChild(Node *pChildNode) {
		if (pRight == NULL) {
			pRight = pChildNode;
		} else {
			cout << "Right child already exists" << endl;
		}
	}

	void traverseInOrder() {
		if (pLeft) {
			depthBelow++;
			cout << "traverse left" << endl;
			pLeft->traverseInOrder();
		}

		cout << "process in order, " << "value is " << val << ", depth is " << depthBelow << endl;

		if (pRight) {
			depthBelow++;
			cout << "traverse right" << endl;
			pRight->traverseInOrder();
		}

		depthBelow--;
		return;
	}

	void traversePreOrder() {
		cout << "process pre order, " << "value is " << val << ", depth is " << depthBelow << endl;

		if (pLeft) {
			depthBelow++;
			cout << "traverse left" << endl;
			pLeft->traversePreOrder();
		}

		if (pRight) {
			depthBelow++;
			cout << "traverse right" << endl;
			pRight->traversePreOrder();
		}

		depthBelow--;
		return;
	}

	void traversePostOrder() {
		if (pLeft) {
			depthBelow++;
			cout << "traverse left" << endl;
			pLeft->traversePostOrder();
		}

		if (pRight) {
			depthBelow++;
			cout << "traverse right" << endl;
			pRight->traversePostOrder();
		}

		cout << "process post order, " << "value is " << val << ", depth is " << depthBelow << endl;

		depthBelow--;
		return;
	}
};

/*
	Sample code to test the binary tree traversal class.
*/
int main(int argc, char ** argv) {
	Node n1(12); Node n2(9); Node n3(8); Node n4(11); Node n5(15); Node n6(16); Node n7(17);

	/* Create the following binary tree.
	           12
	          /  \
	        9     15
	       /  \     \
	      8   11    16
	                  \
	                  17
	*/
	n1.insertLeftChild(&n2);
	n2.insertLeftChild(&n3);
	n2.insertRightChild(&n4);
	n1.insertRightChild(&n5);
	n5.insertRightChild(&n6);
	n6.insertRightChild(&n7);
	cout << "Tree populated" << endl;

	cout << " " << endl;
	cout << "in-order traversal" << endl;
	depthBelow = 0;
	n1.traverseInOrder();

	cout << " " << endl;
	cout << "pre-order traversal" << endl;
	depthBelow = 0;
	n1.traversePreOrder();

	cout << " " << endl;
	cout << "post-order traversal" << endl;
	depthBelow = 0;
	n1.traversePostOrder();

	return 0;
}
