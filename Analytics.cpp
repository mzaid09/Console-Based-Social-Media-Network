/*
 * File     : Analytics.cpp
 * Module   : F — Analytics & Ranking (AVL Tree x2)
 * Purpose  : Implements both AVL trees (user activity + post likes).
 *            All four rotation types are implemented:
 *              LL (Right rotation), RR (Left rotation),
 *              LR (Left-Right), RL (Right-Left).
 * Member   : Member 3
 */

#include "Analytics.h"

// Define the two AVL tree root pointers
AVLNode* userAVLRoot = nullptr;
AVLNode* postAVLRoot = nullptr;

/* ══════════════════════════════════════════════════════════════
 * avlHeight
 * Returns the height of a node. Returns 0 for nullptr.
 * Height is stored in the node to avoid recomputing.
 ══════════════════════════════════════════════════════════════ */
int avlHeight(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

/* ══════════════════════════════════════════════════════════════
 * avlGetBalance
 * Balance Factor = height(left) - height(right)
 * Valid range: -1, 0, +1. Outside this triggers rotation.
 ══════════════════════════════════════════════════════════════ */
int avlGetBalance(AVLNode* node) {
    if (node == nullptr) return 0;
    return avlHeight(node->left) - avlHeight(node->right);
}

/* ══════════════════════════════════════════════════════════════
 * avlRotateRight (LL Case)
 * Called when left subtree is too heavy (balance > +1).
 *      y                x
 *     / \             /   \
 *    x   T3    →    T1     y
 *   / \                   / \
 *  T1  T2               T2  T3
 ══════════════════════════════════════════════════════════════ */
AVLNode* avlRotateRight(AVLNode* y) {
    AVLNode* x  = y->left;
    AVLNode* T2 = x->right;

    // perform rotation
    x->right = y;
    y->left  = T2;

    // update heights (y first because it is now lower)
    y->height = max(avlHeight(y->left), avlHeight(y->right)) + 1;
    x->height = max(avlHeight(x->left), avlHeight(x->right)) + 1;

    return x;  // x is the new root of this subtree
}

/* ══════════════════════════════════════════════════════════════
 * avlRotateLeft (RR Case)
 * Called when right subtree is too heavy (balance < -1).
 *    x                  y
 *   / \               /   \
 *  T1   y    →       x    T3
 *      / \          / \
 *     T2  T3       T1  T2
 ══════════════════════════════════════════════════════════════ */
AVLNode* avlRotateLeft(AVLNode* x) {
    AVLNode* y  = x->right;
    AVLNode* T2 = y->left;

    // perform rotation
    y->left  = x;
    x->right = T2;

    // update heights
    x->height = max(avlHeight(x->left), avlHeight(x->right)) + 1;
    y->height = max(avlHeight(y->left), avlHeight(y->right)) + 1;

    return y;  // y is new root
}

/* ══════════════════════════════════════════════════════════════
 * avlInsert
 * Standard BST insert followed by balance check and rotation.
 * Uses string key comparison for BST ordering.
 ══════════════════════════════════════════════════════════════ */
AVLNode* avlInsert(AVLNode* root, string key, int value) {
    // Step 1: normal BST insert
    if (root == nullptr) {
        AVLNode* newNode = new AVLNode();
        newNode->key     = key;
        newNode->value   = value;
        return newNode;
    }

    if (key < root->key)
        root->left  = avlInsert(root->left,  key, value);
    else if (key > root->key)
        root->right = avlInsert(root->right, key, value);
    else {
        root->value = value;  // key exists — update value
        return root;
    }

    // Step 2: update height of this ancestor node
    root->height = max(avlHeight(root->left), avlHeight(root->right)) + 1;

    // Step 3: get balance factor to check if rotation is needed
    int balance = avlGetBalance(root);

    // LL Case — right rotation
    if (balance > 1 && key < root->left->key)
        return avlRotateRight(root);

    // RR Case — left rotation
    if (balance < -1 && key > root->right->key)
        return avlRotateLeft(root);

    // LR Case — left rotate left child, then right rotate root
    if (balance > 1 && key > root->left->key) {
        root->left = avlRotateLeft(root->left);
        return avlRotateRight(root);
    }

    // RL Case — right rotate right child, then left rotate root
    if (balance < -1 && key < root->right->key) {
        root->right = avlRotateRight(root->right);
        return avlRotateLeft(root);
    }

    return root;  // balanced, no rotation needed
}

/* ══════════════════════════════════════════════════════════════
 * avlMinNode (internal helper for delete)
 * Returns the node with the smallest key in a subtree.
 ══════════════════════════════════════════════════════════════ */
static AVLNode* avlMinNode(AVLNode* node) {
    AVLNode* curr = node;
    while (curr->left != nullptr)
        curr = curr->left;
    return curr;
}

/* ══════════════════════════════════════════════════════════════
 * avlDelete
 * Removes a node by key. Rebalances after deletion.
 ══════════════════════════════════════════════════════════════ */
AVLNode* avlDelete(AVLNode* root, string key) {
    if (root == nullptr) return root;

    // Step 1: standard BST delete
    if (key < root->key)
        root->left = avlDelete(root->left, key);
    else if (key > root->key)
        root->right = avlDelete(root->right, key);
    else {
        // node found — handle deletion cases
        if (root->left == nullptr || root->right == nullptr) {
            // one child or no child
            AVLNode* temp = (root->left) ? root->left : root->right;
            if (temp == nullptr) {
                // no child
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;   // copy content of non-null child
            }
            delete temp;
        } else {
            // two children: replace with inorder successor
            AVLNode* temp = avlMinNode(root->right);
            root->key     = temp->key;
            root->value   = temp->value;
            root->right   = avlDelete(root->right, temp->key);
        }
    }

    if (root == nullptr) return root;

    // Step 2: update height
    root->height = max(avlHeight(root->left), avlHeight(root->right)) + 1;

    // Step 3: check balance and rotate if needed
    int balance = avlGetBalance(root);

    if (balance > 1 && avlGetBalance(root->left) >= 0)
        return avlRotateRight(root);

    if (balance > 1 && avlGetBalance(root->left) < 0) {
        root->left = avlRotateLeft(root->left);
        return avlRotateRight(root);
    }

    if (balance < -1 && avlGetBalance(root->right) <= 0)
        return avlRotateLeft(root);

    if (balance < -1 && avlGetBalance(root->right) > 0) {
        root->right = avlRotateRight(root->right);
        return avlRotateLeft(root);
    }

    return root;
}

/* ══════════════════════════════════════════════════════════════
 * avlSearch — returns node if found, nullptr otherwise
 ══════════════════════════════════════════════════════════════ */
AVLNode* avlSearch(AVLNode* root, string key) {
    if (root == nullptr) return nullptr;
    if (key == root->key) return root;
    if (key < root->key)  return avlSearch(root->left,  key);
    return avlSearch(root->right, key);
}

/* ══════════════════════════════════════════════════════════════
 * avlUpdateValue — find by key and update value in place
 ══════════════════════════════════════════════════════════════ */
void avlUpdateValue(AVLNode* root, string key, int newValue) {
    AVLNode* node = avlSearch(root, key);
    if (node != nullptr)
        node->value = newValue;
}

/* ══════════════════════════════════════════════════════════════
 * avlInorder — Left → Root → Right (ascending order by key)
 ══════════════════════════════════════════════════════════════ */
void avlInorder(AVLNode* root) {
    if (root == nullptr) return;
    avlInorder(root->left);
    cout << "  " << root->key << " : " << root->value << endl;
    avlInorder(root->right);
}

/* ══════════════════════════════════════════════════════════════
 * avlCollect (internal helper for getTopK)
 * Collects all nodes into an array via inorder traversal.
 ══════════════════════════════════════════════════════════════ */
static void avlCollect(AVLNode* root, AVLNode* arr[], int& count) {
    if (root == nullptr) return;
    avlCollect(root->left, arr, count);
    arr[count++] = root;
    avlCollect(root->right, arr, count);
}

/* ══════════════════════════════════════════════════════════════
 * avlGetTopK
 * Collects all nodes, sorts by value (selection sort), prints top K.
 ══════════════════════════════════════════════════════════════ */
void avlGetTopK(AVLNode* root, int k) {
    AVLNode* arr[500];
    int      count = 0;
    avlCollect(root, arr, count);

    // simple selection sort by value (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (arr[j]->value > arr[i]->value) {
                AVLNode* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    cout << "Top " << k << " results:" << endl;
    for (int i = 0; i < k && i < count; i++) {
        cout << "  " << (i + 1) << ". " << arr[i]->key
             << " — score: " << arr[i]->value << endl;
    }
}

/* ══════════════════════════════════════════════════════════════
 * avlRangeQuery — print all nodes where minVal <= value <= maxVal
 ══════════════════════════════════════════════════════════════ */
void avlRangeQuery(AVLNode* root, int minVal, int maxVal) {
    if (root == nullptr) return;
    avlRangeQuery(root->left, minVal, maxVal);
    if (root->value >= minVal && root->value <= maxVal)
        cout << "  " << root->key << " : " << root->value << endl;
    avlRangeQuery(root->right, minVal, maxVal);
}

/* ═══ Module-level wrappers ═══ */

void insertUserAVL(string userName, int activity) {
    userAVLRoot = avlInsert(userAVLRoot, userName, activity);
}

void deleteUserAVL(string userName) {
    userAVLRoot = avlDelete(userAVLRoot, userName);
}

void incrementUserActivity(string userName) {
    AVLNode* node = avlSearch(userAVLRoot, userName);
    if (node != nullptr) {
        node->value++;
    }
}

void insertPostAVL(string postID, int likes) {
    postAVLRoot = avlInsert(postAVLRoot, postID, likes);
}

void deletePostAVL(string postID) {
    postAVLRoot = avlDelete(postAVLRoot, postID);
}

void updatePostAVL(string postID, int newLikes) {
    avlUpdateValue(postAVLRoot, postID, newLikes);
}

/* ══════════════════════════════════════════════════════════════
 * analyticsMenu — Displays the Analytics submenu
 ══════════════════════════════════════════════════════════════ */
void analyticsMenu() {
    int choice;
    do {
        cout << "\n======= ANALYTICS & RANKING =======" << endl;
        if (currentUser)
            cout << "Logged in as: " << currentUser->userName << endl;
        else
            cout << "Not logged in" << endl;
        cout << "-----------------------------------" << endl;
        cout << "1. Top Active Users"    << endl;
        cout << "2. Most Liked Posts"   << endl;
        cout << "3. All Users (sorted)" << endl;
        cout << "4. All Posts (sorted)" << endl;
        cout << "5. Range Query (posts)" << endl;
        cout << "6. Back"               << endl;
        cout << "-----------------------------------" << endl;
        cout << "Choice: "; cin >> choice;

        int k, minV, maxV;
        switch (choice) {
            case 1:
                cout << "Show top how many? "; cin >> k;
                avlGetTopK(userAVLRoot, k);
                break;
            case 2:
                cout << "Show top how many? "; cin >> k;
                avlGetTopK(postAVLRoot, k);
                break;
            case 3:
                cout << "\nAll users (sorted by name):" << endl;
                if (userAVLRoot == nullptr) cout << "No data." << endl;
                else avlInorder(userAVLRoot);
                break;
            case 4:
                cout << "\nAll posts (sorted by ID):" << endl;
                if (postAVLRoot == nullptr) cout << "No data." << endl;
                else avlInorder(postAVLRoot);
                break;
            case 5:
                cout << "Min likes: "; cin >> minV;
                cout << "Max likes: "; cin >> maxV;
                cout << "\nPosts with likes in [" << minV << ", " << maxV << "]:" << endl;
                avlRangeQuery(postAVLRoot, minV, maxV);
                break;
            case 6: cout << "Returning to main menu..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 6);
}
