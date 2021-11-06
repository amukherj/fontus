#include "avltree.h"

int main() {
	AVLTree<std::string> tree;
	tree.insert(std::string("hello"));
	tree.traverse();
	tree.remove(std::string("hello1"));
	tree.insert(std::string("ibis"));
	tree.insert(std::string("cheetah"));
	tree.insert(std::string("cat"));
	tree.insert(std::string("dog"));
	tree.insert(std::string("horse"));
	tree.insert(std::string("jaguar"));
	tree.traverse();
	tree.remove(std::string("ibis"));
	tree.traverse();
	tree.insert("ibex");
	tree.insert("kangaroo");
	tree.insert("lemur");
	tree.traverse();
}
