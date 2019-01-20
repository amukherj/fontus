#include <vector>
#include <memory>
#include <algorithm>
#include <ostream>
#include <iostream>
using namespace std;

namespace fontus {

template <typename T>
struct PairingHeapNode {
	typedef std::unique_ptr<PairingHeapNode<T>> Ptr;

	T value;
	std::vector<Ptr> sub_heaps;

	PairingHeapNode(T value) : value{value} {}
	PairingHeapNode(PairingHeapNode&&) = default;
	PairingHeapNode& operator=(PairingHeapNode&&) = default;

	void print(std::ostream& ostr) const {
		ostr << value << '\n';
		for (auto& node: sub_heaps) {
			node->print(ostr);
		}
	}
};

template <typename T, typename Cmp = greater<T>>
class PairingHeap {
public:
	PairingHeap() : root{}, size{0} {}

	PairingHeap(T value) : root(make_unique<PairingHeapNode<T>>(value)),
		size{0} {}

	PairingHeap(PairingHeap&&) = default;
	PairingHeap& operator=(PairingHeap&&) = default;

	std::ostream& print(std::ostream& ostr) const {
		ostr << "<<<<\n";
		if (root) {
			root->print(ostr);
		}
		ostr << ">>>>\n";
		return ostr;
	}

	T top() const {
		return root->value;
	}

	bool empty() const {
		return !root;
	}

	void merge(PairingHeap<T, Cmp>& that) {
		if (empty() && !that.empty()) {
			root = std::move(that.root);
		} else if (!empty() && !that.empty()) {
			if (Cmp()(root->value, that.root->value)) {
				root->sub_heaps.push_back(std::move(that.root));
			} else {
				that.root->sub_heaps.push_back(std::move(root));
				root = std::move(that.root);
			}
		}
	}

	void push(T value) {
		PairingHeap<T, Cmp> new_heap(value);
		this->merge(new_heap);
		++size;
	}

	T pop() {
		if (!root) {
			throw std::runtime_error("Underflow error");
		}

		T result = root->value;
		root = merge_pairs(root->sub_heaps);
		--size;

		return result;
	}

private:
	typedef typename PairingHeapNode<T>::Ptr PtrType;
	PtrType root;
	size_t size;

	PairingHeap(PtrType node) : root(move(node)) {}

	PtrType merge_pairs(std::vector<PtrType>& heaps) {
		int sz = heaps.size();
		bool merge_done = (sz <= 1);

		while (!merge_done) {
			int j = 0;

			for (int i = 0; i < sz; i += 2) {
				if (i == sz - 1) {
					if (j < i) {
						heaps[j++] = std::move(heaps[i]);
					} else {
						++j;
					}
					continue;
				}

				if (Cmp()(heaps[i]->value, heaps[i+1]->value)) {
					heaps[i]->sub_heaps.push_back(std::move(heaps[i+1]));
					if (j < i) {
						heaps[j++] = std::move(heaps[i]);
					} else {
						++j;
					}
				} else {
					heaps[i+1]->sub_heaps.push_back(std::move(heaps[i]));
					heaps[j++] = std::move(heaps[i+1]);
				}
			}
			sz = j;
			merge_done = (sz <= 1); // one elem remaining
		}

		if (sz > 0) {
			heaps.erase(heaps.begin() + sz, heaps.end());
			return std::move(heaps[0]);
		} else {
			heaps.erase(heaps.begin(), heaps.end());
			return PtrType{};
		}
	}
};

} // namespace fontus

