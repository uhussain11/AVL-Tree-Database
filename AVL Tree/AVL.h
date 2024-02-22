#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

struct Node {
    string name;
    int ufid;
    int height;
    Node* left;
    Node* right;
    Node(string n, int id) : name(n), ufid(id), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node *root;

    // helper functions to enhance AVL operation
    Node *InsertHelper(Node *node, string name, int ufid);
    Node *removeInorderHelper(Node *node, int &count, int N);
    void inorderHelper(Node *node, stringstream &ss);
    void preorderHelper(Node *node, stringstream &ss);
    void postorderHelper(Node *node, stringstream &ss);
    Node *deleteAndBalance(Node *node, int ufid);
    Node *balanceNode(Node *node);
    Node *minValueNode(Node *node);
    int getHeight(Node *node);
    int getBalance(Node *node);
    bool searchHelper(Node *node, int ufid);
    int getLevelCount(Node *node);
    Node *searchByIDHelper(Node *node, int ufid);
    void searchByNameHelper(Node *node, const string &name, bool &found, stringstream &ss);
    // rotational operations to maintain balance
    Node *rotateLeft(Node *node);
    Node *rotateRight(Node *node);
    Node *rotateLeftRight(Node *node);
    Node *rotateRightLeft(Node *node);

public:
    AVLTree() : root(nullptr) {}
    bool Insert(string name, int ufid);
    bool Delete(int ufid);
    void removeInorder(int N);
    string printInorder();
    string printPreorder();
    string printPostorder();
    int printLevelCount();
    string searchByID(int ufid);
    string searchByName(const string &name);
};

Node* AVLTree::rotateLeft(Node* node) {
    Node* newParent = node->right; // right child becomes parent
    node->right = newParent->left; // adjust right child of node
    newParent->left = node; // current node becomes left child of new parent
    node->height = 1 + max(getHeight(node->left), getHeight(node->right)); // recalculate heights
    newParent->height = 1 + max(getHeight(newParent->left), getHeight(newParent->right));
    return newParent;
}

Node* AVLTree::rotateRight(Node* node) {
    Node* newParent = node->left; // left child becomes parent
    node->left = newParent->right; // adjust left child of the node
    newParent->right = node; // current node becomes right child of new parent
    node->height = 1 + max(getHeight(node->left), getHeight(node->right)); // recalculate heights
    newParent->height = 1 + max(getHeight(newParent->left), getHeight(newParent->right));
    return newParent;
}

Node* AVLTree::rotateLeftRight(Node* node) {
    node->left = rotateLeft(node->left); // left rotation on left child
    return rotateRight(node); // right rotation on current node
}

Node* AVLTree::rotateRightLeft(Node* node) {
    node->right = rotateRight(node->right); // right rotation on right child
    return rotateLeft(node); // left rotation on current node
}

int AVLTree::getHeight(Node* node) {
    if (!node) return 0; // if null, returns 0
    return node->height;
}

// computes balance factor (difference between height of left and right subtree)
int AVLTree::getBalance(Node* node) {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// searched for UFID in AVL tree. Returns true if found, false if not
bool AVLTree::searchHelper(Node* node, int ufid) {
    if (!node) return false;
    if (ufid == node->ufid) return true;
    if (ufid < node->ufid) return searchHelper(node->left, ufid);
    return searchHelper(node->right, ufid);
}


Node* AVLTree::InsertHelper(Node* node, string name, int ufid) {
    if (node == nullptr) { // create a new node
        return new Node(name, ufid);
    }

    if (ufid < node->ufid) { // insert in left subtree if ufid is less than current
        node->left = InsertHelper(node->left, name, ufid);
    } else if (ufid > node->ufid) { // insert in right subtree if greater than current
        node->right = InsertHelper(node->right, name, ufid);
    } else {
        return node;
    }
    // update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    return balanceNode(node);
}

bool AVLTree::Insert(string name, int ufid) {
    if (searchHelper(root, ufid)) { // Checks if UFID already exists
        return false;
    }
    if (to_string(ufid).length() != 8 || !all_of(name.begin(), name.end(), [](char c) { return isalpha(c) || isspace(c); })) {
        return false; // Checks if UFID has 8 digits, only contains alphabets or spaces
    }
    root = InsertHelper(root, name, ufid); // Recursive insert
    return true;
}

Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;
    // Keep moving to the leftmost node to find the smallest value.
    while (current->left) current = current->left;
    return current;
}

