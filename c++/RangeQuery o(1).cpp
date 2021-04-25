template<class T>
struct rangeQuery{
	vector<vector<T>> val;
	int n;
	rangeQuery(vector<T> &a) {
		n = a.size();
		int LOGN = 1;
		while ((1 << LOGN) < n) LOGN++;
		val.resize(LOGN, vector<T>(n, 0));
		
		for (int i = 0; i < n; i++) val[0][i] = a[i];
		for (int cur_pow = 1; cur_pow < LOGN; cur_pow++) {
			int dist = 1 << (cur_pow - 1);
			for (int i = 0; i + 2 * dist < n; i++) {
				val[cur_pow][i] = combine(val[cur_pow - 1][i], val[cur_pow - 1][i + dist]);
			}
		}
	}

	T query(int l, int r) {
		int len = r - l + 1;
		int cur_pow = 0;
		while (len > 1) {
			len /= 2;
			cur_pow++;
		}

		T leftVal = val[cur_pow][l];
		T rightVal = val [cur_pow][r - (1 << cur_pow) + 1];
		T ans = combine(leftVal, rightVal);
		return ans;
	}

	T combine(T& x, T& y) {
		return min(x, y);
	}
};