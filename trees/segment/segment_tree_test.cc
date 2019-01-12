#include <string>
#include "segment_tree.h"
using namespace std;

int main() {
	string arr[10] = {"Australia", "England", "West Indies", "England",
	"England", "Australia", "Pakistan", "India", "Sri Lanka", "Sri Lanka"};

	fontus::SegmentTree<string, string*> stree(&arr[0], &arr[10],
		[] (string a, string b) ->string {
			return a + b;
		});
	stree.print();

	int arr1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	fontus::SegmentTree<int, int*> stree1(&arr1[0], &arr1[10],
		[] (int a, int b) ->int {
			return a + b;
		});
	stree1.print();
	stree1.update(10, 4);
}
