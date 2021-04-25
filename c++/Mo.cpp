const int block_size = 350;

struct Query {
	int l, r, k, ind;
	bool operator<(Query other) const{
		return make_pair(l / block_size, r) <
				make_pair(other.l / block_size, other.r);
	}
};

int freq[100001];
int cnt[100001];

struct MO{
	vector<Query> queries;
	vector<int> res;
    // TODO: initialize data structure
	vi a;
	int ans;

	MO(int n) : a(n), ans(0) {}

	void push(int l, int r, int ind){
		queries.push_back({l, r, ind});
	}

	void solve() {
	    res = vector<int>(queries.size());
	    sort(queries.begin(), queries.end());
	    int cur_l = 0;
	    int cur_r = -1;
	    // invariant: data structure will always reflect the range [cur_l, cur_r]
	    for (Query q : queries) {
	        while (cur_l > q.l) {
	            cur_l--;
	            add(cur_l);
	        }
	        while (cur_r < q.r) {
	            cur_r++;
	            add(cur_r);
	        }
	        while (cur_l < q.l) {
	            remove(cur_l);
	            cur_l++;
	        }
	        while (cur_r > q.r) {
	            remove(cur_r);
	            cur_r--;
	        }
	        res[q.ind] = get_answer();
	    }
	}

	void remove(int ind){
		int elem = a[ind];
		cnt[freq[elem]]--;
		if(cnt[freq[elem]] == 0 && freq[elem] == ans){
			ans = freq[elem] - 1;
		}
		--freq[elem];
		cnt[freq[elem]]++;
	}

	void add(int ind){
		int elem = a[ind];
		cnt[freq[elem]]--;
		++freq[elem];
		cnt[freq[elem]]++;
		if(freq[elem] > ans){
			ans = freq[elem];
		}
	}

	int get_answer(){
		return ans;
	}
};

// coordinate compression
void process(vi &a){
	map<int, int> m;
	int cur = 0;
	for(auto &it: a) m[it];
	for(auto &it: m) it.second = cur++;
	for(auto &it: a) it = m[it];
}