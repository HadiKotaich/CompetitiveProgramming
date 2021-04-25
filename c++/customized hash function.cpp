struct poss {
	int start, end;
	vector<int> scoreVector;
	vector<int> ans;

	bool operator==(const poss &other) const {
		for (int i = 0; i < scoreVector.size(); i++) {
			if (scoreVector[i] != other.scoreVector[i]) return false;
		}
		return true;
	}
};

namespace std {
	template <>
	struct hash<poss> {
		std::size_t operator()(const poss& k) const {
			using std::size_t;
			using std::hash;
			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:
			size_t ret = 0;
			for (auto it : k.scoreVector) {
				ret = (ret ^ hash<int>()(it)) << 1;
			}
			return ret;
		}
	};
}
unordered_map<poss, ll> leftPoss;
