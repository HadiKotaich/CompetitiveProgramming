int SA[(int)1e6 + 10], T[(int)1e6 + 10], rnk[(int)1e6 + 10], lcp[(int)1e6 + 10];// SA[rnk[i]] = i
const int LOGN = 21, N = 1 << LOGN;
int rm[LOGN][N];
int LeadingZeros[(int)(1e6) + 10];

inline bool leq(int a1, int a2, int b1, int b2) // lexicographic order
{
	return(a1 < b1 || a1 == b1 && a2 <= b2);
} // for pairs
inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3) {
	return(a1 < b1 || a1 == b1 && leq(a2, a3, b2, b3));
} // and triples
static void radixPass(int* a, int* b, int* r, int n, int K) {
	// count occurrences
	int* c = new int[K + 1]; // counter array
	for (int i = 0; i <= K; i++) c[i] = 0; // reset counters
	for (int i = 0; i < n; i++) c[r[a[i]]]++; // count occurrences
	for (int i = 0, sum = 0; i <= K; i++) // exclusive prefix sums
	{
		int t = c[i]; c[i] = sum; sum += t;
	}
	for (int i = 0; i < n; i++) b[c[r[a[i]]]++] = a[i]; // sort
	delete[] c;
}

//only if not GNU
void LZ(int n) {
	int Lzeros = 31;
	int j = 1;
	for (int i = 1; i <= n; i = (i << 1)) {
		while (j < (i << 1) && j <= n) {
			LeadingZeros[j] = Lzeros;
			j++;
		}
		Lzeros--;
	}
}

// K is alphabet size

void suffixArray(int* T, int* SA, int n, int K) {
	if(n == 1) {
		SA[0] = 0;
		return;
	}
	T[n] = T[n + 1] = T[n + 2] = 0;
	int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
	int* R = new int[n02 + 3]; R[n02] = R[n02 + 1] = R[n02 + 2] = 0;
	int* SA12 = new int[n02 + 3]; SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;
	int* R0 = new int[n0];
	int* SA0 = new int[n0];

	for (int i = 0, j = 0; i < n + (n0 - n1); i++) if (i % 3 != 0) R[j++] = i;

	radixPass(R, SA12, T + 2, n02, K);
	radixPass(SA12, R, T + 1, n02, K);
	radixPass(R, SA12, T, n02, K);

	int name = 0, c0 = -1, c1 = -1, c2 = -1;
	for (int i = 0; i < n02; i++) {
		if (T[SA12[i]] != c0 || T[SA12[i] + 1] != c1 || T[SA12[i] + 2] != c2)
		{
			name++; c0 = T[SA12[i]]; c1 = T[SA12[i] + 1]; c2 = T[SA12[i] + 2];
		}
		if (SA12[i] % 3 == 1) { R[SA12[i] / 3] = name; } // write to R1
		else { R[SA12[i] / 3 + n0] = name; } // write to R2
	}

	if (name < n02) {
		suffixArray(R, SA12, n02, name);
		for (int i = 0; i < n02; i++) R[SA12[i]] = i + 1;
	}
	else
		for (int i = 0; i < n02; i++) SA12[R[i] - 1] = i;

	for (int i = 0, j = 0; i < n02; i++) if (SA12[i] < n0) R0[j++] = 3 * SA12[i];
	radixPass(R0, SA0, T, n0, K);

	for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
		int i = GetI();
		int j = SA0[p];
		if (SA12[t] < n0 ?
			leq(T[i], R[SA12[t] + n0], T[j], R[j / 3]) :
			leq(T[i], T[i + 1], R[SA12[t] - n0 + 1], T[j], T[j + 1], R[j / 3 + n0]))
		{
			SA[k] = i; t++;
			if (t == n02)
				for (k++; p < n0; p++, k++) SA[k] = SA0[p];
		}
		else {
			SA[k] = j; p++;
			if (p == n0)
				for (k++; t < n02; t++, k++) SA[k] = GetI();
		}
	}
	delete[] R; delete[] SA12; delete[] SA0; delete[] R0;
}
void GetLcp(int n) {
	for (int i = 0; i < n; i++) rnk[SA[i]] = i;
	for (int i = 0, k = 0; i < n; i++) {
		if (k > 0) k--;
		if (rnk[i] == n - 1) { lcp[n - 1] = -1; k = 0; continue; }
		int j = SA[rnk[i] + 1];
		while (T[i + k] == T[j + k]) k++;
		lcp[rnk[i]] = k;
	}
}

// if this is for the suffix array: initialization: init(s); init(lcp, SZ(s));
void init(int* tab, int n) { // tab[0..n-1]
	LZ(n);
	for (int i = 0; i < n; i++) rm[0][i] = tab[i];
	for (int k = 1; k < LOGN; k++)
		for (int i = 0; i <= n - (1 << k); i++)
			rm[k][i] = std::min(rm[k - 1][i],
				rm[k - 1][i + (1 << (k - 1))]);
}
int query(int a, int b) {
	int k = 31 - LeadingZeros[b - a + 1];//__builtin_clz(b - a + 1) if working on linux
	return std::min(rm[k][a], rm[k][b - (1 << k) + 1]);
}
int lcpBetween(int a, int b) { // assume: a != b, a and b indices T // a and b indices in the original string
	a = rnk[a], b = rnk[b];
	if (a > b) std::swap(a, b);
	return query(a, b - 1);
}

//init suffix array
void InitSuffixArray(string s) {
	set<char> st;
	for (auto it : s) {
		st.insert(it);
	}

	map<char, int> charRank;
	int cur = 1;
	for (auto it : st) {
		charRank[it] = cur++;
	}

	for (int i = 0; i < s.size(); i++) {
		T[i] = charRank[s[i]];
	}

	int sizeAlphabet = st.size() + 1;
	suffixArray(T, SA, s.size(), sizeAlphabet);
	GetLcp(s.size());
	init(lcp, s.size());

}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);

	string s;
	cin >> s;

	InitSuffixArray(s);
}

