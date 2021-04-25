namespace Treap{
	struct Node {
		int val, priority, size;
		Node *left, *right;
		Node(int val) : val(val), priority(rng()), size(1), left(nullptr), right(nullptr) {}
	};
	typedef Node *pnode;
	int sz(pnode t) {
		return t ? t->size : 0;
	}
	void upd_sz(pnode t) {
		if (t) t->size = sz(t->left) + 1 + sz(t->right);
	}
	void split(pnode t, pnode &leftRes, pnode &rightRes, int key) {
		if (!t) leftRes = rightRes = nullptr;
		else if (t->val <= key) split(t->right, t->right, rightRes, key), leftRes = t;
		else split(t->left, leftRes, t->left, key), rightRes = t;
		upd_sz(t);
	}
	void merge(pnode &t, pnode l, pnode r) {
		if (!l || !r) t = l ? l : r;
		else if (l->priority > r->priority) merge(l->right, l->right, r), t = l;
		else merge(r->left, l, r->left), t = r;
		upd_sz(t);
	}
	void insert(pnode &t, pnode it) {
		if (!t) t = it;
		else if (it->priority > t->priority) split(t, it->left, it->right, it->val), t = it;
		else insert(t->val <= it->val ? t->right : t->left, it);
		upd_sz(t);
	}
	void erase(pnode &t, int key) {
		if (!t) return;
		else if (t->val == key) {
			pnode temp = t;
			merge(t, t->left, t->right);
			delete temp;
		}
		else erase(t->val < key ? t->right : t->left, key);
		upd_sz(t);
	}
	// return null if not found
	pnode findByRank(pnode t, int k){
		if(!t || sz(t->left) == k - 1) return t;
		if(sz(t->left) >= k) return findByRank(t->left, k);
		else return findByRank(t->right, k - sz(t->left) - 1);
	}
	// number of elements smaller than k
	int findRank(pnode t, int k){
		if(!t) return 0;
		if(t->val >= k) return findRank(t->left, k);
		else return sz(t->left) + 1 + findRank(t->right, k);
	}
	void inorder(pnode t, vi &res){
		if(!t) return;
		inorder(t->left, res);
		res.push_back(t->val);
		inorder(t->right, res);
	}
	vi inorder(pnode t){
		vi res;
		inorder(t, res);
		return res;
	}
};

// usage: 
// Treap::pnode root = nullptr;
// reap::insert(root, new Treap::Node(k));