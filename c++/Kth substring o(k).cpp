string s; cin >> s;
int k; cin >> k;
ll n = s.size();

ll upperBound = n * (n + 1);
upperBound /= 2;

if (k > upperBound) cout << "No such line.\n";
else {
	string ans;
	map<int, ll> cnt;
	map<int, vi> occ;
	for (int i = 0; i < s.size(); i++) {
		cnt[s[i] - 'a'] += (n - i);
		occ[s[i] - 'a'].push_back(i);
	}
	while (k > 0) {
		int cur = -1;
		for (auto it : cnt) {
			if (it.second < k) 
				k -= it.second;
			else { 
				k -= (int)(occ[it.first].size());
				cur = it.first; 
				break; 
			};
		}

		ans.push_back('a' + cur);
		vi curVec = occ[cur];

		cnt.clear();
		occ.clear();

		for (auto it : curVec) {
			if (it != n) {
				cnt[s[it + 1] - 'a'] += (n - it - 1);
				occ[s[it + 1] - 'a'].push_back(it + 1);
			}
		}
	}
	cout << ans << "\n";
}