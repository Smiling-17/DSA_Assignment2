// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n) {
    for (int i = 0; i < n; i++) {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const {
    int height = this->getHeight();
		if (this->root == NULL)
		{
			cout << "NULL\n";
			return;
		}
		queue<AVLNode *> q;
		q.push(root);
		AVLNode *temp;
		int count = 0;
		int maxNode = 1;
		int level = 0;
		int space = pow(2, height);
		printNSpace(space / 2);
		while (!q.empty())
		{
			temp = q.front();
			q.pop();
			if (temp == NULL)
			{
				cout << " ";
				q.push(NULL);
				q.push(NULL);
			}
			else
			{
				cout << temp->data;
				q.push(temp->pLeft);
				q.push(temp->pRight);
			}
			printNSpace(space);
			count++;
			if (count == maxNode)
			{
				cout << endl;
				count = 0;
				maxNode *= 2;
				level++;
				space /= 2;
				printNSpace(space / 2);
			}
			if (level == height)
				return;
		}
}


//TODO: Implement all AVLTree<K, T> methods here
template <class K, class T>
AVLTree<K, T>::AVLTree() {
    this->root = nullptr;
}

template <class K, class T>
void AVLTree<K, T>::clearHelper(AVLNode* node) {
    if (node == nullptr) 
        return;
    
    clearHelper(node->pLeft);
    clearHelper(node->pRight);
    delete node;
}

template <class K, class T>
void AVLTree<K, T>::clear() {
    clearHelper(this->root);
    this->root = nullptr;
}

template <class K, class T>
AVLTree<K, T>::~AVLTree() {
    clear();
}

template <class K, class T>
bool AVLTree<K, T>::empty() const {
    return (root == nullptr);
}

// -----------------
template <class K, class T>
int AVLTree<K, T>::height(AVLNode* node) const {
    if (node == nullptr) 
        return 0;
    
    int height_Left = height(node->pLeft);
    int height_Right = height(node->pRight);

    int max_values = 0;
    if (height_Left >= height_Right)
        max_values = height_Left;
    else    
        max_values = height_Right;
    
    return (1 + max_values);
}

template <class K, class T>
int AVLTree<K, T>::getHeight() const {
    return height(this->root);
}

// -----------------
template <class K, class T>
int AVLTree<K, T>::size(AVLNode* node) const {
    if (node == nullptr) 
        return 0;
    
    return (1 + size(node->pLeft) + size(node->pRight));
}

template <class K, class T>
int AVLTree<K, T>::getSize() const {
    return size(this->root);
}

// -----------------
template<class K, class T>
typename AVLTree<K,T>::AVLNode* 
AVLTree<K,T>::rotateRight(typename AVLTree<K,T>::AVLNode*& node) 
{
    if (node == nullptr || node->pLeft == nullptr) return node;

    AVLNode* newRoot = node->pLeft;      
    node->pLeft = newRoot->pRight;    
    newRoot->pRight = node;           
    return newRoot;  
}

template<class K, class T>
typename AVLTree<K,T>::AVLNode* 
AVLTree<K,T>::rotateLeft(typename AVLTree<K,T>::AVLNode*& node) 
{
    if (node == nullptr || node->pRight == nullptr) return node;

    AVLNode* newRoot = node->pRight;     
    node->pRight = newRoot->pLeft;    
    newRoot->pLeft = node;            
    return newRoot;    
}

// -----------------
template <class K, class T>
bool AVLTree<K, T>::containsNode(AVLNode* node, const K& key) const {
    if (node == nullptr) 
        return false;
    
    if (node->key == key) {
        return true;
    }
    else if (key < node->key) {
        return containsNode(node->pLeft, key);
    }
    else {
        return containsNode(node->pRight, key);
    }
}

template <class K, class T>
bool AVLTree<K, T>::contains(const K& key) const {
    return containsNode(this->root, key);
}

// -----------------
template <class K, class T>
void AVLTree<K, T>::inorderHelper(AVLNode* node, void (*action)(const T&)) const {
    if (node == nullptr)
        return;
    
    inorderHelper(node->pLeft, action);
    action(node->data);
    inorderHelper(node->pRight, action);
}

template <class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T&)) const {
    inorderHelper(this->root, action);
}

// -----------------
template <class K, class T>
void AVLTree<K,T>::insert(const K& key, const T& value) {
    bool taller = false;
    insertNode(this->root, key, value, taller);
}

