#include<iostream>
#include<algorithm>

namespace persistentcount {
	long long  NEXT_FREE_INDEX = 1;
	long long  s[1000000],L[1000000], R[1000000];
	//L and R initially should be zero
	//s[0] should initially be something neutral like 0 for sum, INT_MAX for min ...
	//first start with a current_root 0, then when you do your first update update this current root to be 1
	// in other words your id is always 0 untill you make the first update then  id will be 1
	long long  n = 1 << 30;

	long long upd(long long  p, long long  v, long long  id, long long  l = 0, long long  r = n) {
		int ID = id;
		if (ID == 0)ID = NEXT_FREE_INDEX++;
		s[ID] = s[id] + v;
		if (r - l < 2)
			return ID;
		long long  mid = (l + r) / 2;
		if (p < mid)
			L[ID]=upd(p, v, L[ID], l, mid);
		else
			R[ID]=upd(p, v, R[ID], mid, r);
		return ID;
	}
	long long  sum(long long  x, long long  y, long long  id, long long  l = 0, long long  r = n) {
		if (x >= r || l > y)return 0;
		if (x <= l && (r - 1) <= y)	return s[id];
		long long  mid = (l + r) / 2;
		return sum(x, y, L[id], l, mid) + sum(x, y, R[id], mid, r);
	}
}
//be careful of the size of the array always the bug is there

int main() {
	int Q;
	persistentcount::s[0] = 0;
	int cur_root = 0;
	for (int i = 0; i < Q; i++) {
		int type;
		int x;
		std::cin >> x;
		if (type == 1) {
			persistentcount::upd(x - 1, 1, cur_root);
			cur_root = 1;//here we did an update so 
		}
		else {
			if (persistentcount::sum(x - 1, x - 1, cur_root) > 0)
				persistentcount::upd(x - 1, -1, cur_root);
		}
	}
	//system("pause");
}

