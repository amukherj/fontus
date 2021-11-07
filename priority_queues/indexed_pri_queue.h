#ifndef INDEXED_PRI_QUEUE
#define INDEXED_PRI_QUEUE

#include <bits/stdc++.h>

namespace fontus {

template <typename T, typename U = int>
class IndexedPriorityQueue {
public:
	IndexedPriorityQueue(int initial_size) {
		id_to_key.reserve(initial_size);
		heap.reserve(initial_size);
		id_to_pos.reserve(initial_size);
		pos_to_id.reserve(initial_size);
	}

	bool push(const T& key, U value) {
		if (key.empty() || !value) {
			return false;
		}

		int key_id = 0;
		if (id_pool.begin() == id_pool.end()) {
			key_id = heap.size();
		} else {
			key_id = *id_pool.begin();
			id_pool.erase(id_pool.begin());
		}
		key_to_id.insert({key, key_id});
		if (key_id >= id_to_key.size()) {
			// This happens when we have not handed this key_id out earlier
			id_to_key.push_back(key);
			assert(id_to_key.size() == key_id + 1);
		} else {
			// This happens when we reuse a key_id from the pool
			id_to_key[key_id] = key;
		}
		heap.push_back(value);
		id_to_pos[key_id] = heap.size() - 1;
		pos_to_id[heap.size() - 1] = key_id;

		for (int i = heap.size() - 1; i > 0; i /= 2) {
			if (heap[i] <= heap[i/2]) {
				break;
			}
			heap_swap(i, i/2);
		}
		return true;
	}

	std::optional<std::pair<T, U>> pop() {
		if (heap.empty()) {
			return std::nullopt;
		}
		return remove(0);
	}

	std::optional<std::pair<T, U>> remove(const T& key) {
		if (heap.empty()) {
			return std::nullopt;
		}
		auto iter = key_to_id.find(key);
		if (iter == key_to_id.end()) {
			return std::nullopt;
		}

		int key_id = iter->second;
		return remove(id_to_pos[key_id]);
	}

	std::optional<U> query_priority(const T& key) const {
		auto iter = key_to_id.find(key);
		if (iter == key_to_id.end()) {
			return std::nullopt;
		}

		int key_id = iter->second;
		assert(id_to_pos[key_id] < heap.size());
		return heap[id_to_pos[key_id]];
	}

	void update_priority(const T& key, U value);

private:
	// map keys to an incremental key id
	std::unordered_map<T, int> key_to_id;

	// reverse map the key ids to keys
	std::vector<T> id_to_key;

	// actual heap data structure
	std::vector<U> heap;

	// map key ids to position in heap
	std::vector<int> id_to_pos;

	// map position in heap to key ids
	std::vector<int> pos_to_id;

	// Spare key ids for popped elements
	// for reuse
	std::set<int> id_pool;

	// Swap two elements in the heap at positions i and j.
	void heap_swap(int i, int j) noexcept {
		std::swap(heap[i], heap[j]);
		int key_i = pos_to_id[i];
		int key_j = pos_to_id[j];
		pos_to_id[i] = key_j;
		pos_to_id[j] = key_i;
		id_to_pos[key_j] = i;
		id_to_pos[key_i] = j;
	}

	// Remove the element at position pos in the heap.
	std::optional<std::pair<T, U>> remove(int pos) {
		int priority = heap[pos];
		int key_id = pos_to_id[pos];

		heap[pos] = heap.back();
		heap.pop_back();
		pos_to_id[pos] = pos_to_id[heap.size()];
		id_to_pos[pos_to_id[pos]] = pos;

		for (int i = pos; 2*i + 1 < heap.size(); ) {
			if (heap[i] >= heap[2*i + 1] && (heap.size() <= 2*i + 2
				|| heap[i] >= heap[2*i + 2])) {
				break;
			}
			int j = 2*i + 2;
			if (j >= heap.size() || heap[j-1] > heap[j]) {
				j = j-1;
			}
			heap_swap(i, j);
			i = j;
		}

		T key = id_to_key[key_id];
		id_pool.insert(key_id);
		id_to_key[key_id] = "";
		key_to_id.erase(key);

		return std::make_pair(key, priority);
	}
};

} // namespace fontus

#endif /* INDEXED_PRI_QUEUE */
