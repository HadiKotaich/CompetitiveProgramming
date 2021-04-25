ll dp[65][65];
void hannoi(vvi &pegs, int n, int from, int to, ofstream &ofs){
	if(n == 0) return;
	else if(n == 1){
		assert(SZ(pegs[from]));
		int curDisk = pegs[from].back();
		ofs<<"move "<<curDisk<<" from "<<from<<" to "<<to;
		if(SZ(pegs[to])) {
			int targetDisk = pegs[to].back();
			assert(targetDisk > curDisk);
			ofs<<" atop "<<targetDisk;
		}
		ofs<<"\n";
		pegs[to].push_back(curDisk);
		pegs[from].pop_back();
	}
	else{
		int curDisk = pegs[from].back(), m = 1;
		for(int i = 1; i < SZ(pegs); i++){
			if(!SZ(pegs[i]) || pegs[i].back() > curDisk) m++;
		}
 
		int k = -1;
		for(int i = 1; i < n; i++){
			if(dp[n][m] == dp[i][m]  * 2 + dp[n - i][m - 1])
				k = i;
		}
		assert(k != -1);
 
		int inter1 = -1;
		for(int i = 1; i < SZ(pegs) && inter1 == -1; i++){
			if((!SZ(pegs[i]) || pegs[i].back() > curDisk) && i != to) inter1 = i;
		}
		assert(inter1 != -1);
 
		hannoi(pegs, k, from, inter1, ofs);
		hannoi(pegs, n - k, from, to, ofs);
		hannoi(pegs, k, inter1, to, ofs);
	}
}
 
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
 
	ifstream cin("hanoi.in");
	ofstream cout("hanoi.out");
	int n, m; cin>>n>>m;
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			dp[i][j] = (i == 1 && j != 1 ? 1 : 1e9);
			for(int k = 1; k < i; k++){
				dp[i][j] = min(dp[i][j], dp[k][j]  * 2 + dp[i - k][j - 1]);
			}
		}
	}
	cout<<dp[n][m]<<"\n";
	vvi pegs(m + 1);
	for(int i = n; i >= 1; i--) pegs[1].push_back(i);
	hannoi(pegs, n, 1, m, cout);
	assert(SZ(pegs[m]) == n);
}