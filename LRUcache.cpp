/*
    Doubly Linked List Node:
    (key, value) pairs
    head, tail pointers
*/
struct Node
{
    int key,val;
    Node *next, *prev;
    Node(int x,int y)
    {
        key = x;
        val = y;
        next = prev = NULL;
    }
};
/*
    LRU Cache:
    Properties: 
    - maxSize -> Max capacity of the cache
    - currSize -> Current cache size
    - map<key,Node*> mp -> To store Node wrt to keys
    - head*  -> Dummy head (-1,-1)
    - tail*  -> Dummy tail (-1,-1)
    Methods:
    + addNode(Node)    -> Adds node to the beginning (after the dummy head)
    + deleteNode(Node) -> Removes node from the doubly linked list
    + get(key)         -> Gives associated value (if exists), otherwise -1
    + put(key, value)  -> Assigns a key value pair
*/
class LRUCache {
    int maxSize;
    int currSize;
    unordered_map<int,Node*>mp;
    Node *head, *tail;
public:
    // Constructor -> initializes capacity, currSize, assigns dummy node to head, tail, head and tail pointers
    LRUCache(int capacity) {
        maxSize = capacity;
        currSize = 0;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    // addNode(Node) -> Adds node to the beginning (after the dummy head)
    void addNode(Node *x)
    {
        Node *t = head->next;
        x->prev = head;
        x->next = t;
        head->next = x;
        t->prev = x;
    }

    // deleteNode(Node) -> Removes node from the doubly linked list
    void deleteNode(Node *x) // Deletes node
    {
        Node *p = x->prev;
        Node *q = x->next;
        q->prev = p;
        p->next = q;
    }

    // get(key) -> Gives associated value (if exists), otherwise -1
    int get(int key) {
        if(mp.find(key)!=mp.end()) {
            Node *x = mp[key];
            int ans = x->val;
            deleteNode(x);
            addNode(x);
            return ans;
        }
        return -1;
    }
    
    // put(key, value) -> Assigns a key value pair
    void put(int key, int value) {
        if(mp.find(key)!=mp.end()) {
            Node *x = mp[key];
            x->val = value;
            deleteNode(x);
            addNode(x);
        } else {
            if(currSize == maxSize) {
                // cout<<tail->prev->val<<endl;
                mp.erase(tail->prev->key);
                deleteNode(tail->prev);
                currSize--;
            }
            Node *x = new Node(key, value);
            addNode(x);
            mp[key] = x;
            currSize++;
        }
    }
};