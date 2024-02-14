#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iterator>
#include <fstream>
#include <set>
#include <chrono>
#include <functional>

using namespace std;

// Data type for avl tree
struct DocumentItem {
    string documentName;
    int count;
};

struct WordItem {
    string word;
    vector<DocumentItem> documentItemVec;
};

// Data type for printing at the end
struct printWordItem {
    string printWordName;
    int count;
};

struct printDocItem {
    string printDocName;
    vector<DocumentItem> printWordsAndCountsVec;
};

ostream& operator<<(ostream& os, const DocumentItem& docItem) {
    os << docItem.documentName << ": " << docItem.count;
    return os;
}

ostream& operator<<(ostream& os, const WordItem& wordItem) {
    os << wordItem.word << ": ";
    for (const auto& docItem : wordItem.documentItemVec) {
        os << docItem << " ";
    }
    return os;
}

template <class NodeT, class Comparable>
struct TreeNode {
    NodeT nodeT;
    Comparable comparable;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const NodeT& k, const Comparable& v)
        : nodeT(k), comparable(v), left(nullptr), right(nullptr) {}
};

template <class NodeT, class Comparable>
class BinarySearchTree {
private:
    TreeNode<NodeT, Comparable>* root;

    TreeNode<NodeT, Comparable>* insert(TreeNode<NodeT, Comparable>* node, const NodeT& nodeT, const Comparable& comparable) {
        if (node == nullptr)
            return new TreeNode<NodeT, Comparable>(nodeT, comparable);

        if (nodeT < node->nodeT)
            node->left = insert(node->left, nodeT, comparable);
        else if (nodeT > node->nodeT)
            node->right = insert(node->right, nodeT, comparable);
        else {
            node->comparable.documentItemVec.back().count++;
        }

        return node;
    }