template <class K, class T>
bool AVLTree<K,T>::insertNode(AVLNode*& node, const K& key, const T& value, bool& taller) {
    if (node == nullptr) {
        AVLNode* newNode = new AVLNode(key, value);
        node = newNode;
        taller = true;
        return true;
    }

    if (key == node->key) {
        taller = false;
        return false;
    }
    else if (key < node->key) {
        bool inserted = insertNode(node->pLeft, key, value, taller);
        if (inserted == false)
            return false;

        if (taller == true) {
            if (node->balance == LH) {
                leftBalance(node, taller);
            }
            else if (node->balance == EH) {
                taller = true;
                node->balance = LH;
            }
            else {
                node->balance = EH;
                taller = false;            
            }
        }

        return true;
    }
    else {
        bool inserted = insertNode(node->pRight, key, value, taller);
        if (inserted == false)
            return false;

        if (taller == true) {
            if (node->balance == RH) {
                rightBalance(node, taller);
            }
            else if (node->balance == EH) {
                node->balance = RH;
                taller = true;
            }
            else {
                node->balance = EH;
                taller = false;            
            }
        }

        return true;
    }
}

template <class K, class T>
void AVLTree<K,T>::leftBalance(AVLNode*& node, bool& taller) {
    AVLNode* L = node->pLeft;

    if (L->balance == LH) {
        node = rotateRight(node);
        node->balance = EH;
        node->pRight->balance = EH;
        taller = false;
    }
    else if (L->balance == RH) {
        AVLNode* LR = L->pRight;

        if (LR->balance == LH) {
            L->balance = EH;
            node->balance = RH;
        }
        else if (LR->balance == RH) {
            L->balance = LH;
            node->balance = EH;
        }
        else {
            L->balance = EH;
            node->balance = EH;
        }

        node->pLeft = rotateLeft(node->pLeft);
        node = rotateRight(node);
        
        node->balance = EH;
        taller = false;
    }
}

template <class K, class T>
void AVLTree<K,T>::rightBalance(AVLNode*& node, bool& taller) {
    AVLNode* R = node->pRight;

    if (R->balance == RH) {
        node = rotateLeft(node);
        node->balance = EH;
        node->pLeft->balance = EH;
        taller = false;
    }
    else if (R->balance == LH) {
        AVLNode* RL = R->pLeft;

        if (RL->balance == RH) {
            R->balance = EH;
            node->balance = LH;
        }
        else if (RL->balance == LH) {
            R->balance = RH;
            node->balance = EH;
        }
        else {
            R->balance = EH;
            node->balance = EH;
        }

        node->pRight = rotateRight(node->pRight);
        node = rotateLeft(node);
        
        node->balance = EH;
        taller = false;
    }
}

// -----------------
template <class K, class T>
void AVLTree<K,T>::remove(const K& key) {
    bool shorter = false;
    bool success = false;
    removeNode(this->root, key, shorter, success);
}

template <class K, class T>
bool AVLTree<K,T>::removeNode(AVLNode*& node, const K& key, bool& shorter, bool& success) {
    if (node == nullptr) {
        shorter = false;
        success = false;

        return false;
    }

    if (key < node->key) {
        bool removed = removeNode(node->pLeft, key, shorter, success);
        if (removed == false)
            return false;
        
        if (shorter == true) {
            if (node->balance == LH) {
                node->balance = EH;
                shorter = true;
            }
            else if (node->balance == EH) {
                node->balance = RH;
                shorter = false;
            }
            else {
                rightBalance(node, shorter);
            }
        }

        return true;
    }
    else if (key > node->key) {
        bool removed = removeNode(node->pRight, key, shorter, success);
        if (removed == false)
            return false;
        
        if (shorter == true) {
            if (node->balance == RH) {
                node->balance = EH;
                shorter = true;
            }
            else if (node->balance == EH) {
                node->balance = LH;
                shorter = false;
            }
            else {
                leftBalance(node, shorter);
            }
        }

        return true;
    }
    else {
        if (node->pLeft == nullptr && node->pRight == nullptr) {
            delete node;
            node = nullptr;

            success = true;
            shorter = true;

            return true;
        }
        else if (node->pLeft != nullptr && node->pRight == nullptr) {
            AVLNode* temp = node;
            node = node->pLeft;
            delete temp;

            success = true;
            shorter = true;

            return true;
        }
        else if (node->pRight != nullptr && node->pLeft == nullptr) {
            AVLNode* temp = node;
            node = node->pRight;
            delete temp;

            success = true;
            shorter = true;

            return true;
        }
        else {
            AVLNode* pred = node->pLeft;
            while (pred->pRight != nullptr) {
                pred = pred->pRight;
            }

            node->key = pred->key;
            node->data = pred->data;

            bool dummySuccess = false;
            bool dummyShorter = false;
            removeNode(node->pLeft, pred->key, dummyShorter, dummySuccess);

            success = true;
            shorter = dummyShorter;
        }
    }

    if (shorter == true) {
        if (node->balance == LH) {
            node->balance = EH;
            shorter = true;
        }
        else if (node->balance == EH) {
            node->balance = RH;
            shorter = false;
        }
        else { 
            rightBalance(node, shorter);
        }
    }

    return true;
}


