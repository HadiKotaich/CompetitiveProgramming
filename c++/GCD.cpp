long long gcd(long long a, long long b) {
	long long r;
	while (b != 0) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

//helper for gcdBetween
void gcd_help() {
	for (int i = 0; i < n; i++) gcdVal[i][0] = a[i];
	int cur_pow = 1;
	while ((1<<(cur_pow-1)) <= n) {
		int dist = 1 << (cur_pow-1);
		for (int i = 0; i+2*dist <= n; i++) {
			gcdVal[i][cur_pow] = gcd(gcdVal[i][cur_pow-1],gcdVal[i+dist][cur_pow-1]);
		}
		cur_pow++;
	}
}
//gets gcd of a segment in o(log(n)), can be faster if we memoize all the queries [i][j] for i,j<=1000
int gcdBetween(int start, int end) {
	int len = end - start + 1;
	int cur_pow = 0;
	while (len > 1) {
		len /= 2;
		cur_pow++;
	}
	
	int left = gcdVal[start][cur_pow];
	int right = gcdVal[end-(1<<cur_pow)+1][cur_pow];
	int ans = gcd(left,right);
	return ans;
}