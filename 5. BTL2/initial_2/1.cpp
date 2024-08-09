#include "main.h"

static int MAXSIZE      = 0;
static int countDelete  = 0;


/////////////////////////////////////////////	CLASS DECLARATION + IMPLEMENTATION	//////////////////////////////////////////////////////////////////////////////////
class Hash 
{
public:
    struct Node {
        int data;
        Node *left;
        Node *right;
        Node() : data(0), left(nullptr), right(nullptr) {}
        Node(int a) : data(a), left(nullptr), right(nullptr) {}
    };

    struct areaFields {
        queue<int> order;
        Node *root;
        areaFields() : root(nullptr) {}
        areaFields(Node *r) : root(r) {}
    };

public:
    vector<areaFields> listArea;
public:
    Hash    () {

    }

    ~Hash   () {
        for (areaFields &i : listArea) {
            clear(i.root);
            while (!i.order.empty()) {
                i.order.pop();
            }
        }
        listArea.clear();
    }
    
    void pseudo_printBST        (Node *root) {
        if (root == nullptr) {
            return;
        }
        else {
            cout << root->data << " ";
            pseudo_printBST(root->left);
            pseudo_printBST(root->right);
        }
    }

    void pseudo_printHash       () {
        int count = 0;
        if (listArea.empty()) {
            cout << "Empty Hash";
        }
        else {
            for (areaFields &i : listArea) {
                cout << "Area " << count++ << " : ";
                pseudo_printBST(i.root);
                cout << endl;
            }
        }
        cout << endl << endl;
    }

    void pseudo_printQueueOrder (const int &id) {
        queue<int> tmp;
        while (!listArea[id].order.empty()) {
            cout << listArea[id].order.front() << " ";
            tmp.push(listArea[id].order.front());
            listArea[id].order.pop();
        }
        while (!tmp.empty()) {
            listArea[id].order.push(tmp.front());
            tmp.pop();
        }
        cout << endl;
    }

    void recursiveInOrder       (Node *root) {
        if (root == nullptr) {
            return;
        }
        else {
            recursiveInOrder(root->left);
            cout << root->data << endl;
            recursiveInOrder(root->right);
        }
    }

    void printBST_LIMITLESS     (const int &id) {
        int tmp = listArea.size();
        if (id < tmp) {
            recursiveInOrder(listArea[id].root);
        }
    }

    void recursiveInsert        (Node *&root, const int &value) {
        if (root == nullptr) {
            root = new Node(value); countDelete++;
        }
        else if (value >= root->data) {
            return recursiveInsert(root->right, value);
        }
        else {
            return recursiveInsert(root->left, value);
        }
    }

    void insertCustomer         (const int &id, const int &result) {
        recursiveInsert(listArea[id].root, result);
        listArea[id].order.push(result);
    }

    void removeAt               (Node *&root) {
        Node *deletedNode = root;
        if (root->left == nullptr && root->right == nullptr) {
            root = nullptr;
        }
        else if (root->left == nullptr) {
            root = root->right;
        }
        else if (root->right == nullptr) {
            root = root->left;
        }
        else {
            Node *parentDeletedNode = root;
            deletedNode = root->right;
            if (deletedNode == nullptr) {

            }
            while (deletedNode->left != nullptr) {
                parentDeletedNode = deletedNode;
                deletedNode = deletedNode->left;
            }
            root->data = deletedNode->data;
            if (parentDeletedNode == root) {
                parentDeletedNode->right = deletedNode->right;
            }
            else {
                parentDeletedNode->left = deletedNode->right;
            }
            parentDeletedNode = nullptr;
        }
        delete deletedNode; deletedNode = nullptr; countDelete--;
    }

    void recursiveRemove        (Node *&root, const int &value) {
        if (root == nullptr) {
            cout << "Loi recursiveRemove" << endl;
        }
        else if (value > root->data) {
            recursiveRemove(root->right, value);
        }
        else if (value < root->data) {
            recursiveRemove(root->left, value);
        }
        else {
            removeAt(root);
        }
    }

    void removeCustomerAt       (const int &id, int Y) {
        int tmp = listArea[id].order.size();
        if (Y > tmp) {
            Y = tmp;
        }
        int value;
        for (int i = 0; i < Y; i++) {
            value = listArea[id].order.front(); 
            listArea[id].order.pop();
            recursiveRemove(listArea[id].root, value);
        }
    }