// =====================================
// RedBlackTree<K, T> implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const {
    if (this->root == nullptr) {
        cout << "NULL\n";
        return;
    }
    
    queue<RBTNode*> q;
    q.push(root);
    RBTNode* temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    
    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode*> heightQ;
    heightQ.push(root);
    while (!heightQ.empty()) {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++) {
            RBTNode* node = heightQ.front();
            heightQ.pop();
            if (node->left) heightQ.push(node->left);
            if (node->right) heightQ.push(node->right);
        }
    }
    
    int space = pow(2, height);
    printNSpace(space / 2);
    
    while (!q.empty()) {
        temp = q.front();
        q.pop();
        
        if (temp == nullptr) {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        } else {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED) {
                cout << "R)";
            } else {
                cout << "B)";
            }
            
            q.push(temp->left);
            q.push(temp->right);
        }
        
        printNSpace(space);
        count++;
        
        if (count == maxNode) {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        
        if (level == height) {
            return;
        }
    }
}

//TODO: Implement all other RedBlackTree<K, T> methods here
template <class K, class T>
RedBlackTree<K,T>::RBTNode::RBTNode(const K& key, const T& value)
    : key(key), data(value), color(RED), parent(nullptr), left(nullptr), right(nullptr) {
}

template <class K, class T>
void RedBlackTree<K,T>::RBTNode::recolorToRed() {
    this->color = RED;
}

template <class K, class T>
void RedBlackTree<K,T>::RBTNode::recolorToBlack() {
    this->color = BLACK;
}

// -----------------
template <class K, class T>
static void rbClearHelper(typename RedBlackTree<K,T>::RBTNode* node) {
    if (node == nullptr) 
        return;

    rbClearHelper<K,T>(node->left);
    rbClearHelper<K,T>(node->right);
    delete node;
}

template <class K, class T>
void RedBlackTree<K,T>::clear() {
    rbClearHelper<K,T>(this->root);
    this->root = nullptr;
}

template <class K, class T>
RedBlackTree<K,T>::RedBlackTree() {
    this->root = nullptr;
}

template <class K, class T>
RedBlackTree<K,T>::~RedBlackTree() {
    clear();
}

template <class K, class T>
bool RedBlackTree<K,T>::empty() const {
    return (root == nullptr);
}

template <class K, class T>
static int rbSizeHelper(typename RedBlackTree<K,T>::RBTNode* node) {
    if (node == nullptr) return 0;
    return 1 + rbSizeHelper<K,T>(node->left) + rbSizeHelper<K,T>(node->right);
}

template <class K, class T>
int RedBlackTree<K,T>::size() const {
    return rbSizeHelper<K,T>(this->root);
}


template <class K, class T>
typename RedBlackTree<K,T>::RBTNode* 
RedBlackTree<K,T>::find(const K& key) const {
    RBTNode* curr = this->root;

    while (curr != nullptr) {
        if (key == curr->key) {
            return curr;
        }
        else if (key < curr->key) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }

    return nullptr;
}

template <class K, class T>
bool RedBlackTree<K,T>::contains(const K& key) const {
    return (find(key) != nullptr);
}

