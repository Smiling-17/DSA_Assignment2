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
    this->root = nullptr; // Khởi tạo cây rỗng, độ phức tạp O(1)
}

template <class K, class T>
void AVLTree<K, T>::clearHelper(AVLNode* node) {
    if (node == nullptr) 
        return;
    
    // Duyệt hậu tự (Post-order): Xóa các con trước khi xóa node cha
    clearHelper(node->pLeft);
    clearHelper(node->pRight);
    delete node;
}

template <class K, class T>
void AVLTree<K, T>::clear() {
    clearHelper(this->root); // Xóa toàn bộ node trong cây, độ phức tạp O(n)
    this->root = nullptr;    // Đặt lại root về nullptr để an toàn
}

template <class K, class T>
AVLTree<K, T>::~AVLTree() {
    clear(); // Giải phóng bộ nhớ khi hủy đối tượng, độ phức tạp O(n)
}

template <class K, class T>
bool AVLTree<K, T>::empty() const {
    return (root == nullptr); // Kiểm tra cây rỗng, độ phức tạp O(1)
}

// ----------------------------------------------------
template <class K, class T>
int AVLTree<K, T>::height(AVLNode* node) const {
    if (node == nullptr) 
        return 0; // Node rỗng có chiều cao là 0
    
    // Đệ quy tính chiều cao hai nhánh
    int height_Left = height(node->pLeft);
    int height_Right = height(node->pRight);

    // Tìm chiều cao lớn nhất giữa hai nhánh
    int max_values = 0;
    if (height_Left >= height_Right)
        max_values = height_Left;
    else    
        max_values = height_Right;
    
    return (1 + max_values); // Chiều cao = 1 + max(trái, phải)
}

template <class K, class T>
int AVLTree<K, T>::getHeight() const {
    return height(this->root); // Gọi helper bắt đầu từ gốc, độ phức tạp O(n)
}

// ----------------------------------------------------
template <class K, class T>
int AVLTree<K, T>::size(AVLNode* node) const {
    if (node == nullptr) 
        return 0; // Nút rỗng thì kích thước là 0
    
    // Đệ quy: 1 (nút hiện tại) + kích thước con trái + kích thước con phải
    return (1 + size(node->pLeft) + size(node->pRight));
}

template <class K, class T>
int AVLTree<K, T>::getSize() const {
    return size(this->root); // Gọi helper từ gốc để đếm toàn bộ cây, độ phức tạp O(n)
}

// ----------------------------------------------------
template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::rotateRight(typename AVLTree<K,T>::AVLNode*& node) {
    // Kiểm tra an toàn
    if (node == nullptr || node->pLeft == nullptr) 
        return node;

    AVLNode* newRoot = node->pLeft;      // Node trái lên làm gốc mới
    node->pLeft = newRoot->pRight;       // Chuyển cây con phải của gốc mới sang node cũ
    newRoot->pRight = node;              // Node cũ xuống làm con phải
    
    return newRoot;  // Trả về gốc mới
}

template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::rotateLeft(typename AVLTree<K,T>::AVLNode*& node) {
    // Kiểm tra an toàn
    if (node == nullptr || node->pRight == nullptr) 
        return node;

    AVLNode* newRoot = node->pRight;     // Node phải lên làm gốc mới
    node->pRight = newRoot->pLeft;       // Chuyển cây con trái của gốc mới sang node cũ
    newRoot->pLeft = node;               // Node cũ xuống làm con trái
    
    return newRoot;    // Trả về gốc mới
}

// ----------------------------------------------------
template <class K, class T>
bool AVLTree<K, T>::containsNode(AVLNode* node, const K& key) const {
    if (node == nullptr) 
        return false; // Đã duyệt hết nhánh mà không thấy
    
    if (node->key == key) {
        return true; // Đã tìm thấy khóa
    }
    else if (key < node->key) {
        return containsNode(node->pLeft, key); // Khóa nhỏ hơn -> tìm bên trái
    }
    else {
        return containsNode(node->pRight, key); // Khóa lớn hơn -> tìm bên phải
    }
}

template <class K, class T>
bool AVLTree<K, T>::contains(const K& key) const {
    return containsNode(this->root, key); // Gọi helper từ gốc, độ phức tạp O(log n)
}

// ----------------------------------------------------
template <class K, class T>
void AVLTree<K, T>::inorderHelper(AVLNode* node, void (*action)(const T&)) const {
    if (node == nullptr)
        return;
    
    inorderHelper(node->pLeft, action);  // Duyệt con trái
    action(node->data);                  // Xử lý node hiện tại
    inorderHelper(node->pRight, action); // Duyệt con phải
}

template <class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T&)) const {
    inorderHelper(this->root, action); // Bắt đầu duyệt từ gốc, O(n)
}

// ----------------------------------------------------
template <class K, class T>
void AVLTree<K,T>::insert(const K& key, const T& value) {
    bool taller = false;
    insertNode(this->root, key, value, taller); // Gọi hàm đệ quy chèn, O(log n)
}

template <class K, class T>
bool AVLTree<K,T>::insertNode(AVLNode*& node, const K& key, const T& value, bool& taller) {
    // Trường hợp cơ sở: Thêm node mới
    if (node == nullptr) {
        AVLNode* newNode = new AVLNode(key, value);
        node = newNode;
        taller = true; // Cây cao lên
        return true;
    }

    // Khóa đã tồn tại -> Không chèn
    if (key == node->key) {
        taller = false;
        return false;
    }
    // Chèn vào nhánh trái
    else if (key < node->key) {
        bool inserted = insertNode(node->pLeft, key, value, taller);
        if (inserted == false)
            return false;

        // Cập nhật cân bằng sau khi chèn trái
        if (taller == true) {
            if (node->balance == LH) {
                leftBalance(node, taller); // Mất cân bằng trái -> Xử lý
            }
            else if (node->balance == EH) {
                taller = true;       // Từ cân bằng sang lệch trái -> Cây cao lên
                node->balance = LH;
            }
            else { // node->balance == RH
                node->balance = EH;  // Từ lệch phải sang cân bằng -> Chiều cao không đổi
                taller = false;            
            }
        }

        return true;
    }
    // Chèn vào nhánh phải
    else {
        bool inserted = insertNode(node->pRight, key, value, taller);
        if (inserted == false)
            return false;

        // Cập nhật cân bằng sau khi chèn phải
        if (taller == true) {
            if (node->balance == RH) {
                rightBalance(node, taller); // Mất cân bằng phải -> Xử lý
            }
            else if (node->balance == EH) {
                node->balance = RH;  // Từ cân bằng sang lệch phải -> Cây cao lên
                taller = true;
            }
            else { // node->balance == LH
                node->balance = EH;  // Từ lệch trái sang cân bằng -> Chiều cao không đổi
                taller = false;            
            }
        }

        return true;
    }
}