    friend int totalNodes       (Node *root) {
        if (root == nullptr) {
            return 0;
        }
        else {
            int left = totalNodes(root->left);
            int right = totalNodes(root->right);
            return left + right + 1;
        }
    }

    void clear                  (Node *&root) {
        if (root == nullptr) {
            return;
        }
        else {
            clear(root->left);
            clear(root->right);
            delete root; root = nullptr; countDelete--;
        }
    }
};

class Heap
{
public:
    struct areaFields
    {
        int id;
        list<int> listCustomer;
        areaFields(int i, int r) : id(i) {
            listCustomer.push_back(r);
        }
    };

public:
    vector<areaFields> listArea;
    list<int> listIdOrder;

public:
    Heap()
    {
        
    }

    ~Heap()
    {
        for (areaFields &i : listArea) {
            i.listCustomer.clear();
        }
        listArea.clear();

        listIdOrder.clear();
    }

    void pseudoPrintList(const list<int> &List)
    {
        for (int i : List)
        {
            cout << i << " ";
        }
        cout << endl;
    }

    void pseudoPrintHeap()
    {
        int tmp = listArea.size();
        for (int i = 0; i < tmp; i++) {
            cout << "Area " << listArea[i].id << " : ";
            pseudoPrintList(listArea[i].listCustomer);
        }
        cout << endl;
    }

    void printCustomer_LIFO(const int &index, int num)
    {
        int tmp = listArea[index].listCustomer.size();
        if (num > tmp)
        {
            num = tmp;
        }

        for (list<int>::reverse_iterator tmp = listArea[index].listCustomer.rbegin(); tmp != listArea[index].listCustomer.rend() && num > 0; tmp++, num--)
        {
            cout << listArea[index].id << "-" << *tmp << endl;
        }
    }

    void printArea_preOrder_CLEAVE(int index, const int &num)
    {
        int tmp = listArea.size();
        if (index < 0 || index >= tmp)
        {
            return;
        }
        else
        {
            printCustomer_LIFO(index, num);
            printArea_preOrder_CLEAVE(2 * index + 1, num);
            printArea_preOrder_CLEAVE(2 * index + 2, num);
        }
    }

    friend bool isSmallerArea (vector<areaFields> &listArea, list<int> &listIdOrder, int position1, int position2) {
        if (listArea[position1].listCustomer.size() < listArea[position2].listCustomer.size()) {
            return true;
        }
        else if (listArea[position1].listCustomer.size() == listArea[position2].listCustomer.size()) {
            for (list<int>::iterator it = listIdOrder.begin(); it != listIdOrder.end(); it++) {
                if (*it == listArea[position1].id) {
                    return true;
                }
                else if (*it == listArea[position2].id) {
                    return false;
                }
            }
            return false;
        }
        else {
            return false;
        }
    }

    void reHeapUp(int position)
    {
        if (position == 0) {
            return;
        }
        else {
            int parent = (position - 1)/2;
            if (isSmallerArea(listArea, listIdOrder, position, parent)) {
                swap(listArea[parent], listArea[position]);
                reHeapUp(parent);
            }
        }
    }

    void reHeapDown(int position)
    {
        int smallest = position;
        int left = position * 2 + 1;
        int right = position * 2 + 2;

        int listSize = listArea.size();
        if (left < listSize && isSmallerArea(listArea, listIdOrder, left, smallest)) {
            smallest = left;
        }
        if (right < listSize && isSmallerArea(listArea, listIdOrder, right, smallest)) {
            smallest = right;
        }

        if (smallest != position) {
            swap(listArea[smallest], listArea[position]);
            reHeapDown(smallest);
        }
    }

    friend int getIndex_inListArea(vector<areaFields> &listArea, int ID)
    {
        int tmp = listArea.size();
        for (int i = 0; i < tmp; i++) {
            if (listArea[i].id == ID) {
                return i;
            }
        }
        return -1;
    }

    void update_listIdOrder (int id) 
    {
        for (list<int>::iterator it = listIdOrder.begin(); it != listIdOrder.end(); it++) {
            if (*it == id) {
                listIdOrder.splice(listIdOrder.end(), listIdOrder, it);
            }
        }
    }   

