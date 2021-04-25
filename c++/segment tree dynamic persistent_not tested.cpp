namespace persistentcount {
	long long  NEXT_FREE_INDEX = 1;
	long long  s[10000000], L[10000000], R[10000000];
	//L and R initially should be zero
	//s[0] should initially be something neutral like 0 for sum, INT_MAX for min ...
	long long  root[100005];
	long long  n = 1 << 30;

	long long  upd(long long  p, long long  v, long long  id, long long  l = 0, long long  r = n) {

		long long  ID = NEXT_FREE_INDEX++; // index of the node in new version of segment tree
		s[ID] = s[id] + v;
		if (r - l < 2)
			return ID;

		long long  mid = (l + r) / 2;
		L[ID] = L[id], R[ID] = R[id]; // in case of not updating the long long erval of left child or right child
		if (p < mid)
			L[ID] = upd(p, v, L[ID], l, mid);
		else
			R[ID] = upd(p, v, R[ID], mid, r);
		return ID;

	}
	long long  sum(long long  x, long long  y, long long  id, long long  l = 0, long long  r = n) {
		if (x >= r || l > y)return 0;
		if (x <= l && (r-1)<=y)	return s[id];
		long long  mid = (l + r) / 2;
		return sum(x, y, L[id], l, mid)+sum(x, y, R[id], mid, r);
	}
}

/* update in case we are querying on the max not the sum
long long  ID = NEXT_FREE_INDEX++; // index of the node in new version of segment tree
		if (r - l < 2) {
			s[ID] = v;
			return ID;
		}
		long long  mid = (l + r) / 2;
		L[ID] = L[id], R[ID] = R[id]; // in case of not updating the long long erval of left child or right child
		if (p < mid)
			L[ID] = upd(p, v, L[ID], l, mid);
		else
			R[ID] = upd(p, v, R[ID], mid, r);
		s[ID] = std::max(s[L[ID]], s[R[ID]]);
		return ID;
*/