// -----------------
template <class K, class T>
void RedBlackTree<K,T>::rotateLeft(RBTNode* node) {
    RBTNode* pivot = node->right;
    if (pivot == nullptr)
        return;

    // Di chuyển pivot->left thành node->right
    node->right = pivot->left;
    if (pivot->left != nullptr) {
        pivot->left->parent = node;
    }

    // Pivot lấy cha của node
    pivot->parent = node->parent;

    if (node->parent == nullptr) {
        root = pivot;
    }
    else if (node == node->parent->left) {
        node->parent->left = pivot;
    }
    else {
        node->parent->right = pivot;
    }

    // Pivot trở thành cha của node
    pivot->left = node;
    node->parent = pivot;
}

template <class K, class T>
void RedBlackTree<K,T>::rotateRight(RBTNode* node) {
    RBTNode* pivot = node->left;
    if (pivot == nullptr) 
        return; 

    // Di chuyển pivot->right thành node->left 
    node->left = pivot->right;
    if (pivot->right != nullptr) {
        pivot->right->parent = node;
    }

    // Pivot lấy cha của node
    pivot->parent = node->parent;

    if (node->parent == nullptr) {
        root = pivot;
    }
    else if (node == node->parent->left) {
        node->parent->left = pivot;
    }
    else {
        node->parent->right = pivot;
    }

    // Pivot trở thành cha của node
    pivot->right = node;
    node->parent = pivot;
}

// -----------------
template <class K, class T>
void RedBlackTree<K,T>::insert(const K& key, const T& value) {
    if (this->root == nullptr) {
        this->root = new RBTNode(key, value);
        this->root->recolorToBlack();
        return;
    }

    RBTNode* newNode = new RBTNode(key, value);

    RBTNode* parent = nullptr;
    RBTNode* curr = root;

    while (curr != nullptr) {
        parent = curr;

        if (key < curr->key) {
            curr = curr->left;
        }
        else if (key > curr->key) {
            curr = curr->right;
        }
        else {
            delete newNode;
            return;
        }
    }

    newNode->parent = parent;

    if (key < parent->key) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }

    fixInsert(newNode);
}

template <class K, class T>
void RedBlackTree<K,T>::fixInsert(RBTNode* node) {
    while (node != this->root && node->parent->color == RED) {
        RBTNode* parent = node->parent;
        RBTNode* grandparent = parent->parent;

        if (parent == grandparent->left) {
            RBTNode* uncle = grandparent->right;

            if (uncle != nullptr && uncle->color == RED) {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();

                node = grandparent;
            }
            else {
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }

                parent->recolorToBlack();
                grandparent->recolorToRed();
                rotateRight(grandparent);
            }
        }
        else {
            RBTNode* uncle = grandparent->left;

            if (uncle != nullptr && uncle->color == RED) {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();

                node = grandparent;
            }
            else {
                if (node == parent->left) {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }

                parent->recolorToBlack();
                grandparent->recolorToRed();
                rotateLeft(grandparent);
            }
        }
    }

    this->root->recolorToBlack();
}