    void insertCustomer(const int &id, const int &RESULT)
    {
        int index = getIndex_inListArea(listArea, id);
        if (index == -1) {
            areaFields newArea(id, RESULT);
            listArea.push_back(newArea);
            listIdOrder.push_back(id);
            reHeapUp(listArea.size() - 1);
        }
        else {
            listArea[index].listCustomer.push_back(RESULT);
            update_listIdOrder(id);
            reHeapDown(index);
        }
    }

    void removeArea(int indexToRemove)
    {
        if (indexToRemove == -1)
        {
            return;
        }
        else
        {
            swap(listArea[indexToRemove], listArea[listArea.size() - 1]);
            listArea.erase(listArea.end() - 1);
            
            reHeapDown(indexToRemove);
            reHeapUp(indexToRemove);
        }
    }

    void removeArea_listIdOrder (int id) 
    {
        for (list<int>::iterator it = listIdOrder.begin(); it != listIdOrder.end(); it++) {
            if (*it == id) {
                listIdOrder.erase(it);
                return;
            }
        }
    }

    void printRemovedCustomer_Keiteiken(const int &result, const int &id)
    {
        cout << result << "-" << id << endl;
    }

    void remove_NumCus_inArea (int id, int num) 
    {
        int id_index = getIndex_inListArea(listArea, id);
        while (num--) {
            listArea[id_index].listCustomer.pop_front();
            if (listArea[id_index].listCustomer.size() == 0) {
                removeArea(id_index);
                removeArea_listIdOrder(id);
                return;
            }
            else {
                update_listIdOrder(id);
                int tmp = listArea.size();
                if (id_index < tmp) {
                    reHeapUp(id_index);
                }
            }
        }
    }
};


//.............................. 	FOR HUFFMAN-TREE	............................................//
class HuffNode
{
    public:
        virtual int getWeight() = 0;
        virtual int getHeight() = 0;
        virtual int getPriority() = 0;
        virtual bool isLeaf() = 0;
        virtual ~HuffNode() {}
};

class LeafNode      : public HuffNode
{
    private:
        char ch;
        int wgt;
        int hgt;
        int prio;
    public:
        LeafNode    () {}
        LeafNode    (char c, int w, int h, int p) {
            this->ch = c;
            this->wgt = w;
            this->hgt = h;
            this->prio = p;
        }
        ~LeafNode   () {
            ch = '\0';
            wgt = 0;
            hgt = 0;
            prio = 0;
        }
        
        char getChar    () {
            return this->ch;
        }
        int getWeight   () override {
            return this->wgt;
        }
        int getHeight   () override {
            return this->hgt;
        }
        int getPriority () override {
            return this->prio;
        }
        bool isLeaf     () override {
            return true;
        }
};

class InternalNode  : public HuffNode
{
    private:
        int wgt;
        int hgt;
        int prio;
        HuffNode *leftNode;
        HuffNode *rightNode;
    public:
        InternalNode    () {}
        InternalNode    (HuffNode *a, HuffNode *b, int &numPrio) {
            this->wgt = a->getWeight() + b->getWeight();
            this->hgt = max(a->getHeight(), b->getHeight()) + 1;
            this->prio = numPrio++;
            this->leftNode = a;
            this->rightNode = b;
        }
        ~InternalNode   () {
            wgt = 0;
        }
        int getWeight   () override {
            return this->wgt;
        }
        int getHeight   () override {
            return this->hgt;
        }
        int getPriority () override {
            return this->prio;
        }
        bool isLeaf     () override {
            return false;
        }
        HuffNode*& getLeftNode  () {
            return this->leftNode;
        }
        HuffNode*& getRightNode () {
            return this->rightNode;
        }
        void setLeftNode        (HuffNode *a) {
            this->leftNode = a;
        }
        void setRightNode       (HuffNode *b) {
            this->rightNode = b;
        }
        void setHeight          (int h) {
            this->hgt = h;
        }
};

class HuffTree
{
    private:
        HuffNode *root;
    public:
        HuffTree    () {

        } 
        
        HuffTree    (char character, int weight, int height, int priority) {
            root = new LeafNode(character, weight, height, priority); countDelete++;
        }
        
        HuffTree    (HuffTree *a, HuffTree *b, int &numPrio) {
            root = new InternalNode(a->root, b->root, numPrio); countDelete++;
        }
        
        ~HuffTree   () {
            clear();
        }
        
        HuffNode*& getRoot      () {
            return this->root;
        }