// ----------------------------------------------------
template <class K, class T>
void AVLTree<K,T>::leftBalance(AVLNode*& node, bool& taller) {
    AVLNode* L = node->pLeft;

    // Trường hợp 1: Mất cân bằng Trái-Trái (Left-Left) -> Quay đơn
    if (L->balance == LH) {
        node = rotateRight(node);
        node->balance = EH;
        node->pRight->balance = EH;
        taller = false; // Cây đã cân bằng, chiều cao giảm
    }
    // Trường hợp 2: Mất cân bằng Trái-Phải (Left-Right) -> Quay kép
    else if (L->balance == RH) {
        AVLNode* LR = L->pRight;

        // Cập nhật chỉ số cân bằng dựa trên node LR
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

        node->pLeft = rotateLeft(node->pLeft); // Quay trái con trái
        node = rotateRight(node);              // Quay phải node hiện tại
        
        node->balance = EH; // Node mới lên làm gốc luôn cân bằng
        taller = false;
    }
}

template <class K, class T>
void AVLTree<K,T>::rightBalance(AVLNode*& node, bool& taller) {
    AVLNode* R = node->pRight;

    // Trường hợp 1: Mất cân bằng Phải-Phải (Right-Right) -> Quay đơn
    if (R->balance == RH) {
        node = rotateLeft(node);
        node->balance = EH;
        node->pLeft->balance = EH;
        taller = false;
    }
    // Trường hợp 2: Mất cân bằng Phải-Trái (Right-Left) -> Quay kép
    else if (R->balance == LH) {
        AVLNode* RL = R->pLeft;

        // Cập nhật chỉ số cân bằng dựa trên node RL
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

        node->pRight = rotateRight(node->pRight); // Quay phải con phải
        node = rotateLeft(node);                  // Quay trái node hiện tại
        
        node->balance = EH;
        taller = false;
    }
}

// ----------------------------------------------------
template <class K, class T>
void AVLTree<K,T>::remove(const K& key) {
    bool shorter = false;
    bool success = false;
    removeNode(this->root, key, shorter, success); // Gọi helper đệ quy, O(log n)
}

