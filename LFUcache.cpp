/*
    Doubly Linked List Node:
    (key, value) pairs
    use counter
    head, tail pointers
*/
struct Node
{
    int key, val;
    int use;
    Node *prev, *next;
    Node(int x, int y)
    {
        key = x;
        val = y;
        use = 0;
        prev = next = NULL;
    }
};
/*
    LFU Cache:
    Properties: 
    - maxSize -> Max capacity of the cache
    - currSize -> Current cache size
    - map<key,Node*> mp -> To store Node wrt to keys
    - head*  -> Dummy head (-1,-1)
    - tail*  -> Dummy tail (-1,-1)
    Methods:
    + addNodeBack(Node)-> Adds node just before the dummy tail
    + moveBack(Node)   -> Moves the node back by 1 position
    + get(key)         -> Gives associated value (if exists), otherwise -1
    + put(key, value)  -> Assigns a key value pair
*/
class LFUCache {
    int maxSize, currSize;
    unordered_map<int,Node*>mp;
    Node *head, *tail;
public:
    LFUCache(int capacity) {
        maxSize = capacity;
        currSize = 0;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }
    void addNodeBack(Node *x) {
        Node *t = tail->prev;
        t->next = x;
        x->prev = t;
        x->next = tail;
        tail->prev = x;
    }
    void moveBack(Node *x) {
        Node *v = x->prev->prev;
        Node *w = x->prev;
        Node *y = x->next;
        w->next = y; y->prev = w;
        x->next = w; w->prev = x;
        v->next = x; x->prev = v;
    }
    int get(int key) {
        if(mp.find(key)!=mp.end()) {
            Node *x = mp[key];
            int ans = x->val;
            x->use++;
            while(x->prev->key != -1 && x->prev->use <= x->use) {
                moveBack(x);
            }
            return ans;
        }
        return -1;
    }
    
    void put(int key, int value) {
        if(mp.find(key)!=mp.end()) {
            Node *x = mp[key];
            x->val = value;
            x->use++;
            while(x->prev->key != -1 && x->prev->use <= x->use) {
                moveBack(x);
            }
        } else {
            if(currSize == maxSize) {
                mp.erase(tail->prev->key);
                deleteNode(tail->prev);
                currSize--;
            }
            Node *x = new Node(key, value);
            x->use++;
            mp[key] = x;
            addNodeBack(x);
            while(x->prev->key != -1 && x->prev->use <= x->use) {
                moveBack(x);
            }
            currSize++;
        }
    }
};