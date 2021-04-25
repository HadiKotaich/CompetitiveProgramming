bitset<31>L, R;
ll dp[32][2][2][2][2];
ll calc(int cur, bool firstEqualToL, bool firstEqualToR, bool secondEqualToL, bool secondEqualToR) {
	if (cur < 0) return 1;
 
	ll ans = dp[cur][firstEqualToL][firstEqualToR][secondEqualToL][secondEqualToR];
	
	if (ans != -1) return ans;
	else ans = 0;
 
	// put 0 0 
	bool canPut00 = !(L[cur] && (firstEqualToL || secondEqualToL));
	if (canPut00) {
		ans += calc(cur - 1, firstEqualToL, firstEqualToR && !R[cur], secondEqualToL, secondEqualToR && !R[cur]);
	}
 
	// put 0 1
	bool canPut01 = !((L[cur] && firstEqualToL) || (!R[cur] && secondEqualToR));
	if (canPut01) {
		ans += calc(cur - 1, firstEqualToL, firstEqualToR && !R[cur], secondEqualToL && L[cur], secondEqualToR);
	}
	
	// put 1 0
	bool canPut10 = !((!R[cur] && firstEqualToR) || (L[cur] && secondEqualToL));
	if (canPut10) {
		ans += calc(cur - 1, firstEqualToL && L[cur], firstEqualToR, secondEqualToL, secondEqualToR && !R[cur]);
	}
 
	return dp[cur][firstEqualToL][firstEqualToR][secondEqualToL][secondEqualToR] = ans;
}
 
 
int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
 
	
	int t;
	cin >> t;
	while (t--) {
		memset(dp, -1, sizeof dp);
		ll l, r;
		cin >> l >> r;
 
		L = bitset<31>(l);
		R = bitset<31>(r);
 
		ll ans = calc(30, true, true, true, true);
		cout << ans << "\n";
	}
	return 0;
}