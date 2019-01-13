#ifndef FONTUS_SEGMENT_TREE_H
#define FONTUS_SEGMENT_TREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

int fold_right(int x) {
	auto max = 8 * sizeof(x);
	int prevx = 0;

	for (int i = 1; i < max && x != prevx; i *= 2) {
		prevx = x;
		x |= (x >> i);
	}

	return x;
}

int next_power_2(int x) {
	return fold_right(x) + 1;
}

int last_power_2(int x) {
	return fold_right(x >> 1) + 1;
}

namespace fontus {

template <typename T>
struct Segment {
	T value;
	int min;
	int max;

	Segment(T value) : value(value) {}
	Segment(T value, int min, int max) : value(value), min(min), max(max) {}
};

template <typename T, typename Iter>
class SegmentTree {
public:
	SegmentTree(Iter start, Iter end, std::function<T(T,T)> func, T identity = T{})
		: identity(std::move(identity)), size(end - start), nelems(0), func(func) {
		if (size > 0) {
			nelems = next_power_2(size);
		}
		tree.reserve(2*nelems - 1);
		tree.resize(2*nelems - 1, Segment<T>{this->identity});
		populate(start, end);
	}

	T query(int left, int right) {
		return query(0, left, right);
	}

	void update(const T& val, int index) {
		return update(0, val, index);
	}

	void print() {
		for (auto& e: tree) {
			std::cout << e.value << '[' << e.min << ','
				  << e.max << ']' << '\n';
		}
	}

private:
	const T identity;
	std::vector<Segment<T>> tree;
	int size;
	int nelems;
	std::function<T(T,T)> func;

	void populate(Iter start, Iter end) {
		int i = nelems - 1;
		for (Iter it = start; it != end; ++it) {
			tree[i] = Segment<T>{*it, int(it - start),
				int(it - start)};
			++i;
		}

		int index = end - start;
		while (i < tree.size()) {
			tree[i++] = Segment<T>{this->identity, index, index};
			++index;
		}

		for (int i = nelems - 2; i >= 0; --i) {
			tree[i].value = func(tree[2*i + 1].value,
				tree[2*i + 2].value);
			tree[i].min = tree[2*i + 1].min;
			tree[i].max = tree[2*i + 2].max;
		}
	}

	T query(int base, int left, int right) {
		auto min = tree[base].min, max = tree[base].max;

		if (left < min || right > max) {
			throw std::runtime_error("Out of range");
		}

		if (left == right && min == left && max == right) {
			return tree[base].value;
		}

		int mid = min + (max - min) / 2;
		if (right <= mid) {
			return query(2*base + 1, left, right);
		} else if (left > mid) {
			return query(2*base + 2, left, right);
		} else {
			return func(query(2*base + 1, left, mid),
				query(2*base + 2, mid+1, right));
		}
	}

	void update(int base, const T& val, int index) {
		if (base < 0 || base >= tree.size()) {
			throw std::runtime_error("Update base out of range");
		}

		if (index < 0 || index >= size) {
			throw std::runtime_error("Update index out of range");
		}

		int i = nelems - 1 + index;
		tree[i].value = val;
		while (i > 0) {
			i = i / 2;
			tree[i].value = func(tree[2*i + 1].value,
						tree[2*i + 2].value);
		}
	}
};

} // namespace fontus

#endif /* FONTUS_SEGMENT_TREE_H */