// -----------------
template <class K, class T>
typename RedBlackTree<K,T>::RBTNode*
RedBlackTree<K,T>::lowerBound(const K& key, bool& found) const {
    RBTNode* curr = this->root;
    RBTNode* result = nullptr;

    found = false;

    while (curr != nullptr) {
        if (curr->key >= key) {
            result = curr;
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }

    if (result != nullptr) {
        found = true;
    }
    return result;
}

template <class K, class T>
typename RedBlackTree<K,T>::RBTNode*
RedBlackTree<K,T>::upperBound(const K& key, bool& found) const {
    RBTNode* curr = this->root;
    RBTNode* result = nullptr;

    found = false;

    while (curr != nullptr) {
        if (curr->key > key) {
            result = curr;
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }

    if (result != nullptr) {
        found = true;
    }
    return result;
}

template <class K, class T>
typename RedBlackTree<K,T>::RBTNode*
RedBlackTree<K,T>::lowerBoundNode(const K& key) const {
    RBTNode* curr = this->root;
    RBTNode* result = nullptr;

    while (curr != nullptr) {
        if (curr->key >= key) {
            result = curr;       
            curr = curr->left;   
        }
        else {
            curr = curr->right;  
        }
    }

    return result;   
}

template <class K, class T>
typename RedBlackTree<K,T>::RBTNode*
RedBlackTree<K,T>::upperBoundNode(const K& key) const {
    RBTNode* curr = this->root;
    RBTNode* result = nullptr;

    while (curr != nullptr) {
        if (curr->key > key) {
            result = curr;      
            curr = curr->left;  
        }
        else {
            curr = curr->right; 
        }
    }

    return result;
}

// -----------------
template <class K, class T>
void RedBlackTree<K,T>::remove(const K& key) {
    RBTNode* node = find(key);
    if (node == nullptr) 
        return;
    
    RBTNode* x = nullptr;
    RBTNode* xParent = nullptr;
    Color deletedColor = node->color;

    if (node->left == nullptr && node->right == nullptr) {
        x = nullptr;
        xParent = node->parent;

        if (node->parent == nullptr) {
            this->root = nullptr;
        }
        else if (node == node->parent->left) {
            node->parent->left = nullptr;
        }
        else {
            node->parent->right = nullptr;
        }

        delete node;
    }
    else if (node->left == nullptr || node->right == nullptr) {
        RBTNode* child = nullptr;
        if (node->left != nullptr) {
            child = node->left;
        }
        else {
            child = node->right;
        }

        child->parent = node->parent;

        if (node->parent == nullptr) {
            this->root = child;
        }
        else if (node == node->parent->left) {
            node->parent->left = child;
        }
        else {
            node->parent->right = child;
        }

        if (node->color == BLACK && child->color == RED) {
            child->recolorToBlack();
            deletedColor = RED;   
        }

        x = child;
        xParent = child->parent;
        delete node;
    }
    else {
        RBTNode* pred = node->left;
        
        while (pred->right != nullptr) {
            pred = pred->right;
        }

        node->key = pred->key;
        node->data = pred->data;

        deletedColor = pred->color;

        RBTNode* child = pred->left;

        if (child != nullptr) {
            child->parent = pred->parent;
        }

        if (pred->parent->left == pred) {
            pred->parent->left = child;
        }
        else {
            pred->parent->right = child;
        }

        if (pred->color == BLACK && child != nullptr && child->color == RED) {
            child->recolorToBlack();
            deletedColor = RED;  
        }

        x = child;
        xParent = pred->parent;
        delete pred;
    }

    if (deletedColor == BLACK && xParent != nullptr) {
        fixDelete(x, xParent);
    }
}

template <class K, class T>
void RedBlackTree<K,T>::fixDelete(RBTNode* x, RBTNode* parent) {
    while (x != this->root && (x == nullptr || x->color == BLACK)) {
        // CASE 1 – Sibling ĐỎ
        if (x == parent->left) {
            RBTNode* sibling = parent->right;

            if (sibling == nullptr) {
                x = parent;
                parent = x->parent;
                continue;
            }

            if (sibling->color == RED) {
                sibling->recolorToBlack();
                parent->recolorToRed();
                rotateLeft(parent);
                sibling = parent->right;
            }

            // CASE 2 – sibling BLACK & cả 2 con đều BLACK (hoặc nullptr)
            bool leftBlack = (sibling->left == nullptr || sibling->left->color == BLACK);
            bool rightBlack = (sibling->right == nullptr || sibling->right->color == BLACK);

            if (leftBlack && rightBlack) {
                sibling->recolorToRed();
                x = parent;
                parent = x->parent;
                continue;
            }

            // CASE 3 – sibling BLACK, near-nephew RED, far-nephew BLACK
            if (sibling != nullptr) { 
                if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                    (sibling->left  != nullptr && sibling->left->color  == RED)) {

                    sibling->left->recolorToBlack();
                    sibling->recolorToRed();
                    rotateRight(sibling);
                    sibling = parent->right;   
                }
            }

            // CASE 4 – sibling BLACK, far-nephew RED
            if (sibling != nullptr) {
                sibling->color = parent->color;   
                parent->recolorToBlack();
                if (sibling->right != nullptr)
                    sibling->right->recolorToBlack();

                rotateLeft(parent);

                x = this->root;   
            }


        }
        else {
            RBTNode* sibling = parent->left;

            if (sibling == nullptr) {
                x = parent;
                parent = x->parent;
                continue;
            }

            // CASE 1: sibling RED
            if (sibling->color == RED) {
                sibling->recolorToBlack();
                parent->recolorToRed();
                rotateRight(parent);
                sibling = parent->left;
            }

            bool leftBlack  = (sibling->left  == nullptr || sibling->left->color  == BLACK);
            bool rightBlack = (sibling->right == nullptr || sibling->right->color == BLACK);

            // CASE 2: sibling BLACK & 2 con BLACK
            if (leftBlack && rightBlack) {
                sibling->recolorToRed();
                x = parent;
                parent = x->parent;
                continue;
            }

            // CASE 3: sibling BLACK, near-nephew RED, far-nephew BLACK
            if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling->right != nullptr && sibling->right->color == RED)) {

                sibling->right->recolorToBlack();
                sibling->recolorToRed();
                rotateLeft(sibling);
                sibling = parent->left;
            }

            // CASE 4: sibling BLACK, far-nephew RED
            sibling->color = parent->color;
            parent->recolorToBlack();
            if (sibling->left != nullptr)
                sibling->left->recolorToBlack();

            rotateRight(parent);

            x = this->root;   
        }
    }

    if (x != nullptr) {
        x->recolorToBlack();
    }
}



// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
std::ostream& operator<<(std::ostream& os, const VectorRecord& record) {
    os << record.id;
    return os;
}

// =====================================
// VectorStore implementation
// =====================================

//TODO: Implement all VectorStore methods here

VectorStore::VectorStore(int dimension,
                    std::vector<float>* (*embeddingFunction)(const std::string&),
                    const std::vector<float>& referenceVector) 
    : dimension(dimension), embeddingFunction(embeddingFunction), count(0), averageDistance(0.0f), rootVector(nullptr) {
    
    this->vectorStore = new AVLTree<double, VectorRecord>();
    this->normIndex = new RedBlackTree<double, VectorRecord>();
    
    this->referenceVector = new std::vector<float>(referenceVector);
}

VectorStore::~VectorStore() {
    clear();

    if (vectorStore != nullptr) 
        delete vectorStore;
    
    if (normIndex != nullptr) 
        delete normIndex;
    
    if (referenceVector) 
        delete referenceVector;
    
}

int VectorStore::size() {
    return this->count;  
}

bool VectorStore::empty() {
    return count == 0;  
}

static void deleteVectorDataRec(AVLTree<double, VectorRecord>::AVLNode* node) {
    if (node == nullptr) 
        return;

    deleteVectorDataRec(node->pLeft);    
    deleteVectorDataRec(node->pRight);

    if (node->data.vector != nullptr) {
        delete node->data.vector;   
        node->data.vector = nullptr; 
    }
}

void VectorStore::clear() {
    if (vectorStore != nullptr) {
        deleteVectorDataRec(vectorStore->getRoot()); 
        this->vectorStore->clear(); 
    }

    if (normIndex != nullptr) {
        this->normIndex->clear(); 
    }

    count = 0;
    averageDistance = 0.0;
    rootVector = nullptr;
}

//-----------------
// Hàm hoán đổi 
static void manualSwap(VectorRecord& a, VectorRecord& b) {
    VectorRecord temp = a;
    a = b;
    b = temp;
}

// Hàm phân hoạch cho QuickSort
static int partition(std::vector<VectorRecord>& arr, int low, int high) {
    double pivot = arr[high].distanceFromReference;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        // Sắp xếp tăng dần theo distanceFromReference
        if (arr[j].distanceFromReference < pivot) {
            i++;
            manualSwap(arr[i], arr[j]);
        }
    }
    manualSwap(arr[i + 1], arr[high]);

    return (i + 1);
}

// Hàm QuickSort đệ quy
static void myQuickSort(std::vector<VectorRecord>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        myQuickSort(arr, low, pi - 1);
        myQuickSort(arr, pi + 1, high);
    }
}

std::vector<float>* VectorStore::preprocessing(std::string rawText) {
    std::vector<float>* vec = embeddingFunction(rawText);

    // Safety check
    if (vec == nullptr) 
        return new std::vector<float>(dimension, 0.0f);

    // Chuẩn hóa kích thước
    if ((int)vec->size() > this->dimension) {
        vec->resize(this->dimension);
    }
    else if ((int)vec->size() < this->dimension) {
        vec->resize(this->dimension, 0.0f);
    }

    return vec;
}

// Dùng để lấy dữ liệu khi cần Rebuild cây
static void collectVectors(AVLTree<double, VectorRecord>::AVLNode* node, std::vector<VectorRecord*>& list) {
    if (node == nullptr) 
        return;

    collectVectors(node->pLeft, list);
    list.push_back(&(node->data));
    collectVectors(node->pRight, list);
}

