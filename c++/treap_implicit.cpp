namespace Treap{ // implicit, 1 based
	struct Node {
		int val, priority, size;
		bool rev;
		Node *left, *right;
		Node(int val) : val(val), priority(rng()), size(1), rev(false), left(nullptr), right(nullptr) {}
	};
	typedef Node *pnode;
	int sz(pnode t) {
		return t ? t->size : 0;
	}
	void update(pnode t) {
		if (t) {
			t->size = sz(t->left) + 1 + sz(t->right);
		}
	}
	void push (pnode it) {
	    if (it && it->rev) {
	        it->rev = false;
	        swap (it->left, it->right);
	        if (it->left)  it->left->rev ^= true;
	        if (it->right)  it->right->rev ^= true;
	    }
	}

	void split(pnode t, pnode &leftRes, pnode &rightRes, int pos, int add = 0) {
		if (!t) return void(leftRes = rightRes = nullptr);
		push(t);
		int curPos = add + sz(t->left) + 1;
		if (curPos <= pos) split(t->right, t->right, rightRes, pos, curPos), leftRes = t;
		else split(t->left, leftRes, t->left, pos, add), rightRes = t;
		update(t);
	}

	void merge(pnode &t, pnode l, pnode r) {
		push(l), push(r);
		if (!l || !r) t = l ? l : r;
		else if (l->priority > r->priority) merge(l->right, l->right, r), t = l;
		else merge(r->left, l, r->left), t = r;
		update(t);
	}

	void insert(pnode &t, int pos, int val) {
		pnode leftRes = nullptr, rightRes = nullptr;
		split(t, leftRes, rightRes, pos - 1);
		merge(t, leftRes, new Node(val));
		merge(t, t, rightRes);
	}

	void erase(pnode &t, int pos){
		pnode leftRes = nullptr, rightRes = nullptr, elem = nullptr;
		split(t, leftRes, rightRes, pos - 1);
		split(rightRes, elem, rightRes, 1);
		merge(t, leftRes, rightRes);
		delete elem;
	}

	void update(pnode &t, int pos, int val){
		pnode leftRes = nullptr, rightRes = nullptr, elem = nullptr;
		split(t, leftRes, rightRes, pos - 1);
		split(rightRes, elem, rightRes, 1);
		elem->val = val;
		merge(t, leftRes, elem);
		merge(t, t, rightRes);
	}

	void reverse (pnode t, int l, int r) {
	    pnode t1, t2, t3;
	    split (t, t1, t2, l - 1);
	    split (t2, t2, t3, r-l+1);
	    t2->rev ^= true;
	    merge (t, t1, t2);
	    merge (t, t, t3);
	}
	void inorder(pnode t, vi &res){
		if(!t) return;
		push(t);
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