template <class K, class T>
bool AVLTree<K,T>::removeNode(AVLNode*& node, const K& key, bool& shorter, bool& success) {
    // 1. Tìm kiếm node cần xóa
    if (node == nullptr) {
        shorter = false;
        success = false;
        return false;
    }

    // Tìm bên trái
    if (key < node->key) {
        bool removed = removeNode(node->pLeft, key, shorter, success);
        if (removed == false) 
            return false;
        
        // Cân bằng lại khi nhánh trái bị ngắn đi
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
    // Tìm bên phải
    else if (key > node->key) {
        bool removed = removeNode(node->pRight, key, shorter, success);
        if (removed == false) 
            return false;
        
        // Cân bằng lại khi nhánh phải bị ngắn đi
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
    // 2. Đã tìm thấy node cần xóa
    else {
        // Trường hợp 1: Node lá hoặc có 1 con
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
        // Trường hợp 2: Node có 2 con
        else {
            // Yêu cầu đề bài: Chọn nút bé nhất bên phải (Successor) để thay thế
            AVLNode* succ = node->pRight; 
            while (succ->pLeft != nullptr) {
                succ = succ->pLeft;
            }

            // Sao chép dữ liệu từ Successor sang node hiện tại
            node->key = succ->key;
            node->data = succ->data;

            // Đệ quy xóa Successor (nằm ở nhánh phải)
            bool dummySuccess = false;
            bool dummyShorter = false;
            removeNode(node->pRight, succ->key, dummyShorter, dummySuccess); 

            success = true;
            shorter = dummyShorter; // Cập nhật trạng thái ngắn đi từ nhánh phải
        }
    }

    // 3. Cân bằng lại sau khi xóa node có 2 con (nhánh phải bị ngắn đi)
    if (shorter == true) {
        if (node->balance == RH) { // Đang lệch phải, mất bên phải -> Cân bằng
            node->balance = EH;
            shorter = true;
        }
        else if (node->balance == EH) { // Đang cân bằng, mất bên phải -> Lệch trái
            node->balance = LH;
            shorter = false;
        }
        else { // Đang lệch trái, mất bên phải -> Mất cân bằng trái -> Xoay trái
            leftBalance(node, shorter);
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
    // Khởi tạo node mới luôn là màu ĐỎ theo quy tắc chèn RBT
}

template <class K, class T>
void RedBlackTree<K,T>::RBTNode::recolorToRed() {
    this->color = RED; // Đổi màu thành Đỏ
}

template <class K, class T>
void RedBlackTree<K,T>::RBTNode::recolorToBlack() {
    this->color = BLACK; // Đổi màu thành Đen
}

// ----------------------------------------------------
template <class K, class T>
static void rbClearHelper(typename RedBlackTree<K,T>::RBTNode* node) {
    if (node == nullptr) 
        return;

    // Duyệt hậu tự: Xóa con trước khi xóa cha
    rbClearHelper<K,T>(node->left);
    rbClearHelper<K,T>(node->right);
    delete node;
}

template <class K, class T>
void RedBlackTree<K,T>::clear() {
    rbClearHelper<K,T>(this->root); // Xóa toàn bộ node, O(n)
    this->root = nullptr;
}

template <class K, class T>
RedBlackTree<K,T>::RedBlackTree() {
    this->root = nullptr; // Khởi tạo cây rỗng, O(1)
}

template <class K, class T>
RedBlackTree<K,T>::~RedBlackTree() {
    clear(); // Giải phóng bộ nhớ, O(n)
}

template <class K, class T>
bool RedBlackTree<K,T>::empty() const {
    return (root == nullptr); // O(1)
}

template <class K, class T>
static int rbSizeHelper(typename RedBlackTree<K,T>::RBTNode* node) {
    if (node == nullptr) 
        return 0;
    // Đệ quy đếm: 1 (gốc) + trái + phải
    return 1 + rbSizeHelper<K,T>(node->left) + rbSizeHelper<K,T>(node->right);
}

template <class K, class T>
int RedBlackTree<K,T>::size() const {
    return rbSizeHelper<K,T>(this->root); // O(n)
}

template <class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::find(const K& key) const {
    RBTNode* curr = this->root;

    // Tìm kiếm nhị phân (Iterative), O(log n)
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
    return (find(key) != nullptr); // Tận dụng find, O(log n)
}

// ----------------------------------------------------
template <class K, class T>
void RedBlackTree<K,T>::rotateLeft(RBTNode* node) {
    RBTNode* pivot = node->right;
    if (pivot == nullptr) 
        return;

    // 1. Chuyển cây con trái của pivot sang phải của node
    node->right = pivot->left;
    if (pivot->left != nullptr) {
        pivot->left->parent = node;
    }

    // 2. Chuyển cha của node sang cho pivot
    pivot->parent = node->parent;
    if (node->parent == nullptr) {
        this->root = pivot;
    }
    else if (node == node->parent->left) {
        node->parent->left = pivot;
    }
    else {
        node->parent->right = pivot;
    }

    // 3. Đặt node làm con trái của pivot
    pivot->left = node;
    node->parent = pivot;
}

template <class K, class T>
void RedBlackTree<K,T>::rotateRight(RBTNode* node) {
    RBTNode* pivot = node->left;
    if (pivot == nullptr) 
        return; 

    // 1. Chuyển cây con phải của pivot sang trái của node
    node->left = pivot->right;
    if (pivot->right != nullptr) {
        pivot->right->parent = node;
    }

    // 2. Chuyển cha của node sang cho pivot
    pivot->parent = node->parent;
    if (node->parent == nullptr) {
        this->root = pivot;
    }
    else if (node == node->parent->left) {
        node->parent->left = pivot;
    }
    else {
        node->parent->right = pivot;
    }

    // 3. Đặt node làm con phải của pivot
    pivot->right = node;
    node->parent = pivot;
}

// ----------------------------------------------------
template <class K, class T>
void RedBlackTree<K,T>::fixInsert(RBTNode* node) {
    // Lặp khi cha còn là màu Đỏ (vi phạm Đỏ-Đỏ)
    while (node != this->root && node->parent->color == RED) {
        RBTNode* parent = node->parent;
        RBTNode* grandparent = parent->parent;

        // Trường hợp A: Cha là con trái của Ông
        if (parent == grandparent->left) {
            RBTNode* uncle = grandparent->right;

            // Case 1: Chú màu Đỏ -> Đổi màu và đẩy lên trên
            if (uncle != nullptr && uncle->color == RED) {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();
                node = grandparent;
            }
            // Case 2 & 3: Chú màu Đen -> Xoay
            else {
                if (node == parent->right) { // Tam giác (Trái-Phải) -> Xoay trái
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                // Đường thẳng (Trái-Trái) -> Đổi màu + Xoay phải
                parent->recolorToBlack();
                grandparent->recolorToRed();
                rotateRight(grandparent);
            }
        }
        // Trường hợp B: Cha là con phải của Ông (Đối xứng)
        else {
            RBTNode* uncle = grandparent->left;

            if (uncle != nullptr && uncle->color == RED) {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();
                node = grandparent;
            }
            else {
                if (node == parent->left) { // Tam giác (Phải-Trái) -> Xoay phải
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                // Đường thẳng (Phải-Phải) -> Đổi màu + Xoay trái
                parent->recolorToBlack();
                grandparent->recolorToRed();
                rotateLeft(grandparent);
            }
        }
    }
    this->root->recolorToBlack(); // Luôn đảm bảo gốc Đen
}

template <class K, class T>
void RedBlackTree<K,T>::insert(const K& key, const T& value) {
    // Chèn vào gốc nếu cây rỗng (Gốc luôn Đen)
    if (this->root == nullptr) {
        this->root = new RBTNode(key, value);
        this->root->recolorToBlack();
        return;
    }

    RBTNode* newNode = new RBTNode(key, value); // Mặc định màu Đỏ

    // Tìm vị trí chèn (như BST)
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
        else { // Khóa trùng, hủy node mới
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

    // Sửa vi phạm màu (nếu có)
    fixInsert(newNode);
}

// ----------------------------------------------------
template <class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::lowerBound(const K& key, bool& found) const {
    RBTNode* curr = this->root;
    RBTNode* result = nullptr; // Lưu kết quả tốt nhất tìm được

    found = false;
    while (curr != nullptr) {
        if (curr->key >= key) { // Thoả mãn điều kiện >=
            result = curr;      // Lưu lại và thử tìm node nhỏ hơn bên trái
            curr = curr->left;
        } 
        else {
            curr = curr->right; // Chưa thoả, tìm số lớn hơn bên phải
        }
    }

    if (result != nullptr) {
        found = true;
    }
    return result;
}

template <class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::upperBound(const K& key, bool& found) const {
    RBTNode* curr = this->root;
    RBTNode* result = nullptr;

    found = false;
    while (curr != nullptr) {
        if (curr->key > key) {  // Thoả mãn điều kiện >
            result = curr;      // Lưu lại và thử tìm node nhỏ hơn (nhưng vẫn > key) bên trái
            curr = curr->left;
        } 
        else {
            curr = curr->right; // Chưa thoả (<=), tìm bên phải
        }
    }

    if (result != nullptr) {
        found = true;
    }
    return result;
}

// Các hàm helper protected thực hiện logic tương tự
template <class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::lowerBoundNode(const K& key) const {
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
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::upperBoundNode(const K& key) const {
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

// ----------------------------------------------------
template <class K, class T>
void RedBlackTree<K,T>::fixDelete(RBTNode* x, RBTNode* parent) {
    // Lặp khi x chưa là gốc và (x đen hoặc null)
    while (x != this->root && (x == nullptr || x->color == BLACK)) {
        if (x == parent->left) {
            RBTNode* sibling = parent->right;
            
            // Case 1: Anh em Đỏ -> Đổi màu + Xoay trái -> Về Case 2/3/4
            if (sibling->color == RED) {
                sibling->recolorToBlack();
                parent->recolorToRed();
                rotateLeft(parent);
                sibling = parent->right;
            }

            // Case 2: Anh em Đen, 2 con Đen -> Đổi màu Anh em, đẩy khó lên trên
            if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling->right == nullptr || sibling->right->color == BLACK)) {
                sibling->recolorToRed();
                x = parent;
                parent = x->parent;
            } 
            else {
                // Case 3: Anh em Đen, con trái Đỏ (gần) -> Xoay phải tại Anh em -> Về Case 4
                if (sibling->right == nullptr || sibling->right->color == BLACK) {
                    if (sibling->left) {
                        sibling->left->recolorToBlack();
                    }
                    sibling->recolorToRed();
                    rotateRight(sibling);
                    sibling = parent->right;
                }

                // Case 4: Anh em Đen, con phải Đỏ (xa) -> Đổi màu + Xoay trái -> Xong
                sibling->color = parent->color;
                parent->recolorToBlack();
                if (sibling->right) {
                    sibling->right->recolorToBlack();
                }
                rotateLeft(parent);
                x = this->root; // Kết thúc
            }
        } 
        else { // Đối xứng (x là con phải)
            RBTNode* sibling = parent->left;
            
            // Case 1
            if (sibling->color == RED) {
                sibling->recolorToBlack();
                parent->recolorToRed();
                rotateRight(parent);
                sibling = parent->left;
            }
            
            // Case 2
            if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                (sibling->left == nullptr || sibling->left->color == BLACK)) {
                sibling->recolorToRed();
                x = parent;
                parent = x->parent;
            }
            else {
                // Case 3
                if (sibling->left == nullptr || sibling->left->color == BLACK) {
                    if (sibling->right) {
                        sibling->right->recolorToBlack();
                    }
                    sibling->recolorToRed();
                    rotateLeft(sibling);
                    sibling = parent->left;
                }
                // Case 4
                sibling->color = parent->color;
                parent->recolorToBlack();
                if (sibling->left) {
                    sibling->left->recolorToBlack();
                }
                rotateRight(parent);
                x = this->root;
            }
        }
    }

    if (x != nullptr) {
        x->recolorToBlack();
    }
}

template <class K, class T>
void RedBlackTree<K,T>::remove(const K& key) {
    // Tìm node cần xóa
    RBTNode* node = find(key);
    if (node == nullptr) 
        return;
    
    RBTNode* x = nullptr; // Node con thay thế vị trí node bị xóa
    RBTNode* xParent = nullptr;
    Color deletedColor = node->color;

    // Trường hợp 1: Node có ít hơn 2 con
    if (node->left == nullptr || node->right == nullptr) {
        RBTNode* child = (node->left != nullptr) ? node->left : node->right;
        
        // Nối cha với con
        if (child != nullptr) {
            child->parent = node->parent;
        }

        if (node->parent == nullptr) {
            this->root = child;
        }
        else if (node == node->parent->left) {
            node->parent->left = child;
        }
        else {
            node->parent->right = child;
        }

        // Nếu xóa node đen -> Mất cân bằng đen
        if (node->color == BLACK) {
            // Nếu con là đỏ -> Tô đen con là xong
            if (child != nullptr && child->color == RED) {
                child->recolorToBlack();
                deletedColor = RED; // Coi như đã xử lý
            }
        }

        x = child;
        xParent = node->parent; // Lưu cha để fix
        delete node;
    }
    // Trường hợp 2: Node có 2 con -> Dùng Predecessor (lớn nhất bên trái)
    else {
        RBTNode* pred = node->left;
        while (pred->right != nullptr) {
            pred = pred->right;
        }

        // Copy dữ liệu
        node->key = pred->key;
        node->data = pred->data;
        deletedColor = pred->color;

        // Xóa node thế mạng (pred)
        RBTNode* child = pred->left;
        if (child != nullptr) {
            child->parent = pred->parent;
        }

        if (pred->parent == node) {
            pred->parent->left = child; // Đặc biệt: pred là con trực tiếp
        }
        else {
            pred->parent->right = child;
        }
        
        if (pred->color == BLACK && child != nullptr && child->color == RED) {
            child->recolorToBlack();
            deletedColor = RED;
        }

        x = child;
        xParent = pred->parent; // Cha của node thực sự bị xóa
        delete pred;
    }

    // Nếu mất một node đen thực sự -> Gọi fixDelete
    if (deletedColor == BLACK) {
        // Lưu ý: fixDelete xử lý cả trường hợp x là nullptr (nhưng cần xParent)
        fixDelete(x, xParent);
    }
}



// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
ostream& operator<<(ostream& os, const VectorRecord& record) {
    os << record.id;
    return os;
}

// =====================================
// VectorStore implementation
// =====================================

//TODO: Implement all VectorStore methods here
// Khởi tạo và quản lý cơ bản
VectorStore::VectorStore(int dimension,
                    vector<float>* (*embeddingFunction)(const string&),
                    const vector<float>& referenceVector) 
    : dimension(dimension), embeddingFunction(embeddingFunction), count(0), averageDistance(0.0f), rootVector(nullptr) {
    
    this->vectorStore = new AVLTree<double, VectorRecord>();
    this->normIndex = new RedBlackTree<double, VectorRecord>();
    
    // Deep copy (sao chép sâu) reference vector để quản lý bộ nhớ độc lập
    this->referenceVector = new vector<float>(referenceVector);
}

VectorStore::~VectorStore() {
    clear(); // Xóa dữ liệu bên trong trước

    // Giải phóng các cấu trúc quản lý
    if (vectorStore != nullptr) 
        delete vectorStore;
    
    if (normIndex != nullptr) 
        delete normIndex;
    
    if (referenceVector) 
        delete referenceVector;
}

int VectorStore::size() {
    return this->count;  // Độ phức tạp O(1)
}

bool VectorStore::empty() {
    return count == 0;  // Độ phức tạp O(1)
}

static void deleteVectorDataRec(AVLTree<double, VectorRecord>::AVLNode* node) {
    if (node == nullptr) 
        return;

    // Duyệt hậu tự (Post-order): Xóa con trước khi xóa node hiện tại
    deleteVectorDataRec(node->pLeft);    
    deleteVectorDataRec(node->pRight);

    // Giải phóng vùng nhớ heap của vector dữ liệu
    if (node->data.vector != nullptr) {
        delete node->data.vector;   
        node->data.vector = nullptr; // Gán nullptr để tránh lỗi double free
    }
}

void VectorStore::clear() {
    if (vectorStore != nullptr) {
        deleteVectorDataRec(vectorStore->getRoot()); // Xóa dữ liệu vector thông qua AVL
        this->vectorStore->clear(); // Xóa cấu trúc node AVL
    }

    if (normIndex != nullptr) {
        this->normIndex->clear(); // Chỉ cần xóa cấu trúc node RBT (dữ liệu đã xóa ở trên)
    }

    // Reset trạng thái về ban đầu
    count = 0;
    averageDistance = 0.0;
    rootVector = nullptr;
}

// ----------------------------------------------------
// Tiền xử lý và quản lý dữ liệu
// Hàm hoán đổi thủ công
static void manualSwap(VectorRecord& a, VectorRecord& b) {
    VectorRecord temp = a; 
    a = b; 
    b = temp;
}

// Phân hoạch QuickSort (tăng dần theo distance)
static int partition(vector<VectorRecord>& arr, int low, int high) {
    double pivot = arr[high].distanceFromReference;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].distanceFromReference < pivot) {
            i++;
            manualSwap(arr[i], arr[j]);
        }
    }

    manualSwap(arr[i + 1], arr[high]);
    return (i + 1);
}

// QuickSort đệ quy
static void myQuickSort(vector<VectorRecord>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        myQuickSort(arr, low, pi - 1);
        myQuickSort(arr, pi + 1, high);
    }
}

vector<float>* VectorStore::preprocessing(string rawText) {
    vector<float>* vec = embeddingFunction(rawText);
    if (vec == nullptr) {
        return new vector<float>(dimension, 0.0f); // Safety check
    }

    // Resize đúng dimension
    if ((int)vec->size() > this->dimension) {
        vec->resize(this->dimension);
    }
    else if ((int)vec->size() < this->dimension) {
        vec->resize(this->dimension, 0.0f);
    }

    return vec;
}

// Helper: Lấy toàn bộ vector (In-order)
static void collectVectors(AVLTree<double, VectorRecord>::AVLNode* node, vector<VectorRecord*>& list) {
    if (node == nullptr) 
        return;

    collectVectors(node->pLeft, list);
    list.push_back(&(node->data));
    collectVectors(node->pRight, list);
}

// Helper: Tìm ID lớn nhất
static int findMaxId(AVLTree<double, VectorRecord>::AVLNode* node) {
    if (node == nullptr) 
        return -1;

    int maxLeft = findMaxId(node->pLeft);
    int maxRight = findMaxId(node->pRight);
    int currentMax = node->data.id;

    if (maxLeft > currentMax) {
        currentMax = maxLeft;
    }
    if (maxRight > currentMax) {
        currentMax = maxRight;
    }

    return currentMax;
}

// Helper: Dựng cây cân bằng từ danh sách đã sort
void buildBalancedAVL(AVLTree<double, VectorRecord>& tree, const vector<VectorRecord>& sortedRecords, int left, int right) {
    if (left > right) 
        return;

    int mid = left + (right - left) / 2;
    tree.insert(sortedRecords[mid].distanceFromReference, sortedRecords[mid]);

    buildBalancedAVL(tree, sortedRecords, left, mid - 1);
    buildBalancedAVL(tree, sortedRecords, mid + 1, right);
}

void VectorStore::addText(string rawText) {
    vector<float>* vec = preprocessing(rawText);
    double distance = l2Distance(*vec, *referenceVector);

    // Cập nhật Avg Distance
    if (this->count == 0) {
        this->averageDistance = distance;
    }
    else {
        this->averageDistance = (this->averageDistance * this->count + distance) / (this->count + 1);
    }

    // Tính Norm
    double norm = 0.0;
    for (size_t i = 0; i < vec->size(); i++) {
        norm += (*vec)[i] * (*vec)[i];
    }
    norm = sqrt(norm);

    // Tạo ID mới
    int newId = 0;
    if (this->count > 0 && vectorStore->getRoot() != nullptr) {
        newId = findMaxId(vectorStore->getRoot()) + 1;
    }

    VectorRecord newRecord(newId, rawText, vec, distance);
    newRecord.norm = norm; 
    
    // Case 1: Store rỗng
    if (this->count == 0) {
        vectorStore->insert(distance, newRecord);
        normIndex->insert(norm, newRecord);
        rootVector = &(vectorStore->getRoot()->data);
        this->count++;
        return;
    }

    normIndex->insert(norm, newRecord); // Luôn insert RBT

    // Rebuild nếu vector mới gần Avg hơn Root cũ
    double distNewToAvg = fabs(distance - this->averageDistance);
    double distRootToAvg = fabs(rootVector->distanceFromReference - this->averageDistance);

    if (distNewToAvg < distRootToAvg) {
        vector<VectorRecord*> existingPtrs;
        collectVectors(vectorStore->getRoot(), existingPtrs);
        
        vector<VectorRecord> allRecords; // Copy dữ liệu
        for (auto ptr : existingPtrs) {
            allRecords.push_back(*ptr);
        }
        allRecords.push_back(newRecord); 
        
        vectorStore->clear(); // Xóa cây cũ
        
        vectorStore->insert(newRecord.distanceFromReference, newRecord); // Insert Root mới
        rootVector = &(vectorStore->getRoot()->data);
        
        vector<VectorRecord> leftList, rightList;
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
        
        // Sort thủ công và dựng lại cây
        if (!leftList.empty()) {
            myQuickSort(leftList, 0, leftList.size() - 1);
        }
        if (!rightList.empty()) {
            myQuickSort(rightList, 0, rightList.size() - 1);
        }
        
        buildBalancedAVL(*vectorStore, leftList, 0, leftList.size() - 1);
        buildBalancedAVL(*vectorStore, rightList, 0, rightList.size() - 1);
    } 
    else {
        vectorStore->insert(distance, newRecord); // Insert thường
    }
    count++;
}

// Helper: Duyệt In-order để tìm phần tử thứ k
static void getVectorByIndexRec(AVLTree<double, VectorRecord>::AVLNode* node, int& counter, int targetIndex, VectorRecord*& result) {
    if (node == nullptr || result != nullptr) 
        return;

    getVectorByIndexRec(node->pLeft, counter, targetIndex, result);
    if (result != nullptr) 
        return;

    if (counter == targetIndex) { 
        result = &(node->data); 
        return; 
    }
    counter++;
    getVectorByIndexRec(node->pRight, counter, targetIndex, result);
}

VectorRecord* VectorStore::getVector(int index) {
    if (index < 0 || index >= count) 
        throw out_of_range("Index is invalid!");

    int counter = 0;
    VectorRecord* result = nullptr;
    getVectorByIndexRec(vectorStore->getRoot(), counter, index, result);
    return result;
}

string VectorStore::getRawText(int index) {
    return getVector(index)->rawText;
}

int VectorStore::getId(int index) {
    return getVector(index)->id;
}

bool VectorStore::removeAt(int index) {
    VectorRecord* recToRemove = getVector(index); // Lấy vector cần xóa
    double distKey = recToRemove->distanceFromReference;
    double normKey = recToRemove->norm;
    int idToRemove = recToRemove->id;
    bool isRoot = (rootVector != nullptr && idToRemove == rootVector->id);

    // Giải phóng bộ nhớ heap của vector
    if (recToRemove->vector != nullptr) {
        delete recToRemove->vector;
        recToRemove->vector = nullptr;
    }

    // Cập nhật khoảng cách trung bình
    if (count > 1) {
        this->averageDistance = (this->averageDistance * this->count - distKey) / (this->count - 1);
    }
    else {
        averageDistance = 0.0;
    }

    vectorStore->remove(distKey);
    normIndex->remove(normKey);
    count--;

    // Rebuild cây nếu xóa mất Root
    if (count > 0 && isRoot) {
        vector<VectorRecord*> remainingPtrs;
        collectVectors(vectorStore->getRoot(), remainingPtrs); // Lấy list đã sort
        
        double minDiff = 1e9; 
        VectorRecord* newRootCandidate = nullptr;
        for (auto ptr : remainingPtrs) { // Tìm Root mới gần Avg nhất
            double diff = fabs(ptr->distanceFromReference - this->averageDistance);
            if (diff < minDiff) { 
                minDiff = diff; 
                newRootCandidate = ptr; 
            }
        }

        if (newRootCandidate != nullptr) {
            vector<VectorRecord> recordsToRebuild;
            for(auto ptr : remainingPtrs) {
                recordsToRebuild.push_back(*ptr); // Copy dữ liệu
            }

            vectorStore->clear(); // Xóa cây cũ
            
            vectorStore->insert(newRootCandidate->distanceFromReference, *newRootCandidate); // Insert Root mới
            rootVector = &(vectorStore->getRoot()->data);

            vector<VectorRecord> leftList, rightList;
            for (const auto& rec : recordsToRebuild) {
                if (rec.id == newRootCandidate->id) 
                    continue;
                if (rec.distanceFromReference < rootVector->distanceFromReference) {
                    leftList.push_back(rec);
                }
                else {
                    rightList.push_back(rec);
                }
            }
            
            // Dựng lại cây cân bằng (không cần sort vì remainingPtrs đã có thứ tự)
            buildBalancedAVL(*vectorStore, leftList, 0, leftList.size() - 1);
            buildBalancedAVL(*vectorStore, rightList, 0, rightList.size() - 1);
        }
    } 
    else if (count == 0) {
        rootVector = nullptr;
    }
    return true;
}

// ----------------------------------------------------
// Quản lý Reference Vector và hàm nhúng
void VectorStore::setReferenceVector(const vector<float>& newReference) {
    // Cập nhật reference vector mới (xóa cũ trước để tránh leak)
    if (this->referenceVector != nullptr) {
        delete this->referenceVector;
    }
    this->referenceVector = new vector<float>(newReference);

    if (count == 0) 
        return;

    // Bước 1: Tính lại khoảng cách cho toàn bộ vector dựa trên reference mới
    vector<VectorRecord*> ptrs;
    collectVectors(vectorStore->getRoot(), ptrs);

    double totalDist = 0.0;
    for (auto ptr : ptrs) {
        ptr->distanceFromReference = l2Distance(*(ptr->vector), *this->referenceVector);
        totalDist += ptr->distanceFromReference;
    }

    // Bước 2: Xác định nút gốc (root vector) mới gần averageDistance nhất
    this->averageDistance = totalDist / this->count;

    VectorRecord* bestCandidatePtr = nullptr;
    double minDiff = 1e9; 

    for (auto ptr : ptrs) {
        double diff = fabs(ptr->distanceFromReference - this->averageDistance);
        if (diff < minDiff) {
            minDiff = diff;
            bestCandidatePtr = ptr;
        }
    }

    // Bước 3: Tạo bản sao dữ liệu và sắp xếp tăng dần theo distance
    vector<VectorRecord> allRecords;
    int rootId = (bestCandidatePtr != nullptr) ? bestCandidatePtr->id : -1;

    for (auto ptr : ptrs) {
        allRecords.push_back(*ptr);
    }

    if (!allRecords.empty()) {
        myQuickSort(allRecords, 0, allRecords.size() - 1);
    }

    // Bước 4: Xây dựng lại cây AVL
    vectorStore->clear(); // Xóa cấu trúc cây cũ

    // Tìm lại object Root trong danh sách đã sort
    VectorRecord rootObj;
    bool foundRoot = false;
    for (const auto& rec : allRecords) {
        if (rec.id == rootId) {
            rootObj = rec;
            foundRoot = true;
            break;
        }
    }

    if (foundRoot) {
        // Insert Root mới đầu tiên để làm gốc cây
        vectorStore->insert(rootObj.distanceFromReference, rootObj);
        this->rootVector = &(vectorStore->getRoot()->data);

        // Chia danh sách thành 2 nửa: Nhỏ hơn Root và Lớn hơn Root 
        vector<VectorRecord> leftList, rightList;
        for (const auto& rec : allRecords) {
            if (rec.id == rootId) 
                continue; 

            if (rec.distanceFromReference < this->rootVector->distanceFromReference) {
                leftList.push_back(rec);
            } 
            else {
                rightList.push_back(rec);
            }
        }

        // Dựng cây cân bằng đệ quy cho 2 nhánh
        buildBalancedAVL(*vectorStore, leftList, 0, leftList.size() - 1);
        buildBalancedAVL(*vectorStore, rightList, 0, rightList.size() - 1);
    }
}

vector<float>* VectorStore::getReferenceVector() const {
    return this->referenceVector;
}

VectorRecord* VectorStore::getRootVector() const {
    return this->rootVector;
}

double VectorStore::getAverageDistance() const {
    return this->averageDistance;
}

void VectorStore::setEmbeddingFunction(vector<float>* (*newEmbeddingFunction)(const string&)) {
    this->embeddingFunction = newEmbeddingFunction;
}

// ----------------------------------------------------
// Duyệt và Lặp 
// Helper: Duyệt In-order để đảm bảo thứ tự khoảng cách tăng dần
static void forEachRecursive(AVLTree<double, VectorRecord>::AVLNode* node, 
                             void (*action)(vector<float>&, int, string&)) {
    if (node == nullptr) 
        return;

    forEachRecursive(node->pLeft, action);
    // Thực thi action trên dữ liệu của node hiện tại
    action(*(node->data.vector), node->data.id, node->data.rawText);
    forEachRecursive(node->pRight, action);
}

void VectorStore::forEach(void (*action)(vector<float>&, int, string&)) {
    forEachRecursive(vectorStore->getRoot(), action); // O(n)
}

vector<int> VectorStore::getAllIdsSortedByDistance() const {
    vector<int> idList;
    vector<VectorRecord*> sortedRecords;
    
    // Tận dụng collectVectors để lấy danh sách đã sắp xếp
    collectVectors(vectorStore->getRoot(), sortedRecords);

    idList.reserve(sortedRecords.size()); // Tối ưu cấp phát
    for (const auto& rec : sortedRecords) {
        idList.push_back(rec->id);
    }

    return idList;
}

vector<VectorRecord*> VectorStore::getAllVectorsSortedByDistance() const {
    vector<VectorRecord*> sortedRecords;
    collectVectors(vectorStore->getRoot(), sortedRecords); // O(n)

    return sortedRecords;
}

// ----------------------------------------------------
// Các độ đo khoảng cách
double VectorStore::cosineSimilarity(const vector<float>& v1, const vector<float>& v2) {
    if (v1.empty() || v1.size() != v2.size()) 
        return 0.0;

    double dotProduct = 0.0, normA = 0.0, normB = 0.0;

    // Tính tích vô hướng và độ dài vector, O(d)
    for (size_t i = 0; i < v1.size(); i++) {
        dotProduct += v1[i] * v2[i];
        normA += v1[i] * v1[i];
        normB += v2[i] * v2[i];
    }

    // Tránh chia cho 0
    if (normA == 0.0 || normB == 0.0) 
        return 0.0;

    return dotProduct / (sqrt(normA) * sqrt(normB));
}

double VectorStore::l1Distance(const vector<float>& v1, const vector<float>& v2) {
    if (v1.empty() || v1.size() != v2.size()) 
        return 0.0;

    double sum = 0.0;
    // Khoảng cách Manhattan: Tổng trị tuyệt đối hiệu các chiều, O(d)
    for (size_t i = 0; i < v1.size(); i++) {
        sum += fabs(v1[i] - v2[i]);
    }

    return sum;
}

double VectorStore::l2Distance(const vector<float>& v1, const vector<float>& v2) {
    if (v1.empty() || v1.size() != v2.size()) 
        return 0.0;

    double sumSq = 0.0;
    // Khoảng cách Euclidean: Căn bậc hai của tổng bình phương hiệu, O(d)
    for (size_t i = 0; i < v1.size(); i++) {
        double diff = v1[i] - v2[i];
        sumSq += diff * diff;
    }

    return sqrt(sumSq);
}

// ----------------------------------------------------
// Ước lượng ngưỡng D từ k
double VectorStore::estimateD_Linear(const vector<float>& query, int k, double averageDistance,
                                     const vector<float>& reference, double c0_bias, double c1_slope) {

    double dr = l2Distance(query, reference); // Tính dr = ||query - reference||
    double absDiff = fabs(dr - averageDistance); // Tính |dr - avg|
    
    // Áp dụng công thức tuyến tính: D = |dr - avg| + c1 * avg * k + c0
    double D = absDiff + (c1_slope * averageDistance * k) + c0_bias;

    return D;
}

// ----------------------------------------------------
// Tìm kiếm láng giềng gần nhất
// Struct lưu kết quả tạm thời (thay thế pair)
struct Candidate {
    int id;
    double score; 
};

// Helper: Hoán đổi 2 Candidate
static void swapCandidate(Candidate& a, Candidate& b) {
    Candidate temp = a; 
    a = b; 
    b = temp;
}

// Helper: Phân hoạch QuickSort cho Candidate
static int partitionCandidate(vector<Candidate>& arr, int low, int high, bool isAscending) {
    double pivot = arr[high].score;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        bool condition = isAscending ? (arr[j].score < pivot) : (arr[j].score > pivot);
        if (condition) {
            i++;
            swapCandidate(arr[i], arr[j]);
        }
    }
    swapCandidate(arr[i + 1], arr[high]);
    return (i + 1);
}

// Helper: QuickSort Candidate
static void sortCandidates(vector<Candidate>& arr, int low, int high, bool isAscending) {
    if (low < high) {
        int pi = partitionCandidate(arr, low, high, isAscending);
        sortCandidates(arr, low, pi - 1, isAscending);
        sortCandidates(arr, pi + 1, high, isAscending);
    }
}

// Tìm 1 láng giềng gần nhất (Brute-force O(N*d))
int VectorStore::findNearest(const vector<float>& query, string metric) {
    if (metric != "cosine" && metric != "euclidean" && metric != "manhattan") 
        throw invalid_metric();
    if (this->count == 0) 
        return -1; 

    vector<VectorRecord*> allRecords;
    collectVectors(vectorStore->getRoot(), allRecords);

    int bestId = -1;
    // Cosine: Tìm Max Similarity. L1/L2: Tìm Min Distance.
    double bestVal = (metric == "cosine") ? -2.0 : 1e9;
    
    for (auto rec : allRecords) {
        double val;
        if (metric == "cosine") {
            val = cosineSimilarity(query, *(rec->vector));
        }
        else if (metric == "euclidean") {
            val = l2Distance(query, *(rec->vector));
        }
        else {
            val = l1Distance(query, *(rec->vector));
        }

        bool better = (metric == "cosine") ? (val > bestVal) : (val < bestVal);
        if (better) {
            bestVal = val;
            bestId = rec->id;
        }
    }
    return bestId; 
}

// Helper: Tìm kiếm phạm vi trên RBT (Lọc theo Norm)
static void searchRBTRange(RedBlackTree<double, VectorRecord>::RBTNode* node, 
                           double minNorm, double maxNorm, 
                           vector<VectorRecord*>& results) {
    if (node == nullptr) 
        return;

    // Pruning: Chỉ duyệt nhánh có khả năng chứa kết quả
    if (node->key >= minNorm) {
        searchRBTRange(node->left, minNorm, maxNorm, results);
    }
    if (node->key >= minNorm && node->key <= maxNorm) {
        results.push_back(&(node->data));
    }
    if (node->key <= maxNorm) {
        searchRBTRange(node->right, minNorm, maxNorm, results);
    }
}

// Tìm Top-K láng giềng (Lọc RBT -> Tính điểm -> Sort)
int* VectorStore::topKNearest(const vector<float>& query, int k, string metric) {
    if (metric != "cosine" && metric != "euclidean" && metric != "manhattan") 
        throw invalid_metric();
    if (k <= 0 || k > count) 
        throw invalid_k_value();

    // 1. Tính chuẩn query và bán kính D
    double nq = 0.0;
    for (float val : query) {
        nq += val * val;
    }
    nq = sqrt(nq);
    double D = estimateD_Linear(query, k, this->averageDistance, *this->referenceVector);

    // 2. Lọc ứng viên qua RBT
    vector<VectorRecord*> candidates;
    searchRBTRange(normIndex->root, nq - D, nq + D, candidates); 

    int m = (int)candidates.size();
    cout << "Value m: " << m << endl; // Yêu cầu đề bài
    if (m == 0) {
        return new int[0];
    }

    // 3. Tính điểm thật
    vector<Candidate> scoredCandidates;
    bool isCosine = (metric == "cosine");

    for (auto rec : candidates) {
        double score;
        if (isCosine) {
            score = cosineSimilarity(query, *(rec->vector));
        }
        else if (metric == "euclidean") {
            score = l2Distance(query, *(rec->vector));
        }
        else {
            score = l1Distance(query, *(rec->vector));
        }
        
        scoredCandidates.push_back({rec->id, score});
    }

    // 4. Sắp xếp và chọn Top-K
    if (!scoredCandidates.empty()) {
        sortCandidates(scoredCandidates, 0, scoredCandidates.size() - 1, !isCosine);
    }

    int resultSize = (m < k) ? m : k;
    int* result = new int[resultSize];
    for (int i = 0; i < resultSize; i++) {
        result[i] = scoredCandidates[i].id;
    }

    return result; 
}

// ----------------------------------------------------
// Truy vấn phạm vi 
// Helper: Duyệt cây tìm range, tỉa nhánh thông minh
static void searchAVLRange(AVLTree<double, VectorRecord>::AVLNode* node, double minDist, double maxDist, vector<int>& results) {
    if (node == nullptr) 
        return;

    if (node->key >= minDist) {
        searchAVLRange(node->pLeft, minDist, maxDist, results);
    }
    if (node->key >= minDist && node->key <= maxDist) {
        results.push_back(node->data.id);
    }
    if (node->key <= maxDist) {
        searchAVLRange(node->pRight, minDist, maxDist, results);
    }
}

int* VectorStore::rangeQueryFromRoot(double minDist, double maxDist) const {
    vector<int> matchingIds;
    searchAVLRange(vectorStore->getRoot(), minDist, maxDist, matchingIds); // O(k + logN)

    if (matchingIds.empty()) {
        return new int[0];
    }
    
    int* arr = new int[matchingIds.size()];
    for (size_t i = 0; i < matchingIds.size(); i++) {
        arr[i] = matchingIds[i];
    }
    return arr;
}

int* VectorStore::rangeQuery(const vector<float>& query, double radius, string metric) const {
    if (metric != "cosine" && metric != "euclidean" && metric != "manhattan") 
        throw invalid_metric();

    vector<int> matchingIds;
    vector<VectorRecord*> allRecords;
    collectVectors(vectorStore->getRoot(), allRecords);

    bool isCosine = (metric == "cosine");
    for (auto rec : allRecords) {
        double val;
        if (isCosine) {
            val = const_cast<VectorStore*>(this)->cosineSimilarity(query, *(rec->vector));
        }
        else if (metric == "euclidean") {
            val = const_cast<VectorStore*>(this)->l2Distance(query, *(rec->vector));
        }
        else {
            val = const_cast<VectorStore*>(this)->l1Distance(query, *(rec->vector));
        }

        // Cosine: Sim >= Radius. Distance: Val <= Radius
        if ((isCosine && val >= radius) || (!isCosine && val <= radius)) {
            matchingIds.push_back(rec->id);
        }
    }

    if (matchingIds.empty()) {
        return new int[0];
    }
    int* arr = new int[matchingIds.size()];
    for (size_t i = 0; i < matchingIds.size(); i++) {
        arr[i] = matchingIds[i];
    }

    return arr;
}

int* VectorStore::boundingBoxQuery(const vector<float>& minBound, const vector<float>& maxBound) const {
    vector<int> matchingIds;
    vector<VectorRecord*> allRecords;
    collectVectors(vectorStore->getRoot(), allRecords);

    for (auto rec : allRecords) {
        bool inside = true;
        const vector<float>& v = *(rec->vector);
        for (int i = 0; i < dimension; i++) {
            if (v[i] < minBound[i] || v[i] > maxBound[i]) { 
                inside = false; 
                break; 
            }
        }
        if (inside) {
            matchingIds.push_back(rec->id);
        }
    }

    if (matchingIds.empty()) {
        return new int[0];
    }
    int* arr = new int[matchingIds.size()];
    for (size_t i = 0; i < matchingIds.size(); i++) {
        arr[i] = matchingIds[i];
    }

    return arr;
}

double VectorStore::getMaxDistance() const {
    if (this->count == 0) 
        return -1.0;

    AVLTree<double, VectorRecord>::AVLNode* curr = vectorStore->getRoot();
    while (curr && curr->pRight) {
        curr = curr->pRight; // Max là phải cùng
    }

    return curr ? curr->key : 0.0;
}

double VectorStore::getMinDistance() const {
    if (this->count == 0) 
        return -1.0;

    AVLTree<double, VectorRecord>::AVLNode* curr = vectorStore->getRoot();
    while (curr && curr->pLeft) {
        curr = curr->pLeft; // Min là trái cùng
    }

    return curr ? curr->key : 0.0;
}

VectorRecord VectorStore::computeCentroid(const vector<VectorRecord*>& records) const {
    if (records.empty()) {
        VectorRecord res; res.vector = new vector<float>(this->dimension, 0.0f);
        return res;
    }
    vector<float>* centerVec = new vector<float>(this->dimension, 0.0f);
    
    for (auto rec : records) {
        if (rec->vector) {
            for (int i = 0; i < this->dimension; i++) {
                (*centerVec)[i] += (*(rec->vector))[i];
            }
        }
    }
    for (int i = 0; i < this->dimension; i++) {
        (*centerVec)[i] /= records.size();
    }
    
    VectorRecord res; 
    res.vector = centerVec;

    return res;
}

VectorRecord* VectorStore::findVectorNearestToDistance(double targetDistance) const {
    if (this->count == 0) 
        return nullptr;
    AVLTree<double, VectorRecord>::AVLNode* curr = vectorStore->getRoot();
    AVLTree<double, VectorRecord>::AVLNode* closestNode = curr;
    double minDiff = 1e9; 
    
    while (curr) { // Binary Search trên cây AVL
        double diff = fabs(curr->key - targetDistance);
        if (diff < minDiff) { 
            minDiff = diff; 
            closestNode = curr; 
        }
        if (diff == 0) 
            break;
        curr = (targetDistance < curr->key) ? curr->pLeft : curr->pRight;
    }

    return closestNode ? &(closestNode->data) : nullptr;
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