// Tìm ID lớn nhất hiện có
static int findMaxId(AVLTree<double, VectorRecord>::AVLNode* node) {
    if (node == nullptr) 
        return -1;

    int maxLeft = findMaxId(node->pLeft);
    int maxRight = findMaxId(node->pRight);
    
    int currentMax = node->data.id;
    if (maxLeft > currentMax) 
        currentMax = maxLeft;
    if (maxRight > currentMax) 
        currentMax = maxRight;
    
    return currentMax;
}

// Dựng cây cân bằng từ danh sách đã sắp xếp
void buildBalancedAVL(AVLTree<double, VectorRecord>& tree, const std::vector<VectorRecord>& sortedRecords, int left, int right) {
    if (left > right) 
        return;
    
    int mid = left + (right - left) / 2;
    
    // Insert phần tử giữa
    tree.insert(sortedRecords[mid].distanceFromReference, sortedRecords[mid]);
    
    // Đệ quy
    buildBalancedAVL(tree, sortedRecords, left, mid - 1);
    buildBalancedAVL(tree, sortedRecords, mid + 1, right);
}

void VectorStore::addText(std::string rawText) {
    // 1. Preprocessing
    std::vector<float>* vec = preprocessing(rawText);

    // 2. Tính khoảng cách
    double distance = l2Distance(*vec, *referenceVector);

    // 3. Cập nhật khoảng cách trung bình
    if (this->count == 0) {
        this->averageDistance = distance;
    }
    else {
        this->averageDistance = (this->averageDistance * this->count + distance) / (this->count + 1);
    }

    // 4. Tính chuẩn Euclidean (norm) cho RBT
    double norm = 0.0;
    for (size_t i = 0; i < vec->size(); i++) {
        norm += (*vec)[i] * (*vec)[i];
    }
    norm = sqrt(norm);

    // 5. Tạo ID mới
    int newId = 0;
    if (this->count > 0 && vectorStore->getRoot() != nullptr) {
        newId = findMaxId(vectorStore->getRoot()) + 1;
    }

    // 6. Tạo VectorRecord mới
    VectorRecord newRecord(newId, rawText, vec, distance);
    newRecord.norm = norm; 
    
    if (this->count == 0) {
        vectorStore->insert(distance, newRecord);
        normIndex->insert(norm, newRecord);
        rootVector = &(vectorStore->getRoot()->data);
        this->count++;
        return;
    }

    normIndex->insert(norm, newRecord);

    // Kiểm tra điều kiện Rebuild
    double distNewToAvg = fabs(distance - this->averageDistance);
    double distRootToAvg = fabs(rootVector->distanceFromReference - this->averageDistance);

    if (distNewToAvg < distRootToAvg) {
        // Lấy vector cũ
        std::vector<VectorRecord*> existingPtrs;
        collectVectors(vectorStore->getRoot(), existingPtrs);
        
        // Tạo list mới (copy value)
        std::vector<VectorRecord> allRecords;
        for (auto ptr : existingPtrs) {
            allRecords.push_back(*ptr); 
        }
        allRecords.push_back(newRecord); 
        
        // Clear AVL cũ
        vectorStore->clear();
        
        // Insert Root mới
        vectorStore->insert(newRecord.distanceFromReference, newRecord);        
        rootVector = &(vectorStore->getRoot()->data);
        
        // Chia nhóm
        std::vector<VectorRecord> leftList;
        std::vector<VectorRecord> rightList;
        
        for (const auto& rec : allRecords) {
            if (rec.id == newId) 
                continue;
            
            if (rec.distanceFromReference < rootVector->distanceFromReference) {
                leftList.push_back(rec);
            } 
            else {
                rightList.push_back(rec);
            }
        }
        
        // Sắp xếp
        if (!leftList.empty())
            myQuickSort(leftList, 0, leftList.size() - 1);
        
        if (!rightList.empty())
            myQuickSort(rightList, 0, rightList.size() - 1);
        
        // Dựng cây cân bằng
        buildBalancedAVL(*vectorStore, leftList, 0, leftList.size() - 1);
        buildBalancedAVL(*vectorStore, rightList, 0, rightList.size() - 1);
        
    } else {
        vectorStore->insert(distance, newRecord);
    }

    count++;
}

//-----------------
// counter: Biến tham chiếu để đếm số node đã duyệt qua
static void getVectorByIndexRec(AVLTree<double, VectorRecord>::AVLNode* node, int& counter, int targetIndex, VectorRecord*& result) {
    if (node == nullptr || result != nullptr) 
        return;

    getVectorByIndexRec(node->pLeft, counter, targetIndex, result);

    if (result != nullptr) return;

    if (counter == targetIndex) {
        result = &(node->data);
        return;
    }
    counter++;

    getVectorByIndexRec(node->pRight, counter, targetIndex, result);
}

