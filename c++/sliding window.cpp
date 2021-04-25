vi sw(vi &v, int w) {
	deque<int> dq;
	vi res;
	int start = 0, end = 0;
	while (end != v.size()) {
		while (!dq.empty() && v[dq.back()] >= v[end]) {
			dq.pop_back();
		}
		dq.push_back(end);
		int len = end - start + 1;
		if (len == w) {
			res.push_back(v[dq.front()]);
			start++;
			while (!dq.empty() && dq.front() < start) dq.pop_front();
		}
		end++;
	}
	return res;
}