    TreeNode<NodeT, Comparable>* findMin(TreeNode<NodeT, Comparable>* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode<NodeT, Comparable>* remove(TreeNode<NodeT, Comparable>* node, const NodeT& nodeT) {
        if (node == nullptr)
            return nullptr;

        if (nodeT < node->nodeT)
            node->left = remove(node->left, nodeT);
        else if (nodeT > node->nodeT)
            node->right = remove(node->right, nodeT);
        else {
            // Key found
            if (node->left == nullptr && node->right == nullptr) {
                // No child
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) {
                // One right child
                TreeNode<NodeT, Comparable>* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                // One left child
                TreeNode<NodeT, Comparable>* temp = node->left;
                delete node;
                return temp;
            }
            else {
                // Two children
                TreeNode<NodeT, Comparable>* successor = findMin(node->right);
                node->nodeT = successor->nodeT;
                node->comparable = successor->comparable;
                node->right = remove(node->right, successor->nodeT);
            }
        }

        return node;
    }

    TreeNode<NodeT, Comparable>* findNode(TreeNode<NodeT, Comparable>* node, const NodeT& nodeT) {
        if (node == nullptr || node->nodeT == nodeT)
            return node;

        if (nodeT < node->nodeT)
            return findNode(node->left, nodeT);
        else
            return findNode(node->right, nodeT);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    TreeNode<NodeT, Comparable>* getRoot() const {
        return root;
    }

    TreeNode<NodeT, Comparable>* findNode(const NodeT& nodeT) {
        return findNode(root, nodeT);
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
        TreeNode<NodeT, Comparable>* foundNode = findNode(root, nodeT);
        return foundNode != nullptr ? foundNode->comparable : Comparable();
    }
};

void StoreWordItem(const WordItem& wordItem, vector<printDocItem>& DOCvec) {
    for (const auto& docItem : wordItem.documentItemVec) {
        auto it = find_if(DOCvec.begin(), DOCvec.end(),
            [&docItem](const printDocItem& pdi) { return pdi.printDocName == docItem.documentName; });

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
    // printing function for new data type to match output format
    cout << "in document " << docItem.printDocName << ", ";
    for (size_t i = 0; i < docItem.printWordsAndCountsVec.size(); ++i) {
        cout << docItem.printWordsAndCountsVec[i].documentName << " found " << docItem.printWordsAndCountsVec[i].count << " times";
        if (i != docItem.printWordsAndCountsVec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "." << endl;
}


void displayDocItemHT(const printDocItem& docItem) {
    cout << "in document " << docItem.printDocName << ", ";
    for (size_t i = 0; i < docItem.printWordsAndCountsVec.size(); ++i) {
        cout << docItem.printWordsAndCountsVec[i].documentName << " found " << docItem.printWordsAndCountsVec[i].count;
        if (i != docItem.printWordsAndCountsVec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "." << endl;
}

template <typename KeyT, typename ValueT>
class HashNode {
public:
    KeyT key;
    ValueT value;

    HashNode(const KeyT& k, const ValueT& v) : key(k), value(v) {}
};

template <typename KeyT, typename ValueT>
class HashTable {
private:
    size_t capacity;
    size_t uniqueWordCount = 0;
    vector<list<HashNode<KeyT, ValueT>>> table;
    function<size_t(const KeyT&)> hashFunction;

public:
    template <typename KeyType, typename ValueType>
    friend void displayHashTable(const HashTable<KeyType, ValueType>& hashTable);

    HashTable(size_t initialCapacity, function<size_t(const KeyT&)> hashFunc)
        : capacity(initialCapacity), table(initialCapacity), hashFunction(hashFunc) {}

    template <typename KeyT, typename ValueT>
    void displayHashTable(const HashTable<KeyT, ValueT>& hashTable) {
        hashTable.printHashTable();
    }

    void insert(const KeyT& key, const ValueT& value) {

        checkAndResize();

        size_t index = hashFunction(key) % capacity;
        auto& bucket = table[index];

        auto it = find_if(bucket.begin(), bucket.end(), [&key](const HashNode<KeyT, ValueT>& node) {
            return node.key == key;
            });

        if (it != bucket.end()) {
            it->value = value;
        }
        else {

            bucket.emplace_back(key, value);
            uniqueWordCount++;
        }
    }

    ValueT& get(const KeyT& key) {
        size_t index = hashFunction(key) % capacity;
        for (auto& node : table[index]) {
            if (node.key == key) {
                return node.value;
            }
        }


        table[index].emplace_back(key, ValueT());
        return table[index].back().value;
    }

    bool contains(const KeyT& key) {
        size_t index = hashFunction(key) % capacity;
        for (const auto& node : table[index]) {
            if (node.key == key) {
                return true;
            }
        }
        return false;
    }

    double loadFactor() const {
        size_t itemCount = 0;
        for (const auto& bucket : table) {
            itemCount += bucket.size();
        }
        return static_cast<double>(itemCount) / capacity;
    }

    void printHashTable() const {
        for (size_t i = 0; i < capacity; ++i) {
            const auto& bucket = table[i];
            if (!bucket.empty()) {
                cout << "Bucket " << i << ": ";
                for (const auto& node : bucket) {
                    cout << "{" << node.key << ": " << node.value << "} " << endl;
                }
                cout << endl;
            }
        }
    }
    size_t countUniqueWords(const HashTable<KeyT, ValueT>& hashTable) const {
        size_t uniqueCount = 0;
        for (size_t i = 0; i < hashTable.capacity; ++i) {
            for (const auto& node : hashTable.table[i]) {
                ++uniqueCount;
            }
        }
        return uniqueCount;
    }

    void checkAndResize() {
        double currentLoadFactor = loadFactor();
        if (currentLoadFactor > 0.9) {
            size_t newCapacity = determineNewCapacity();
            resizeTable(newCapacity);
        }
    }

    size_t determineNewCapacity() const {
        return capacity * 2;
    }

    void resizeTable(size_t newCapacity) {
        HashTable<KeyT, ValueT> newTable(newCapacity, hashFunction);
        newTable.uniqueWordCount = 0;

        for (int i = 0; i < capacity; ++i) {
            for (const auto& node : table[i]) {
                newTable.insert(node.key, node.value);
            }
        }

        newTable.uniqueWordCount = countUniqueWords(newTable);


        *this = move(newTable);


        cout << "rehashed..." << endl;
        cout << "previous table size: " << capacity << ", new table size: " << newCapacity
            << ", current unique word count " << uniqueWordCount << "," << endl;
        cout << "current load factor: " << loadFactor() << endl;
    }

    void printFinalInfo() const {
        cout << "After preprocessing, the unique word count is " << uniqueWordCount
            << ". Current load ratio is " << loadFactor() << endl;
    }

    bool containsWord(const KeyT& key) const {
        size_t index = hashFunction(key) % capacity;
        for (const auto& node : table[index]) {
            if (node.key == key) {
                return true;
            }
        }
        return false;
    }
};

size_t customHashFunction(const string& key) {
    size_t bucketIndex;
    long long sum = 0, factor = 31;
    for (size_t i = 0; i < key.size(); ++i) {
        sum = ((sum % INT64_MAX) + (static_cast<int>(key[i]) * factor) % INT64_MAX) % INT64_MAX;
        factor = ((factor % INT64_MAX) * (31 % INT64_MAX)) % INT64_MAX;
    }
    bucketIndex = sum;
    return bucketIndex;
}

template <typename KeyT, typename ValueT>
void displayHashTable(const HashTable<KeyT, ValueT>& hashTable) {
    hashTable.printHashTable();
}
int main() {


    BinarySearchTree<string, WordItem> BstTree;
    HashTable<string, WordItem> htTable(11, customHashFunction);

    int numFiles;
    cout << "Enter number of input files: ";
    cin >> numFiles;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');



    vector<string> fileNames;
    for (int i = 0; i < numFiles; ++i) {
        string fileName;
        cout << "Enter " << i + 1 << ". file name: ";
        getline(cin, fileName);
        fileNames.push_back(fileName);
    }


    for (const auto& fileName : fileNames) {
        ifstream inputFile(fileName);
        if (!inputFile.is_open()) {
            cout << "File could not be opened" << endl;
            return 1;
        }

        string word;
        while (inputFile >> word) {
            transform(word.begin(), word.end(), word.begin(), [](char c) {
                return isalpha(c) ? c : ' ';
                });

            istringstream wordStream(word);
            while (wordStream >> word) {
                transform(word.begin(), word.end(), word.begin(), ::tolower);

                if (!word.empty()) {

                    WordItem newItem;
                    newItem.word = word;
                    newItem.documentItemVec.push_back({ fileName, 1 });

                    TreeNode<string, WordItem>* existingNode = BstTree.findNode(word);
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
                        BstTree.insert(word, newItem);
                    }


                    if (true)
                    {
                        WordItem newItem;
                        newItem.word = word;
                        newItem.documentItemVec.push_back({ fileName, 1 });

                        bool keyExists = htTable.contains(word);

                        if (keyExists) {
                            WordItem& existingWordItem = htTable.get(word);

                            bool documentFound = false;
                            for (auto& docItem : existingWordItem.documentItemVec) {
                                if (docItem.documentName == fileName) {
                                    docItem.count++;
                                    documentFound = true;
                                    break;
                                }
                            }

                            if (!documentFound) {
                                existingWordItem.documentItemVec.push_back({ fileName, 1 });
                            }
                        }
                        else {
                            htTable.insert(word, newItem);
                        }
                    }




                }
            }
        }

        inputFile.close();
    }

    htTable.printFinalInfo();

    string queryLine;
    cout << "Enter queried words in one line: ";
    getline(cin, queryLine);
    transform(queryLine.begin(), queryLine.end(), queryLine.begin(), [](char c) {
        return isalpha(c) ? tolower(c) : ' ';
        });
    istringstream iss(queryLine);
    set<string> queriedWordsSet;
    string queryWord;
    while (iss >> queryWord) {
        transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower);
        queriedWordsSet.insert(queryWord);
    }


    if (true)
    {
        vector<printDocItem> DOCvec; // Vec for printing




        bool atLeastOneWordFound = true;
        printDocItem pItem;
        atLeastOneWordFound = true;
        DOCvec.clear();
        for (const auto& word : queriedWordsSet) {
            if (htTable.contains(word)) {
                WordItem wordItem = htTable.get(word);
                StoreWordItem(wordItem, DOCvec);
            }
            else {
                // word not found in ht
                atLeastOneWordFound = false;
            }
        }

        if (atLeastOneWordFound) {
            for (const auto& docItem : DOCvec) {
                //cout << "hs display";
                displayDocItem(docItem);
            }
        }
        else {
            //cout << "No document contains the given query in ht table" << endl;
            cout << "No document contains the given query" << endl;
        }
    }

    if (true)
    {
        vector<printDocItem> DOCvec; // Vec for printing




        bool atLeastOneWordFound = true;
        printDocItem pItem;



        // bst query
        atLeastOneWordFound = true;  // Initialize to true
        DOCvec.clear();
        for (const auto& word : queriedWordsSet) {
            if (BstTree.containsWord(word)) {
                WordItem wordItem = BstTree.getWordItem(word);
                StoreWordItem(wordItem, DOCvec);
            }
            else {
                // word not found in the bst
                atLeastOneWordFound = false;  // Set to false if any word is not found
            }
        }

        if (atLeastOneWordFound) {
            for (const auto& docItem : DOCvec) {
                //cout << "bst display";
                displayDocItem(docItem);
            }
        }
        else {
            cout << "No document contains the given query" << endl;
        }
    }




    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < 20; i++)
    {
        if (true)
        {
            vector<printDocItem> DOCvec; // Vec for printing

            bool atLeastOneWordFound = true;
            printDocItem pItem;

            atLeastOneWordFound = true;
            DOCvec.clear();
            for (const auto& word : queriedWordsSet) {
                if (BstTree.containsWord(word)) {
                    WordItem wordItem = BstTree.getWordItem(word);
                    StoreWordItem(wordItem, DOCvec);
                }
                else {

                    atLeastOneWordFound = false;
                }
            }

            if (atLeastOneWordFound) {
                for (const auto& docItem : DOCvec) {
                    //cout << "bst display";
                    //displayDocItem(docItem);
                }
            }
            else {
                // cout << "No document contains the given query" << endl;
            }
        }
    }
    auto BSTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();

    //cout << "\nTime (BST): " << BSTTime / 20 << "\n";
    cout << "Time: " << BSTTime / 20 << "\n";

    auto startHT = chrono::high_resolution_clock::now();

    for (int i = 0; i < 20; i++)
    {
        if (true)
        {
            vector<printDocItem> DOCvec; // Vec for printing

            bool atLeastOneWordFound = true;
            printDocItem pItem;

            atLeastOneWordFound = true;
            DOCvec.clear();
            for (const auto& word : queriedWordsSet) {
                if (htTable.contains(word)) {
                    WordItem wordItem = htTable.get(word);
                    StoreWordItem(wordItem, DOCvec);
                }
                else {

                    atLeastOneWordFound = false;  // Set to false if any word is not found
                }
            }

            if (atLeastOneWordFound) {
                for (const auto& docItem : DOCvec) {
                    // cout << "hs display";
                     //displayDocItem(docItem);
                }
            }
            else {
                //cout << "No document contains the given query in ht table" << endl;
            }
        }
    }

    // After populating the hash table
    //displayHashTable(htTable);
    auto HTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startHT);
    //cout << "\nTime (HashTable): " << HTTime.count() / 20.0 << "\n";
    cout << "Time: " << HTTime.count() / 20.0 << "\n";
    // Calculate speedup ratio
    double speedup = static_cast<double>(BSTTime) / HTTime.count();
    cout << "Speed Up: " << speedup << endl;

    return 0;
}






/*


int main() {


    // Print HashTable information during rehashing


    string queryLine;
    cout << "Enter queried words in one line: ";
    getline(cin, queryLine);
    transform(queryLine.begin(), queryLine.end(), queryLine.begin(), [](char c) {
        return isalpha(c) ? tolower(c) : ' ';
        });
    istringstream iss(queryLine);
    set<string> queriedWordsSet;
    string queryWord;
    while (iss >> queryWord) {
        transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower);
        queriedWordsSet.insert(queryWord);
    }


    if (true)
    {
        vector<printDocItem> DOCvec; // Vec for printing




        bool atLeastOneWordFound = true;  // Initialize to true
        printDocItem pItem;
        // Perform HashTable query operation
        atLeastOneWordFound = true;  // Initialize to true
        DOCvec.clear();
        for (const auto& word : queriedWordsSet) {
            if (htTable.contains(word)) {
                WordItem wordItem = htTable.get(word);
                StoreWordItem(wordItem, DOCvec);
            }
            else {
                // Word not found in the HashTable
                atLeastOneWordFound = false;  // Set to false if any word is not found
            }
        }

        if (atLeastOneWordFound) {
            for (const auto& docItem : DOCvec) {
                cout << "hs display ";
                displayDocItem(docItem);
            }
        }
        else {
            cout << "No document contains the given query in ht table" << endl;
        }
    }





    auto start = chrono::high_resolution_clock::now();


    auto BSTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();

    cout << "\nTime (BST): " << BSTTime / 20 << "\n";


    auto startHT = chrono::high_resolution_clock::now();

    for (int i = 0; i < 20; i++)
    {
        if (true)
        {
            vector<printDocItem> DOCvec; // Vec for printing




            bool atLeastOneWordFound = true;  // Initialize to true
            printDocItem pItem;
            // Perform HashTable query operation
            atLeastOneWordFound = true;  // Initialize to true
            DOCvec.clear();
            for (const auto& word : queriedWordsSet) {
                if (htTable.contains(word)) {
                    WordItem wordItem = htTable.get(word);
                    StoreWordItem(wordItem, DOCvec);
                }
                else {
                    // Word not found in the HashTable
                    atLeastOneWordFound = false;  // Set to false if any word is not found
                }
            }

            if (atLeastOneWordFound) {
                for (const auto& docItem : DOCvec) {
                    // cout << "hs display";
                     //displayDocItem(docItem);
                }
            }
            else {
                //cout << "No document contains the given query in ht table" << endl;
            }
        }
    }

    // After populating the hash table
    //displayHashTable(htTable);
    auto HTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startHT);
    cout << "\nTime (HashTable): " << HTTime.count() / 20.0 << "\n";

    // Calculate speedup ratio
    double speedup = static_cast<double>(BSTTime) / HTTime.count();
    //cout << "Speed Up: " << speedup << endl;

    return 0;
}


*/