        void printHuffTree      (HuffNode *root) const {
            if (root == nullptr) {
                return;
            }
            else if (root->isLeaf() == true) {
                LeafNode *leaf = dynamic_cast<LeafNode*>(root);
                if (leaf != nullptr) {
                    cout << leaf->getWeight() << "(" << leaf->getChar() << ")\t";
                }
                else {
                    cout << "Loi 1" << endl;
                }
                return;
            }
            else {
                InternalNode *internal = dynamic_cast<InternalNode*>(root);
                if (internal != nullptr) {
                    cout << internal->getWeight() << "\t";
                    printHuffTree(internal->getLeftNode());
                    printHuffTree(internal->getRightNode());
                }
                else {
                    return;
                }
            }
        }
        
        void recursiveRemove    (HuffNode *&root) {
            if (root == nullptr) {
                return;
            }
            else if (root->isLeaf() == false) {
                InternalNode *internalNode = dynamic_cast<InternalNode*>(root);
                if (internalNode != nullptr) {
                    recursiveRemove(internalNode->getLeftNode());
                    recursiveRemove(internalNode->getRightNode());
                }
            }
            delete root; root = nullptr; countDelete--;
        }
        
        void clear              () {
            recursiveRemove(root);
        }
};

struct compareHuffman
{
    bool operator() (HuffTree *a, HuffTree *b) {
        if (a->getRoot()->getWeight() > b->getRoot()->getWeight()) {
            return true;
        }
        else if (a->getRoot()->getWeight() == b->getRoot()->getWeight()) {
            return a->getRoot()->getPriority() > b->getRoot()->getPriority();
        }
        else {
            return false;
        }
    }
};


inline void rotateLeft      (HuffNode *&rt)
{
    if (rt->isLeaf() == true) {
        return;
    }
    else {
        InternalNode *root = dynamic_cast<InternalNode*>(rt);
        HuffNode *right = root->getRightNode();
        InternalNode *rightRoot = dynamic_cast<InternalNode*>(right);
        if (rightRoot == nullptr) {
            cout << "Loi rotateLeft" << endl;
        }
        else {
            root->setHeight(max(root->getLeftNode()->getHeight(), rightRoot->getLeftNode()->getHeight()) + 1);
            rightRoot->setHeight(max(root->getHeight(), rightRoot->getRightNode()->getHeight()) + 1);
            root->setRightNode(rightRoot->getLeftNode());
            rightRoot->setLeftNode(rt);
            rt = right;
        }
        
    }
}

inline void rotateRight     (HuffNode *&rt)
{
    if (rt->isLeaf() == true) {
        return;
    }
    else {
        InternalNode *root = dynamic_cast<InternalNode*>(rt);
        HuffNode *left = root->getLeftNode();
        InternalNode *leftRoot = dynamic_cast<InternalNode*>(left);
        if (leftRoot == nullptr) {
            cout << "Loi rotateRight." << endl;
        }
        else {
            root->setHeight(max(root->getRightNode()->getHeight(), leftRoot->getRightNode()->getHeight()) + 1);
            leftRoot->setHeight(max(root->getHeight(), leftRoot->getLeftNode()->getHeight()) + 1);
            root->setLeftNode(leftRoot->getRightNode());
            leftRoot->setRightNode(rt);
            rt = left;
        }
    }
}

inline void leftBalance     (HuffNode *&rt, int &count)
{
    InternalNode *root = dynamic_cast<InternalNode*>(rt);
    if (root == nullptr) {
        cout << "Loi leftBalance" << endl;
        return;
    }
    else {
        HuffNode *&left = root->getLeftNode();
        if (left->isLeaf() == true) {
            cout << "Loi leftBalance 1" << endl;
            return;
        }
        else {
            InternalNode *leftRoot = dynamic_cast<InternalNode*>(left);
            int difference = leftRoot->getLeftNode()->getHeight() - leftRoot->getRightNode()->getHeight();
            if (difference >= 0 && count != 0) {
                rotateRight(rt); count--; if (count <= 0) return;
            }
            else if (count-- != 0) {
                rotateLeft(left); count--; if (count <= 0) return;
                rotateRight(rt); count--; if (count <= 0) return;
            }
        }
    }
}

