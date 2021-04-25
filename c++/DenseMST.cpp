// O(n^2) for dense graphs instead of n^2 logn
// check: https://cp-algorithms.com/graph/mst_prim.html#toc-tgt-5
struct DenseMST
{
    int n;
    vvl adj; // adjacency matrix of graph
    static const ll INF = 1e15; // weight INF means there is no edge

    DenseMST(int n) : n(n), adj(n, vll(n, INF))
    {
    }

    void addEdge(int u, int v, ll w)
    {
        adj[u][v] = adj[v][u] = w;
    }

    struct Edge
    {
        ll w = INF, to = -1;
    };

    ll prim()
    {
        ll total_weight = 0;
        vector<bool> selected(n, false);
        vector<Edge> min_e(n);
        min_e[0].w = 0;

        for (int i = 0; i < n; ++i)
        {
            int v = -1;
            for (int j = 0; j < n; ++j)
            {
                if (!selected[j] && (v == -1 || min_e[j].w < min_e[v].w))
                    v = j;
            }


            selected[v] = true;

            if (min_e[v].w != INF)
            {
                total_weight += min_e[v].w;
            }

            for (int to = 0; to < n; ++to)
            {
                if (adj[v][to] < min_e[to].w)
                    min_e[to] = { adj[v][to], v };
            }
        }
        return total_weight;
    }
};