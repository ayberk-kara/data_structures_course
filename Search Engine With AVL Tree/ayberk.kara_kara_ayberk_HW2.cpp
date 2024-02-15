#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iterator>
#include <fstream>

using namespace std;

//Data type for avl tree
struct DocumentItem {
    string documentName;
    int count;
};

struct WordItem {
    string word;
    vector<DocumentItem> documentItemVec;
};

//Data type for printing at the end
struct printWordItem {
    string printWordName;
    int count;
};

struct printDocItem {
    string printDocName;
    vector<DocumentItem> printWordsAndCountsVec;
};

template <class NodeT, class Comparable>
class AVLNode {
public:
    NodeT nodeT;
    Comparable comparable;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const NodeT& k, const Comparable& v)
        : nodeT(k), comparable(v), height(1), left(nullptr), right(nullptr) {}
};

template <class NodeT, class Comparable>
class AVLSearchTree {
private:
    AVLNode<NodeT, Comparable>* root;

    int height(AVLNode<NodeT, Comparable>* node) {
        return (node == nullptr) ? 0 : node->height;
    }

    int findBalanceF(AVLNode<NodeT, Comparable>* node) {
        return height(node->right) - height(node->left);
    }

    void findHeightF(AVLNode<NodeT, Comparable>* node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    AVLNode<NodeT, Comparable>* rotateRight(AVLNode<NodeT, Comparable>* y) {
        if (y == nullptr || y->left == nullptr)
            return y;

        AVLNode<NodeT, Comparable>* x = y->left;
        AVLNode<NodeT, Comparable>* T2 = (x->right != nullptr) ? x->right : nullptr;

        x->right = y;
        y->left = T2;

        findHeightF(y);
        findHeightF(x);

        return x;
    }

    AVLNode<NodeT, Comparable>* rotateLeft(AVLNode<NodeT, Comparable>* x) {
        if (x == nullptr || x->right == nullptr)
            return x;

        AVLNode<NodeT, Comparable>* y = x->right;
        AVLNode<NodeT, Comparable>* T2 = (y->left != NULL) ? y->left : NULL;

        y->left = x;
        x->right = T2;

        findHeightF(x);
        findHeightF(y);

        return y;
    }

    AVLNode<NodeT, Comparable>* insert(AVLNode<NodeT, Comparable>* node, const NodeT& nodeT, const Comparable& comparable) {
        if (node == nullptr)
            return new AVLNode<NodeT, Comparable>(nodeT, comparable);

        if (nodeT < node->nodeT)
            node->left = insert(node->left, nodeT, comparable);
        else if (nodeT > node->nodeT)
            node->right = insert(node->right, nodeT, comparable);
        else {

            node->comparable.documentItemVec.back().count++;
            return node;
        }

        findHeightF(node);

        int balance = findBalanceF(node);


        if (balance > 1 && node->left != nullptr && nodeT < node->left->nodeT)
            return rotateRight(node);


        if (balance < -1 && node->right != nullptr && nodeT > node->right->nodeT)
            return rotateLeft(node);


        if (balance > 1 && node->left != nullptr && nodeT > node->left->nodeT) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }


        if (balance < -1 && node->right != nullptr && nodeT < node->right->nodeT) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode<NodeT, Comparable>* findMin(AVLNode<NodeT, Comparable>* node) const {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    AVLNode<NodeT, Comparable>* remove(AVLNode<NodeT, Comparable>* node, const NodeT& nodeT) {
        if (node == NULL) {
            return NULL;
        }

        if (nodeT < node->nodeT) {
            node->left = remove(node->left, nodeT);
        }
        else if (nodeT > node->nodeT) {
            node->right = remove(node->right, nodeT);
        }
        else {

            if (node->left == nullptr && node->right == nullptr) {
                // No child 
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) {
                // One right child  
                AVLNode<NodeT, Comparable>* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                // One left child 
                AVLNode<NodeT, Comparable>* temp = node->left;
                delete node;
                return temp;
            }
            else {
                // Two children 
                AVLNode<NodeT, Comparable>* successor = findMin(node->right);
                node->nodeT = successor->nodeT;
                node->comparable = successor->comparable;
                node->right = remove(node->right, successor->nodeT);
            }
        }

        findHeightF(node);

        int balance = findBalanceF(node);

        // Left Left 
        if (balance > 1 && node->left != nullptr && nodeT < node->left->nodeT)
            return rotateRight(node);

        // Right Right 
        if (balance < -1 && node->right != nullptr && nodeT > node->right->nodeT)
            return rotateLeft(node);

        // Left Right 
        if (balance > 1 && node->left != nullptr && nodeT > node->left->nodeT) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left 
        if (balance < -1 && node->right != nullptr && nodeT < node->right->nodeT) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }



public:
    AVLSearchTree() : root(nullptr) {}

    AVLNode<NodeT, Comparable>* getRoot() const {
        return root;
    }

    AVLNode<NodeT, Comparable>* findNode(AVLNode<NodeT, Comparable>* node, const NodeT& nodeT) {
        if (node == nullptr || node->nodeT == nodeT)
            return node;

        if (nodeT < node->nodeT)
            return findNode(node->left, nodeT);
        else
            return findNode(node->right, nodeT);
    }

    void insert(const NodeT& nodeT, const Comparable& comparable) {
        root = insert(root, nodeT, comparable);
    }

    void remove(const NodeT& nodeT) {
        root = remove(root, nodeT);
    }

    bool containsWord(const NodeT& nodeT) {
        return findNode(root, nodeT) != nullptr;
    }

    Comparable getWordItem(const NodeT& nodeT) {
        AVLNode<NodeT, Comparable>* foundNode = findNode(root, nodeT);
        return foundNode != nullptr ? foundNode->comparable : Comparable();
    }
};

void StoreWordItem(const WordItem& wordItem, vector<printDocItem>& DOCvec) {
    // transforming word{textfilename, count} data structure to textfilename{word, count} structure
    //looking if document name exists in the second vector if not adding if exists, adding word and its count
    for (const auto& docItem : wordItem.documentItemVec) {

        auto it = find_if(DOCvec.begin(), DOCvec.end(), [&docItem](const printDocItem& pdi) { return pdi.printDocName == docItem.documentName; });

        if (it != DOCvec.end()) {

            bool wordFound = false;
            for (auto& wordCount : it->printWordsAndCountsVec) {
                if (wordCount.documentName == wordItem.word) {
                    wordCount.count += docItem.count;
                    wordFound = true;
                    break;
                }
            }
            if (!wordFound) {
                it->printWordsAndCountsVec.push_back({ wordItem.word, docItem.count });
            }
        }
        else {

            printDocItem newDocItem;
            newDocItem.printDocName = docItem.documentName;
            newDocItem.printWordsAndCountsVec.push_back({ wordItem.word, docItem.count });
            DOCvec.push_back(newDocItem);
        }
    }
}

void displayDocItem(const printDocItem& docItem) {
    //printing fuction for new data type to match output format
    cout << "in document " << docItem.printDocName << ", ";
    for (size_t i = 0; i < docItem.printWordsAndCountsVec.size(); ++i) {
        cout << docItem.printWordsAndCountsVec[i].documentName<< " found " << docItem.printWordsAndCountsVec[i].count << " times";
        if (i != docItem.printWordsAndCountsVec.size() - 1) {
            cout << ", ";
        }
    }

    cout << "." << endl;
}





int main() {

    int numFiles;

    cout << "Enter number of input files: ";
    cin >> numFiles;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    AVLSearchTree<string, WordItem> avlTree;


    for (int i = 0; i < numFiles; ++i) {
        string fileName;
        cout << "Enter file name " << i + 1 << ": ";
        getline(cin, fileName);

        ifstream inputFile(fileName);
        if (!inputFile.is_open()) {
            cout << "dosya acilmadi" << endl;
            return 1;
        }

        string word;
        while (inputFile >> word) {
            bool isAlphabetic = true;
            for (char c : word) {
                if (!isalpha(c)) {
                    isAlphabetic = false;
                    break;
                }
            }

            if (isAlphabetic) {
                transform(word.begin(), word.end(), word.begin(), ::tolower);

                WordItem newItem;
                newItem.word = word;
                newItem.documentItemVec.push_back({ fileName, 1 });

                AVLNode<string, WordItem>* existingNode = avlTree.findNode(avlTree.getRoot(), word);
                if (existingNode != nullptr) {
                    bool documentFound = false;
                    for (auto& docItem : existingNode->comparable.documentItemVec) {
                        if (docItem.documentName == fileName) {
                            docItem.count++;
                            documentFound = true;
                            break;
                        }
                    }
                    if (!documentFound) {
                        existingNode->comparable.documentItemVec.push_back({ fileName, 1 });
                    }
                }
                else {
                    avlTree.insert(word, newItem);
                }
            }
        }

        inputFile.close();
    }
    bool removed = false;
    string queryLine;
    cout << "Enter queried words in one line: ";
    getline(cin, queryLine);

    while (queryLine != "ENDOFINPUT") {

        vector<printDocItem> DOCvec; //Vec for printing

        istringstream iss(queryLine);
        string queryWord;
        bool atLeastOneWordFound = false;
        printDocItem pItem;

        while (iss >> queryWord) {
            if (queryWord != "REMOVE")
            {
                transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower);
                //lower case transformation if not REMOVE
            }


            if (queryWord == "REMOVE") {
                iss >> queryWord;
                avlTree.remove(queryWord);
                cout << queryWord << " has been REMOVED" << endl;
                removed = true;
            }
            else {
                if (avlTree.containsWord(queryWord)) {
                    atLeastOneWordFound = true;
                    WordItem wordItem = avlTree.getWordItem(queryWord);
                    StoreWordItem(wordItem, DOCvec);
                }
            }
        }

        for (const auto& docItem : DOCvec) {
            displayDocItem(docItem);
        }

        if (!atLeastOneWordFound && !removed) {
            cout << "No document contains the given query" << endl;
        }

        removed = false;
        cout << "Enter queried words in one line: ";
        getline(cin, queryLine);
    }



    return 0;
}