inline void rightBalance    (HuffNode *&rt, int &count)
{
    InternalNode *root = dynamic_cast<InternalNode*>(rt);
    if (root == nullptr) {
        cout << "Loi rightBalance" << endl;
        return;
    }
    else {
        HuffNode *&right = root->getRightNode();
        if (right->isLeaf() == true) {
            cout << "Loi rightBalance 1" << endl;
            return;
        }
        else {
            InternalNode *rightRoot = dynamic_cast<InternalNode*>(right);
            int difference = rightRoot->getLeftNode()->getHeight() - rightRoot->getRightNode()->getHeight();
            if (difference > 0 && count > 0) {
                rotateRight(right); count--; if (count <= 0) return;
                rotateLeft(rt); count--; if (count <= 0) return;
            }
            else if (count > 0) {
                rotateLeft(rt); count--; if (count <= 0) return;
            }
        }
    }
}

inline void balanceProcess  (HuffNode *&root)
{
    int count = 3;
    while (count > 0)
    {
        if (root->isLeaf() == true) {
            return;
        }
        else {
            InternalNode *internal = dynamic_cast<InternalNode*>(root);
            int difference = internal->getLeftNode()->getHeight() - internal->getRightNode()->getHeight();
            if (abs(difference) <= 1) {
                return;
            } 
            else if (difference >= 2) {
                leftBalance(root, count);
            }
            else {
                rightBalance(root, count);
            }
        }
    }
}

inline HuffTree* buildHuff                          (priority_queue<HuffTree*, vector<HuffTree*>, compareHuffman> &forest, int &numPrio)
{
    if (forest.size() == 0) {
        return nullptr;
    }
    else if (forest.size() == 1) {
        HuffTree *res = forest.top(); forest.pop();
        return res;
    }
    else {
        HuffTree *tmp1 = nullptr, *tmp2 =  nullptr, *tmp3 = nullptr;
        while (forest.size() >= 2) {
            tmp1 = forest.top(); forest.pop();
            tmp2 = forest.top(); forest.pop();
            tmp3 = new HuffTree(tmp1, tmp2, numPrio); 
            balanceProcess(tmp3->getRoot());
            forest.push(tmp3);
        }
        return tmp3;
    }
}

inline void printPriorityQueue                      (priority_queue<HuffTree*, vector<HuffTree*>, compareHuffman> pq)
{
    while (!pq.empty()) {
        HuffTree *top = pq.top(); pq.pop();
        if (top->getRoot()->isLeaf() == true) {
            LeafNode* leaf = dynamic_cast<LeafNode*>(top->getRoot());
            if (top != nullptr) {
                cout << leaf->getWeight() << "(" << leaf->getChar() << ")\t";
            }
            else {
                cout << "Loi 3" << endl;
            }
        }
        else {
            cout << top->getRoot()->getWeight() << "\t";
        }
    }
    cout << endl << endl;
}

inline void depthFirstSearch                        (HuffNode *root, vector<pair<char, string>> &res, string tmp)
{
    if (root == nullptr) {
        return;
    }
    if (root->isLeaf() == true) {
        LeafNode *leaf = dynamic_cast<LeafNode*>(root);
        if (leaf != nullptr) {
            res.push_back({leaf->getChar(), tmp});
        }
    }
    else {
        InternalNode *internal= dynamic_cast<InternalNode*>(root);
        if (internal != nullptr) {
            depthFirstSearch(internal->getLeftNode(), res, tmp + "0");
            depthFirstSearch(internal->getRightNode(), res, tmp + "1");
        }
    }
}

inline vector<pair<char, string>> encodeCharacter   (HuffTree *hufftree)
{
    vector<pair<char, string>> res;
    depthFirstSearch(hufftree->getRoot(), res, "");
    return res;
}

inline string encodeString                          (vector<pair<char, string>> &encode, string &input)
{
    string res = "";
    for (char &i : input) {
        vector<pair<char, string>>::iterator it = find_if(encode.begin(), encode.end(), [i] (const pair<char, string> &a)->bool {return i == a.first;});
        if (it != encode.end()) {
            res += it->second;
        }
        else {
            cout << "Thieu ki tu trong vector encode roi!" << endl;
            return "";
        }
    }
    if (res == "") {
        res = "0";
    }
    return res;
}

