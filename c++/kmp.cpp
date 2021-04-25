// returns 0-based indices of matches
// kn[i] = longest proper prefix in p[0...i] which is also a suffix of p[0...i]
vi kmp(string t, string p) { // text, pattern
	vi kn(SZ(p)+1,-1); // Knuth prefix function
	FORI(i,SZ(p)) {
		int j = kn[i-1];
		while (j != -1 && p[j] != p[i-1]) j=kn[j];
		kn[i] = j+1;
	}
	// kn is computed
	vi ans;
	int ppos=0,tpos=0;
	while (tpos<SZ(t)) {
		while (ppos!=-1 && (ppos == SZ(p) || p[ppos]!=t[tpos])) ppos=kn[ppos];
		ppos++; tpos++;
		if (ppos==SZ(p)) ans.pb(tpos-SZ(p));
	}
	return ans;
}