struct IntervalSet{
    set<pll> st;
    Intervals(){}
    void insert(pll p){
        auto it = st.lower_bound({p.first, -1});
        while(it != st.end()){
            if(it->first > p.second) break;
            p.second = max(it->second, p.second);
            it = st.erase(it);
        }
        if(it != st.begin()){
            --it;
            if(it->second >= p.first){
                p.first = it->first;
                p.second = max(p.second, it->second);
                st.erase(it);
            }
        }
        st.insert(p);
    }
};