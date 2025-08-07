#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int distance (const vector<vector<int>>& code, int n, int k){
	if (n < 2) return 0;
	int minDist = k;
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			int d = 0;
			for (int b = 0; b < k; ++b) {
				if (code[i][b] != code[j][b])
					++d;
			}
			minDist = min(minDist, d);
		}
	}
	return minDist;
}
int main() {
	vector<vector<int>> code = {
		{0,1,1,0,1,0},
		{1,1,0,0,0,0},
		{0,0,0,1,1,1}
	};
	int n = code.size();
	int k = code[0].size();
	cout << "Minimal Hamming destination = "
		<< distance(code, n, k) << "\n";
	return 0;
}