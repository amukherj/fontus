#include "pairing_heap.h"
#include <iostream>
#include <random>
#include <string>
using namespace std;

class RandomInt {
public:
	RandomInt(int min, int max) : dist(min, max) {
		rng.seed(std::random_device()());
	}

	int next() {
		return dist(rng);
	}

private:
	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist;
};

int main() {
	fontus::PairingHeap<int> heap(10);
	fontus::PairingHeap<int> heap1(20);
	heap.merge(heap1);

	heap.push(30);
	cout << heap.top() << '\n';
	heap.print(cout);

	heap.push(5);
	cout << heap.top() << '\n';
	heap.print(cout);

	cout << "Start popping\n";

	auto n = heap.pop();
	cout << n << " popped\n";
	heap.print(cout);

	n = heap.pop();
	cout << n << " popped\n";
	heap.print(cout);

	n = heap.pop();
	cout << n << " popped\n";
	heap.print(cout);

	n = heap.pop();
	cout << n << " popped\n";
	heap.print(cout);

	cout << "Heap ops\n";
	RandomInt r(0, 200);
	fontus::PairingHeap<int> heep(r.next());
	for (int i = 0; i < 20; ++i) {
		heep.push(r.next());
	}
	cout << "Pushed elems\n";

	for (int i = 0; i < 21; ++i) {
		cout << "-> " << heep.pop() << '\n';
	}

	/* fontus::PairingHeap<int> heap2(9);
	heap2.push(100);
	heap2.push(76);
	heap2.push(3);
	heap2.push(122);
	heap2.push(89);
	heap2.push(196);
	heap2.push(4);
	heap2.push(59);
	heap2.push(114);
	heap2.push(35);

	heap2.print(cout);
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n";
	cout << heap2.pop() << '\n';
	cout << "-------\n"; */

	fontus::PairingHeap<std::string> heeps("hello");
	heeps.push("hola");
	heeps.push("servus");
	heeps.push("boker tov");
	heeps.push("hallo");
	heeps.push("nihao");
	heeps.push("bongiorno");

	for (int i = 0; i < 7; ++i) {
		cout << heeps.pop() << '\n';
	}
}
