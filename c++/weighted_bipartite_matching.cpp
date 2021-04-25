// you can use min cost max flow, it has decent speed

// you can also use this, very small and very fast, 
// this matches perfectly all rows in mat, mat doesnt have to be square matrix
// it will try to match all rows perfectly, put infinity if no edge
// returns the min cost weighted bipartite matching
// no need to re initialize anything other than the mat, everything gets reinitialized

long long dist[100]; int via[100],chk[100];
int f[100][100]; long long c[100][100];
long long hungarian(vvl &mat){
	int n = mat.size(); if (n == 0) return 0;
	int m = mat[0].size(); if (n > m) return 1e16;
	int v = n + m + 2;
	for (int i=0;i<v;i++) for (int j=0;j<v;j++) f[i][j] = c[i][j] = 0;
	for (int i=0;i<n;i++) f[0][i+1] = 1;
	for (int i=0;i<m;i++) f[n+i+1][n+m+1] = 1;
	for (int i=0;i<n;i++) for (int j=0;j<m;j++){
		f[i+1][n+j+1] = 1;
		c[i+1][n+j+1] = mat[i][j];
		c[n+j+1][i+1] = -mat[i][j];
	}
	long long r = 0;
	for (int k=0;k<n;k++){
		for (int i=0;i<v;i++) chk[i] = 0, dist[i] = 1e16;
		queue<int> Q; Q.push(0); chk[0] = 1; dist[0] = 0; via[0] = -1;
		while (!Q.empty()){
			int x = Q.front(); Q.pop(); chk[x] = 0;
			for (int i=0;i<v;i++) if (f[x][i] == 1 && dist[i] > dist[x] + c[x][i]){
				if (!chk[i]){Q.push(i); chk[i] = 1;}
				dist[i] = dist[x] + c[x][i]; via[i] = x;
			}
		}
		int p = n + m + 1; r += dist[p];
		while (1){
			int q = via[p];
			if (q == -1) break;
			f[q][p]--; f[p][q]++;
			p = q;
		}
	}
	return r;
}