Node* AVLTree::deleteAndBalance(Node* node, int ufid) {
    if (!node) return node;
    // Traverse the left subtree if the  ufid is smaller than the current node's ufid, 
    // otherwise traverse the right subtree.
    if (ufid < node->ufid) {
        node->left = deleteAndBalance(node->left, ufid);
    } else if (ufid > node->ufid) {
        node->right = deleteAndBalance(node->right, ufid);
    } else {
        // Node with only one child or no child
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            if (!temp) {
                delete node;
                return nullptr;
            }
            Node* childNode = new Node(temp->name, temp->ufid);
            childNode->left = temp->left;
            childNode->right = temp->right;
            childNode->height = temp->height;
            delete node;
            return childNode;
        }
        else {
            // Node with two children
            Node* temp = minValueNode(node->right);
            node->ufid = temp->ufid;
            node->name = temp->name;
            node->right = deleteAndBalance(node->right, temp->ufid);
        }
    }

    if (!node) return node; // Return if the tree had only one node

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    return balanceNode(node);
}

// First checks if the UFID exists, then calls the recursive delete function.
bool AVLTree::Delete(int ufid) {
    if (!searchHelper(root, ufid)) {
        return false;
    }
    root = deleteAndBalance(root, ufid);
    return true;
}

Node* AVLTree::removeInorderHelper(Node* node, int& count, int N) {
    if (!node) return nullptr;

    node->left = removeInorderHelper(node->left, count, N);

    if (count == N) {
        Node* temp;
        if (node->left == nullptr) {
            temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            temp = node->left;
            delete node;
            return temp;
        }
        temp = minValueNode(node->right); // Get the inorder successor
        node->ufid = temp->ufid;
        node->name = temp->name;
        node->right = deleteAndBalance(node->right, temp->ufid);
    }
    count++;

    node->right = removeInorderHelper(node->right, count, N);
    return balanceNode(node); // Re-balance following deletion
}

void AVLTree::removeInorder(int N) {
    int count = 0;
    root = removeInorderHelper(root, count, N);
}

Node* AVLTree::balanceNode(Node* node) {
    int balance = getBalance(node);
    // Right heavy node
    if (balance < -1) {
        if (getBalance(node->right) > 0) {
            return rotateRightLeft(node);
            // Right-Right case
        } else {
            return rotateLeft(node);
        }
        // Left heavy node
    } else if (balance > 1) {
        if (getBalance(node->left) < 0) {
            return rotateLeftRight(node);
            // Left-Left Case
        } else {
            return rotateRight(node);
        }
    }

    return node;
}

// Populates stringstream wutth node names in the in-order sequence
void AVLTree::inorderHelper(Node* node, stringstream &ss) {
    if (!node) return;
    inorderHelper(node->left, ss);
    if (ss.tellp() != 0) ss << ", ";
    ss << node->name;
    inorderHelper(node->right, ss);
}

// Returns names of nodes in in-order traversal
string AVLTree::printInorder() {
    stringstream ss;
    inorderHelper(root, ss);
    return ss.str();
}

// Recursive helper
void AVLTree::preorderHelper(Node* node, stringstream &ss) {
    if (!node) return;
    if (!ss.str().empty()) ss << ", ";
    ss << node->name;
    preorderHelper(node->left, ss);
    preorderHelper(node->right, ss);
}

// Return names of nodes in pre-order traversal
string AVLTree::printPreorder() {
    stringstream ss;
    preorderHelper(root, ss);
    return ss.str();
}

// Recursive helper
void AVLTree::postorderHelper(Node* node, stringstream &ss) {
    if (!node) return;
    postorderHelper(node->left, ss);
    postorderHelper(node->right, ss);
    if (!ss.str().empty()) ss << ", ";
    ss << node->name;
}

// Return names of nodes in post-order traversal
string AVLTree::printPostorder() {
    stringstream ss;
    postorderHelper(root, ss);
    return ss.str();
}

// Computes height of AVL tree
int AVLTree::getLevelCount(Node* node) {
    if (!node) return 0;
    return 1 + max(getLevelCount(node->left), getLevelCount(node->right));
}

// Returns height or level count of AVL tree
int AVLTree::printLevelCount() {
    return getLevelCount(root);
}

// Searches for node by UFID recursively
Node* AVLTree::searchByIDHelper(Node* node, int ufid) {
    if (!node) return nullptr;
    if (ufid == node->ufid) return node;
    if (ufid < node->ufid) return searchByIDHelper(node->left, ufid);
    return searchByIDHelper(node->right, ufid);
}

// Searches for UFID by name recursively
void AVLTree::searchByNameHelper(Node* node, const string& name, bool& found, stringstream& ss) {
    if (!node) return;
    searchByNameHelper(node->left, name, found, ss);
    if (node->name == name) {
        if (found) {
            ss << "\n";
        }
        ss << node->ufid;
        found = true;
    }
    searchByNameHelper(node->right, name, found, ss);
}

string AVLTree::searchByID(int ufid) {
    Node* result = searchByIDHelper(root, ufid);
    if (!result) {
        return "unsuccessful";
    }
    return result->name;
}


string AVLTree::searchByName(const string& name) {
    bool found = false;
    stringstream ss;
    searchByNameHelper(root, name, found, ss);
    if (!found) return "unsuccessful";
    return ss.str();
}



