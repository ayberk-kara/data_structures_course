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
#include <random>
#include <iomanip>
using namespace std;

// Data type for avl tree
struct DocumentItem {
    string documentName;
    int count;
};

struct WordItem {
    string word;
    vector<DocumentItem> documentItemVec;
    bool operator>(const WordItem& other) const {
        return word > other.word;
    }
    bool operator==(const WordItem& other) const {
        return this->word == other.word;
    }
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
bool contains(const std::vector<WordItem>& vec, const std::string& targetWord) {
    // Binary search to check if the targetWord is in the vector
    auto it = std::lower_bound(vec.begin(), vec.end(), WordItem{ targetWord },
        [](const WordItem& item, const WordItem& target) {
            return item.word < target.word;
        });

    return it != vec.end() && it->word == targetWord;
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

    void forEach(std::function<void(const KeyT&, const ValueT&)> func) const {
        for (size_t i = 0; i < capacity; ++i) {
            for (const auto& node : table[i]) {
                func(node.key, node.value);
            }
        }
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


        //cout << "rehashed..." << endl;
       // cout << "previous table size: " << capacity << ", new table size: " << newCapacity
         //   << ", current unique word count " << uniqueWordCount << "," << endl;
        //cout << "current load factor: " << loadFactor() << endl;
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


chrono::microseconds quickSort(vector<WordItem>& vec, const string& pivotType) {
    auto start = chrono::high_resolution_clock::now();

    if (pivotType == "median") {
        for (size_t i = 0; i < vec.size(); ++i) {
            auto medianIt = vec.begin() + i + (vec.size() - i) / 2;
            nth_element(vec.begin() + i, medianIt, vec.end(),
                [](const WordItem& a, const WordItem& b) { return a.word < b.word; });
        }
    }
    else if (pivotType == "random") {
        for (size_t i = 0; i < vec.size(); ++i) {
            size_t randomIndex = i + rand() % (vec.size() - i);
            swap(vec[i], vec[randomIndex]);
            sort(vec.begin() + i + 1, vec.end(),
                [](const WordItem& a, const WordItem& b) { return a.word < b.word; });
        }
    }
    else if (pivotType == "first") {
        for (size_t i = 0; i < vec.size(); ++i) {
            sort(vec.begin() + i, vec.end(),
                [](const WordItem& a, const WordItem& b) { return a.word < b.word; });
        }
    }

    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start);
}


chrono::microseconds insertionSort(vector<WordItem>& vec) {
    auto start = chrono::high_resolution_clock::now();

    int i, j;
    for (i = 1; i < vec.size(); i++) {
        WordItem key = vec[i];
        j = i - 1;

        while (j >= 0 && vec[j].word > key.word) {
            vec[j + 1] = vec[j];
            j = j - 1;
        }
        vec[j + 1] = key;
    }

    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start);
}

void heapify(vector<WordItem>& vec, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && vec[l] > vec[largest])
        largest = l;

    if (r < n && vec[r] > vec[largest])
        largest = r;

    if (largest != i) {
        swap(vec[i], vec[largest]);
        heapify(vec, n, largest);
    }
}
WordItem getWordItem(const std::vector<WordItem>& vec, const std::string& targetWord) {
    auto it = std::lower_bound(vec.begin(), vec.end(), WordItem{ targetWord },
        [](const WordItem& item, const WordItem& target) {
            return item.word < target.word;
        });

    if (it != vec.end() && it->word == targetWord) {
        return *it;
    }
    else {
        return WordItem{};
    }
}

chrono::microseconds heapSort(vector<WordItem>& vec)
{
    auto start = chrono::high_resolution_clock::now();

    int n = vec.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(vec, n, i);

    for (int i = n - 1; i >= 0; i--) {
        swap(vec[0], vec[i]);

        heapify(vec, i, 0);
    }

    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start);
}


