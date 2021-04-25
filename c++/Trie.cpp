struct TrieNode{
    vi adj, stringsEndingHere;
    TrieNode(){
        adj = vi(26, -1);
    }
};
struct Trie {
    vector<TrieNode> nodes;
    Trie() {
        nodes = vector<TrieNode>(1);
    }
    void insert(string& query, int id) {
        int current = 0;
        for (auto letter : query) {
	    letter -= 'a';
            if (nodes[current].adj[letter] != -1) {
                current = nodes[current].adj[letter];
            }
            else {
                nodes.push_back(TrieNode());
                current = nodes[current].adj[letter] = nodes.size() - 1;
            }
        }
        nodes[current].stringsEndingHere.push_back(id);
    }
};