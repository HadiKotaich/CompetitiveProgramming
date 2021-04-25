struct  State
{
	int curNode;
	int curIndex;
	int numOfAllowedMistakesLeft;

	State(int _curNode = 0, int _curIndex = -1, int _numOfAllowedMistakesLeft = 0) {
		curNode = _curNode;
		curIndex = _curIndex;
		numOfAllowedMistakesLeft = _numOfAllowedMistakesLeft;
	}
};
struct Trie 
{

	vector<vector<int>> Adj;
	vector<vector<string>> StringsEndingHere;

	Trie() {
		AddNode();
	}

	void AddNode() {
		Adj.push_back(vector<int>(3, 0));
		StringsEndingHere.push_back(vector<string>());
	}

	void Init(vector<string> strings) {
		for (auto it : strings) {
			Insert(it);
		}
	}

	void Insert(string query) {
		int current = 0;
		for (auto letter : query) {
			if (Adj[current][letter - 'a'] != 0) {
				current = Adj[current][letter - 'a'];
			}
			else {
				AddNode();
				current = Adj[current][letter - 'a'] = Adj.size() - 1;
			}
		}
		StringsEndingHere[current].push_back(query);
	}

	bool FuzzySearch(string query, int numOfMistakes) {
		stack<State> stck;
		stck.push(State(0, 0, numOfMistakes));
		while (!stck.empty()) {
			int curNode = stck.top().curNode;
			int curIndex = stck.top().curIndex;
			int numOfAllowedMistakesLeft = stck.top().numOfAllowedMistakesLeft;

			stck.pop();
			if (curIndex == query.size()) {
				if (StringsEndingHere[curNode].size() > 0 && numOfAllowedMistakesLeft == 0) {
					return true;
				}
				else {
					continue;
				}
			}

			if (numOfAllowedMistakesLeft > 0) {
				for (int i = 0; i < 3; i++) {
					if (i == query[curIndex] - 'a' && Adj[curNode][i] != 0) {
						stck.push(State(Adj[curNode][i], curIndex + 1, numOfAllowedMistakesLeft));
					}
					else if (Adj[curNode][i] != 0){
						stck.push(State(Adj[curNode][i], curIndex + 1, numOfAllowedMistakesLeft - 1));
					}
				}
			}
			else {
				for (int i = 0; i < 3; i++) {
					if (i == query[curIndex] - 'a' && Adj[curNode][i] != 0) {
						stck.push(State(Adj[curNode][i], curIndex + 1, numOfAllowedMistakesLeft));
					}
				}
			}
		}
		return false;
	}
};

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);

    vector<string> ListOfStrings;

	Trie myTrie = Trie();
	myTrie.Init(ListOfStrings);

    
}