inline void printHuffTree_HAND                      (HuffNode *root)
{
    if (root == nullptr) {
        return;
    }
    else if (root->isLeaf() == true) {
        LeafNode *leaf = dynamic_cast<LeafNode*>(root);
        cout << leaf->getChar() << "\n";
    }
    else {
        InternalNode *internal = dynamic_cast<InternalNode*>(root);
        printHuffTree_HAND(internal->getLeftNode());
        cout << internal->getWeight() << "\n";
        printHuffTree_HAND(internal->getRightNode());
    }
}
/* -------------------------------------------------------------------------------------------------------------------------------------------------------------- */




/////////////////////////////////////////////	FUNCTION DECLARATION + IMPLEMENTATION	/////////////////////////////////////////////////////////////////////////////
//..............................     FOR LAPSE       .................................................................
struct compareSort_Ceasar
{
    bool operator() (const pair<char, int> &a, const pair<char, int> &b) {
        if (a.second < b.second) {
            return true;
        }
        else if (a.second == b.second) {
            if (a.first >= 'a' && b.first < 'a') {
                return true;
            }
            else if (('a' <= a.first && 'a' <= b.first) || (a.first <= 'Z' && b.first <= 'Z')) {
                return a.first < b.first;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
};

inline void         listFrequency               (vector<pair<char, int>> &vt, string &name)
{
    for (char &i : name) {
        if ('A' <= i && i <= 'Z') {
            if (vt[i - 'A'].first == '\0') {
                vt[i - 'A'].first = i;
            }
            vt[i - 'A'].second++;
        }
        else {
            if (vt[i - 'a' + 26].first == '\0') {
                vt[i - 'a' + 26].first = i;
            }
            vt[i - 'a' + 26].second++;
        }
    }

    int index = 0;
    while (vt.begin() + index != vt.end()) {
        if (vt[index].first == '\0') {
            vt.erase(vt.begin() + index);
            index--;
        }
        index++;
    }
}

inline void         caesarEncode                (vector<pair<char, int>> &vt)
{
    for (pair<char, int> &i : vt) {
        if ('A' <= i.first && i.first <= 'Z') {
            if (i.first + i.second > 'Z') {
                i.first = 'A' + i.first + i.second - 'Z' - 1;
            }
            else {
                i.first = i.first + i.second;
            }
        }
        else {
            if (i.first + i.second > 'z') {
                i.first = 'a' + i.first + i.second - 'z' - 1;
            }
            else {
                i.first = i.first + i.second;
            }
        }
    }

    vector<pair<char, int>> tmpVt (58, {'\0', 0});
    int vtSize = vt.size();
    for (int i = 0; i < vtSize; i++) {
        if (tmpVt[vt[i].first - 'A'].first == '\0') {
            tmpVt[vt[i].first - 'A'].first = vt[i].first;
            tmpVt[vt[i].first - 'A'].second = i;
        }
        else {
            vt[tmpVt[vt[i].first - 'A'].second].second += vt[i].second;
            vt.erase(vt.begin() + i);
            i--;
        }
    }
    tmpVt.clear();

    sort(vt.begin(), vt.end(), compareSort_Ceasar());

}

inline long int     fastPower                   (int x, unsigned short N)
{
    if (N == 0) return 1;
    if (N % 2 == 1 ) return x * fastPower(x, N - 1);
    long int y = fastPower(x, N/2);
    return y*y;
}

inline long int     convert_BinaryToInt         (string &num)
{
    long int res = 0;
    int numSize = num.size();
    for (int i = 0; i < numSize; i++) {
        if (num[i] == '1') {
            res += fastPower(2, num.size() - i - 1);
        }
    }
    return res;
}

inline void         enter_GojoRestaurant        (Hash &Gojo_Restaurant, const int &result)
{
    int id = result % MAXSIZE + 1;
    Gojo_Restaurant.insertCustomer(id, result);
}

inline void         enter_SukunaRestaurant      (Heap &Sukuna_Restaurant, const int &result)
{
    int id = result % MAXSIZE + 1;
    Sukuna_Restaurant.insertCustomer(id, result);
}


//..............................    FOR KOKUSEN       ...................................................................
inline long int permutation                 (Hash::Node *root, vector<long int> &factorial) 
{
    if ((root == nullptr) || (root->left == nullptr && root->right == nullptr)) {
        return 1;
    }
    else {
        long int left = permutation(root->left, factorial);
        long int right = permutation(root->right, factorial);
        int le = totalNodes(root->left);        // Tổng số node con trái = le
        int ri = totalNodes(root->right);       // Tổng số node con phải = ri
        int ro = le + ri;                       // Tổng số node - 1, tức n - 1
        return (factorial[ro] / (factorial[le]*factorial[ro - le])) * left * right;
    }
}

//..............................    FOR KEITEIKEN       .................................................................
inline bool compare1 (vector<Heap::areaFields> &listArea, list<int> &listIdOrder, const int &a, const int &b) {
    int index1 = getIndex_inListArea(listArea, a);
    int index2 = getIndex_inListArea(listArea, b);

    return isSmallerArea(listArea, listIdOrder, index1, index2);
}

/* --------------------------------------------------------------------------------------------------------------------------------------------------------------- */



/////////////////////////////////////////////	MAIN CLASS TO SIMULATE	//////////////////////////////////////////////////////////////////////////////////////////////
class Implement_Function
{
public:
	Hash Gojo_Restaurant;
	Heap Sukuna_Restaurant;
    HuffTree *lastComingCustomer;
public:
	Implement_Function  () {
        lastComingCustomer = nullptr;
    }
	~Implement_Function () {
        delete lastComingCustomer; lastComingCustomer = nullptr;
    }


	///////////////////////^^^^^^^^^^^^^^^^^^^^^ LAPSE FUNCTION ^^^^^^^^^^^^^^^^^^^^^
	void LAPSE      (string name) 
	{
        // Chuẩn hóa tên khách hàng:
        /* 1. Liệt kê tần suất xuất hiện + sắp xếp theo thứ tự tăng dần */
        vector<pair<char, int>> listNameFreq (52, {'\0', 0});
        listFrequency(listNameFreq, name);
        if (listNameFreq.size() < 3) {
            return;
        }
        
        string input = "";
        /* 2. Mật mã Caesar */
        char newStr;
        for (char &i : name) {
            vector<pair<char, int>>::iterator it = find_if(listNameFreq.begin(), listNameFreq.end(), [i] (const pair<char, int> &a)->bool {return i == a.first;});
            if (it != listNameFreq.end()) {
                if ('A' <= it.base()->first && it.base()->first <= 'Z') {
                    if (it.base()->first + it.base()->second > 'Z') {
                        newStr = ('A' + it.base()->first + it.base()->second - 'Z' - 1);
                        input += newStr;
                    }
                    else {
                        newStr = (it.base()->first + it.base()->second);
                        input += newStr;
                    }
                }
                else {
                    if (it.base()->first + it.base()->second > 'z') {
                        newStr = ('a' + it.base()->first + it.base()->second - 'z' - 1);
                        input += newStr;
                    }
                    else {
                        newStr = (it.base()->first + it.base()->second);
                        input += newStr;
                    }
                }
            }
        }     
        caesarEncode(listNameFreq);

        /* 3. Xây dựng Huffman */
        priority_queue<HuffTree*, vector<HuffTree*>, compareHuffman> forest;
        int numPrio = 0;
        int tmp = listNameFreq.size();
        for (int i = 0; i < tmp; i++) {
            HuffTree *newNode = new HuffTree(listNameFreq[i].first, listNameFreq[i].second, 0, numPrio++);
            forest.push(newNode);
        }
        HuffTree *huff_tree = buildHuff(forest, numPrio);

        /* Cap nhat cay Hufftree duoc them gan nhat */
        delete lastComingCustomer;
        lastComingCustomer = huff_tree;

        vector<pair<char, string>> listEncode = encodeCharacter(huff_tree);

        string binaryString = encodeString(listEncode, input);

        string num = "";
        for (int i = binaryString.size() - 1, k = 9; i >= 0 && k >= 0; i--, k--) {
            num += binaryString[i];
        }
        int RESULT = convert_BinaryToInt(num);


        // Chọn nhà hàng:
        if (RESULT % 2 == 0) {
            enter_SukunaRestaurant(Sukuna_Restaurant, RESULT);
        }
        else {
            if (Gojo_Restaurant.listArea.empty()) {
                for (int i = 0; i <= MAXSIZE; i++) {
                    Hash::areaFields obj;
                    Gojo_Restaurant.listArea.push_back(obj);
                }
            }
            enter_GojoRestaurant(Gojo_Restaurant, RESULT);
        }
	}

	///////////////////////^^^^^^^^^^^^^^^^^^^^^ KOKUSEN FUNCTION ^^^^^^^^^^^^^^^^^^^^^
	void KOKUSEN    () 
	{
        long long Y;
        vector<long int> factorial;
        
        if (!Gojo_Restaurant.listArea.empty()) {
            int GojoSize = Gojo_Restaurant.listArea.size();
            for (int i = 1; i < GojoSize; i++) {
                if (Gojo_Restaurant.listArea[i].root == nullptr) {
                    continue;
                }

                factorial.push_back(1);
                factorial.push_back(1);
                int GojoListSize = Gojo_Restaurant.listArea[i].order.size();
                for (int j = 2; j < GojoListSize; j++) {
                    factorial.push_back(j * factorial[j - 1]);
                }
                
                Y = permutation(Gojo_Restaurant.listArea[i].root, factorial);

                factorial.clear();
                
                Gojo_Restaurant.removeCustomerAt(i, Y % MAXSIZE);

                GojoSize = Gojo_Restaurant.listArea.size();
            }
        }
	}

	///////////////////////^^^^^^^^^^^^^^^^^^^^^ KEITEIKEN FUNCTION ^^^^^^^^^^^^^^^^^^^^^
	void KEITEIKEN  (int num) 
	{
        if (num < 1 || num > MAXSIZE || Sukuna_Restaurant.listArea.size() == 0) {
            return;
        }

        for (int i = 0; i < num; i++) {
            if (Sukuna_Restaurant.listArea.size() == 0) {
                break;
            }

            list<int> copied_listIdOrder(Sukuna_Restaurant.listIdOrder.begin(), Sukuna_Restaurant.listIdOrder.end());
            copied_listIdOrder.sort([this] (const int &a, const int &b) {return compare1(Sukuna_Restaurant.listArea, Sukuna_Restaurant.listIdOrder, a, b);});
            Sukuna_Restaurant.remove_NumCus_inArea(copied_listIdOrder.front(), num);
            copied_listIdOrder.clear();
        }
    }

	///////////////////////^^^^^^^^^^^^^^^^^^^^^ HAND FUNCTION ^^^^^^^^^^^^^^^^^^^^^
	void HAND       () 
	{
        if (lastComingCustomer == nullptr) {
            return;
        }

        printHuffTree_HAND(lastComingCustomer->getRoot());
	}

	///////////////////////^^^^^^^^^^^^^^^^^^^^^ LIMITLESS FUNCTION ^^^^^^^^^^^^^^^^^^^^^
	void LIMITLESS  (int num) 
	{
        if (num < 1 || num > MAXSIZE) {
            return;
        }
        else {
            Gojo_Restaurant.printBST_LIMITLESS(num);
        }
	}

	///////////////////////^^^^^^^^^^^^^^^^^^^^^ CLEAVE FUNCTION ^^^^^^^^^^^^^^^^^^^^^
	void CLEAVE     (int num) 
	{
        if (Sukuna_Restaurant.listArea.size() == 0) {
            return;
        }
        Sukuna_Restaurant.printArea_preOrder_CLEAVE(0, num);
	}
};
/* --------------------------------------------------------------------------------------------------------------------------------------------------------------- */



/////////////////////////////////////////////	FUNCTION DISTRIBUTION	/////////////////////////////////////////////////////////////////////////////////////////////
inline void simulate (string filename)
{
	ifstream input (filename);
	string str, maxsize, name, num;
	Implement_Function *implement = new Implement_Function(); countDelete++;

	while (input >> str) {
		if (str == "MAXSIZE") {
			input >> maxsize;
			MAXSIZE = stoi(maxsize);
		}
		else if (str == "LAPSE") {				// LAPSE <NAME>
			input >> name;
			implement->LAPSE(name);
		}
		else if (str == "KOKUSEN") {			// KOKUSEN
			implement->KOKUSEN();
		}
		else if (str == "KEITEIKEN") {			// KEITEIKEN <NUM>
			input >> num;
			implement->KEITEIKEN(stoi(num));
		}
		else if (str == "HAND") {				// HAND
			implement->HAND();
		}
		else if (str == "LIMITLESS") {			// LIMITLESS <NUM>
			input >> num;
			implement->LIMITLESS(stoi(num));
		}
		else if (str == "CLEAVE") {				// CLEAVE <NUM>
			input >> num;
			implement->CLEAVE(stoi(num));
		}
	}
    delete implement; implement = nullptr; countDelete--;
}
/* --------------------------------------------------------------------------------------------------------------------------------------------------------------- */