VectorRecord* VectorStore::getVector(int index) {
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }

    int counter = 0;
    VectorRecord* result = nullptr;
    
    getVectorByIndexRec(vectorStore->getRoot(), counter, index, result);

    return result;
}

std::string VectorStore::getRawText(int index) {
    VectorRecord* rec = getVector(index);
    return rec->rawText;
}

int VectorStore::getId(int index) {
    VectorRecord* rec = getVector(index);
    return rec->id;
}

bool VectorStore::removeAt(int index) {
    VectorRecord* recToRemove = getVector(index);

    double distToRemove = recToRemove->distanceFromReference;
    double normToRemove = recToRemove->norm;
    int idToRemove = recToRemove->id;
    bool isRoot = (rootVector != nullptr && idToRemove == rootVector->id);

    if (recToRemove->vector != nullptr) {
        delete recToRemove->vector;
        recToRemove->vector = nullptr;
    }

    if (count > 1) {
        averageDistance = (averageDistance * count - distToRemove) / (count - 1);
    } 
    else {
        averageDistance = 0.0;
    }

    vectorStore->remove(distToRemove);
    normIndex->remove(normToRemove);

    count--;

    if (count > 0 && isRoot) {
        std::vector<VectorRecord*> remainingPtrs;
        collectVectors(vectorStore->getRoot(), remainingPtrs);
        
        double minDiff = 1e9; 
        VectorRecord* newRootCandidate = nullptr;

        for (auto ptr : remainingPtrs) {
            double diff = fabs(ptr->distanceFromReference - averageDistance);
            if (diff < minDiff) {
                minDiff = diff;
                newRootCandidate = ptr;
            }
        }

        if (newRootCandidate != nullptr) {
            std::vector<VectorRecord> recordsToRebuild;
            for(auto ptr : remainingPtrs) {
                recordsToRebuild.push_back(*ptr);
            }

            vectorStore->clear();

            vectorStore->insert(newRootCandidate->distanceFromReference, *newRootCandidate);
            
            rootVector = &(vectorStore->getRoot()->data);

            std::vector<VectorRecord> leftList, rightList;
            
            for (const auto& rec : recordsToRebuild) {
                if (rec.id == newRootCandidate->id) continue;

                if (rec.distanceFromReference < rootVector->distanceFromReference) {
                    leftList.push_back(rec);
                } 
                else {
                    rightList.push_back(rec);
                }
            }

            buildBalancedAVL(*vectorStore, leftList, 0, leftList.size() - 1);
            buildBalancedAVL(*vectorStore, rightList, 0, rightList.size() - 1);
        }
    } 
    else if (count == 0) {
        rootVector = nullptr;
    }

    return true;
}



// Các độ đo khoảng cách 
double VectorStore::cosineSimilarity(const std::vector<float>& v1, const std::vector<float>& v2) {
    if (v1.empty() || v1.size() != v2.size()) 
        return 0.0;

    double dotProduct = 0.0;
    double normA = 0.0;
    double normB = 0.0;

    for (size_t i = 0; i < v1.size(); i++) {
        dotProduct += v1[i] * v2[i];
        normA += v1[i] * v1[i];
        normB += v2[i] * v2[i];
    }

    if (normA == 0.0 || normB == 0.0) 
        return 0.0;

    return dotProduct / (sqrt(normA) * sqrt(normB));
}

double VectorStore::l1Distance(const std::vector<float>& v1, const std::vector<float>& v2) {
    if (v1.empty() || v1.size() != v2.size()) 
        return 0.0;

    double sum = 0.0;
    for (size_t i = 0; i < v1.size(); i++) {
        sum += fabs(v1[i] - v2[i]);
    }

    return sum;
}

double VectorStore::l2Distance(const std::vector<float>& v1, const std::vector<float>& v2) {
    if (v1.empty() || v1.size() != v2.size()) 
        return 0.0;

    double sumSq = 0.0;
    for (size_t i = 0; i < v1.size(); i++) {
        double diff = v1[i] - v2[i];
        sumSq += diff * diff;
    }

    return sqrt(sumSq);
}


// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;



