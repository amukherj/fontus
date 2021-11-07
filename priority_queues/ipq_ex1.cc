#include "indexed_pri_queue.h"

int main() {
	fontus::IndexedPriorityQueue<std::string> ipq(10);
	ipq.push("hello", 1);
	ipq.push("welcome", 2);
	ipq.push("good morning", 3);
	ipq.push("hola", 4);
	ipq.push("bongiorno", 5);
	ipq.push("what's up", 6);
	ipq.push("take care", 7);

	std::cout << "Priority for good morning is "
		      << ipq.query_priority("good morning").value_or(0) << '\n';
	std::cout << "Priority for bongiorno is "
		      << ipq.query_priority("bongiorno").value_or(0) << '\n';
	std::cout << "Priority for what's up is "
		      << ipq.query_priority("what's up").value_or(0) << '\n';
	std::cout << "Priority for welcome is "
		      << ipq.query_priority("welcome").value_or(0) << '\n';
	std::cout << "Priority for hello is "
		      << ipq.query_priority("hello").value_or(0) << '\n';
	std::cout << "Priority for hola is "
		      << ipq.query_priority("hola").value_or(0) << '\n';
	std::cout << "Priority for take care is "
		      << ipq.query_priority("take care").value_or(0) << '\n';

	auto rem = ipq.remove("bongiorno");
	std::cout << std::get<0>(rem.value_or(
				std::make_pair(std::string("[]"), 0))) << " removed\n";

	for (auto val = ipq.pop(); val; val = ipq.pop()) {
		std::cout << std::get<0>(val.value_or(
					std::make_pair(std::string("[]"), 0))) << " popped\n";
	}
}
