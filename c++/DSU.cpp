struct UnionFind {
	vll tab, cnt;   
	
	UnionFind(int n) : // creates UF on set 0, 1, ..., n-1      
	tab(n), cnt(n,1) {
		for(int i = 0; i < n; i++)
			tab[i] = i;   
	}

	int find(int a) { 
		if (tab[a] == a) return a;      
		return tab[a] = find(tab[a]); 
	}   

	bool unia(int a, int b) { // true if there really was a union      
		a = find(a), b = find(b);      
		if (a==b) return 0;      
		if (cnt[a] > cnt[b]) swap(a,b);      
		cnt[b] += cnt[a];      
		tab[a] = b;      
		return 1;   
	} 

	ll ComponentSize(int a) {
		return cnt[find(a)];
	}
}; 
