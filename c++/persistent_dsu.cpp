struct PersistentDSU {
	int n;
	vector<int> p, size, saves;
	vector<pair<int, int>> pHist, sizeHist;
	int numOfComponents;
	PersistentDSU(int n) : n(n), p(n), size(n, 1) {
		for (int i = 0; i < n; i++) p[i] = i;
		numOfComponents = n;
	}

	int get(int a) {
		while (a != p[a]) a = p[a];
		return a;
	}

	void uni(int a, int b) {
		a = get(a), b = get(b);
		if (a != b) {
			if (size[a] > size[b]) swap(a, b);
			pHist.push_back({ a, p[a] });
			sizeHist.push_back({ b, size[b] });
			p[a] = b; size[b] += size[a]; numOfComponents--;
		}
	}

	void save() {
		saves.push_back(pHist.size());
	}

	void rollback() {
		while (pHist.size() > saves.back()) {
			int a = pHist.back().first, b = sizeHist.back().first;
			p[a] = pHist.back().second, size[b] = sizeHist.back().second;
			numOfComponents++;
			pHist.pop_back(), sizeHist.pop_back();
		}
		saves.pop_back();
	}
};