void merge(vector<WordItem>& vec, int const left, int const mid, int const right)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;
    auto leftVector = vector<WordItem>(vec.begin() + left, vec.begin() + left + subArrayOne);
    auto rightVector = vector<WordItem>(vec.begin() + mid + 1, vec.begin() + mid + 1 + subArrayTwo);

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftVector[indexOfSubArrayOne].word <= rightVector[indexOfSubArrayTwo].word) {
            vec[indexOfMergedArray] = leftVector[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            vec[indexOfMergedArray] = rightVector[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        vec[indexOfMergedArray] = leftVector[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        vec[indexOfMergedArray] = rightVector[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

chrono::microseconds mergeSort(vector<WordItem>& vec, int const begin, int const last)
{
    auto start = chrono::high_resolution_clock::now();
    if (begin >= last)
        return chrono::microseconds(0);
    int mid = begin + (last - begin) / 2;
    mergeSort(vec, begin, mid);
    mergeSort(vec, mid + 1, last);
    merge(vec, begin, mid, last);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start);
}

bool contains(const vector<WordItem>& vec, const WordItem& element) {
    return find(vec.begin(), vec.end(), element) != vec.end();
}



template <typename KeyT, typename ValueT>
void displayHashTable(const HashTable<KeyT, ValueT>& hashTable) {
    hashTable.printHashTable();
}
int main() {

    BinarySearchTree<string, WordItem> BstTree;
    HashTable<string, WordItem> htTable(11, customHashFunction);
    vector<WordItem> vecQuickSort;
    vector<WordItem> vecInsertSort;
    vector<WordItem> vecMergeSort;
    vector<WordItem> vecHeapSort;

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


    htTable.forEach([&vecHeapSort](const string& key, const WordItem& value) {
        vecHeapSort.push_back(value);
        });

    random_device rd;
    mt19937 g(rd());
    shuffle(vecHeapSort.begin(), vecHeapSort.end(), g);

    for (const auto& element : vecHeapSort) {
        vecQuickSort.push_back(element);
    }
    for (const auto& element : vecHeapSort) {
        vecMergeSort.push_back(element);

    }
    for (const auto& element : vecHeapSort) {
        vecInsertSort.push_back(element);
    }

    /*
    for (size_t i = 0; i < 40; i++)
    {
        cout << vecHeapSort[i];
    }

    */


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

    bool allWordsFound = true;
    vector<printDocItem>DocVec2;
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
                //displayDocItem(docItem);
            }
        }
        else {
            cout << "No document contains the given query" << endl;
            //cout << "No document contains the given query" << endl;
        }
        DocVec2 = DOCvec;
        allWordsFound = atLeastOneWordFound;
    }




    // Binary Search


    if (true)
    {
        vector<printDocItem> DOCvec; // Vec for printing

        bool atLeastOneWordFound = true;
        printDocItem pItem;

        atLeastOneWordFound = true;
        DOCvec.clear();
        for (const auto& word : queriedWordsSet)
        {
            if (contains(vecHeapSort, word)) {
                WordItem wordItem = getWordItem(vecHeapSort, word);
                StoreWordItem(wordItem, DOCvec);
            }
            else {
                atLeastOneWordFound = false;
            }
        }

        if (allWordsFound)
        {
            for (const auto& docItem : DocVec2)
            {
                // cout << "bst display";
                displayDocItem(docItem);
            }
        }
        else
        {
            cout << "No document contains the given query" << endl;
        }
    }

    auto start2 = chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; i++)
    {
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
                    //displayDocItem(docItem);
                }
            }
            else {
                //cout << "No document contains the given query" << endl;
            }
        }
    }
    cout << endl;

    auto BSTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start2).count();

    //cout << "\nTime (BST): " << BSTTime / 20 << "\n";
    cout << "Binary Search Tree Time: " << BSTTime / 100 << "\n";

    auto startHT = chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; i++)
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
    auto HTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startHT).count();
    //cout << "\nTime (HashTable): " << HTTime.count() / 20.0 << "\n";
    cout << "Hash Table Time: " << HTTime / 100.0 << "\n";

    double speedup = static_cast<double>(BSTTime) / HTTime;

    auto start3 = chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; i++)
    {
        if (true)
        {
            vector<printDocItem> DOCvec; // Vec for printing

            bool atLeastOneWordFound = true;
            printDocItem pItem;

            atLeastOneWordFound = true;
            DOCvec.clear();
            for (const auto& word : queriedWordsSet)
            {
                if (contains(vecHeapSort, word)) {
                    WordItem wordItem = getWordItem(vecHeapSort, word);
                    StoreWordItem(wordItem, DOCvec);
                }
                else {
                    atLeastOneWordFound = false;
                }
            }

            if (atLeastOneWordFound)
            {
                for (const auto& docItem : DOCvec)
                {
                    // cout << "bst display";
                    //displayDocItem(docItem);
                }
            }
            else
            {
                //cout << "No document contains the given query" << endl;
            }
        }
    }
    auto BStime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start3).count();
    cout << "Binary Search Time: " << BStime / 100 << "\n";
    cout << endl;
    auto quickSortRandomTime = quickSort(vecQuickSort, "random");
    cout << "Quick Sort(random) Time: " << quickSortRandomTime.count() << endl;
    auto quickSortMedianTime = quickSort(vecQuickSort, "median");
    cout << "Quick Sort(median) Time: " << quickSortMedianTime.count() << endl;
    auto quickSortFirstTime = quickSort(vecQuickSort, "first");
    cout << "Quick Sort(first) Time: " << quickSortFirstTime.count() << endl;
    auto mergeSortTime = mergeSort(vecMergeSort, 0, vecMergeSort.size() - 1);
    cout << "Merge Sort Time: " << mergeSortTime.count() << endl;
    auto heapSortTime = heapSort(vecHeapSort);
    cout << "Heap Sort Time: " << heapSortTime.count() << endl;
    auto insertionSortTime = insertionSort(vecInsertSort);
    cout << "Insertion Sort Time: " << insertionSortTime.count() << endl;
    cout << endl;

    cout << "Speed Up BST/HST " << speedup << endl;
    double speedUpMergeSort = static_cast<double>(quickSortMedianTime.count()) / mergeSortTime.count();
    double speedUpHeapSort = static_cast<double>(quickSortMedianTime.count()) / heapSortTime.count();
    double speedUpbstHT = static_cast<double>(HTTime) / (BStime);
    double speedUpInsertionSort = static_cast<double>(insertionSortTime.count()) / quickSortMedianTime.count();
    double speedUpBSTbst = static_cast<double>(BSTTime) / BStime;
    cout << "Speed Up Merge Sort/Quick Sort(Median): " << speedUpMergeSort << endl;
    cout << "Speed Up Heap Sort/Quick Sort(Median): " << speedUpHeapSort << endl;
    cout << "Speed Up Insertion Sort/Quick Sort(Median): " << speedUpInsertionSort << endl;
    cout << endl;

    cout << "Speed Up Binary Search Tree Time / Binary Search: " << speedUpBSTbst << endl;
    cout << "Speed Up Binary Search / Hash Table Time: " << speedUpbstHT << endl;
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









