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

	AVLTree<std::string> tree1;
	tree1.insert("kangaroo");
	tree1.insert("ibex");
	tree1.insert("lion");
	tree1.insert("horse");
	tree1.insert("leopard");
	tree1.insert("monkey");
	tree1.insert("lynx");
	tree1.insert("ocelot");
	tree1.remove("ibex");
	tree1.traverse();
	assert(tree1.find("kangaroo"));
	assert(tree1.find("lion"));
	assert(tree1.find("horse"));
	assert(tree1.find("leopard"));
	assert(tree1.find("monkey"));
	assert(tree1.find("lynx"));
	assert(tree1.find("ocelot"));
	assert(!tree1.find("ibex"));
}
