template<class T>
struct PersistentStack {
    struct MyStack {
        T val;
        MyStack* next;
        MyStack(T val, MyStack* next) : val(val), next(next) {}
        MyStack* push(T x) {
            return new MyStack(x, this);
        }
        pair<MyStack*, T> pop() {
            return { next, val };
        }
    };
    vector<MyStack*> stacks;
    PersistentStack() { 
        stacks.push_back(new MyStack(0, NULL));
    }
    // v is the version, x is the value
    // by default the version 0 contains an node having data 0 and next NULL
    void push(int v, T x) {
        stacks.push_back(stacks[v]->push(x));
    }
    T pop(int v) {
        auto res = stacks[v]->pop();
        stacks.push_back(res.first);
        return res.second;
    }
};