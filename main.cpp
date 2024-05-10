#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define MAX_HEAP_SIZE 10000

using namespace std;

#define MAX_HEAP_SIZE 50

struct Node {
    unsigned freq;
    char item;
    Node *left, *right;
};

class MinHeap {
private:
    vector<Node> heap;
    int size;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void heapifyUp(int i) {
        while (i != 0 && heap[parent(i)].freq > heap[i].freq) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int minIndex = i;
        int l = leftChild(i);
        int r = rightChild(i);

        if (l < size && heap[l].freq < heap[minIndex].freq)
            minIndex = l;

        if (r < size && heap[r].freq < heap[minIndex].freq)
            minIndex = r;

        if (i != minIndex) {
            swap(heap[i], heap[minIndex]);
            heapifyDown(minIndex);
        }
    }

public:
    MinHeap() {
        size = 0;
        heap.resize(MAX_HEAP_SIZE);
    }

    void insert(Node node) {
        if (size == MAX_HEAP_SIZE) {
            cout << "Heap is full!" << endl;
            return;
        }

        heap[size++] = node;
        heapifyUp(size - 1);
    }

    Node extractMin() {
        if (size <= 0) {
            Node nullNode;
            nullNode.freq = -1;
            return nullNode;
        }

        Node minNode = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
        return minNode;
    }

    int getSize() {
        return size;
    }
};

Node* createNode(char item, unsigned freq) {
    Node* temp = new Node();
    temp->item = item;
    temp->freq = freq;
    temp->left = temp->right = NULL;
    return temp;
}

Node* buildHuffmanTree(const vector<char>& items, const vector<unsigned>& freqs) {
    MinHeap minHeap;

    for (size_t i = 0; i < items.size(); ++i)
        minHeap.insert(*createNode(items[i], freqs[i]));

    while (minHeap.getSize() != 1) {
        Node *left = new Node(minHeap.extractMin());
        Node *right = new Node(minHeap.extractMin());

        Node *top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.insert(*top);
    }
    Node* root = new Node(minHeap.extractMin());
    return root;
}

void printCodes(Node* root, vector<int>& arr, int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (!(root->left) && !(root->right)) {
        cout << root->item << " \t|\t ";
        for (int i = 0; i < top; ++i)
            cout << arr[i];
        cout << endl;
    }
}


void generateCodes(const vector<char>& items, const vector<unsigned>& freqs) {
    Node* root = buildHuffmanTree(items, freqs);
    vector<int> arr(MAX_HEAP_SIZE, 0);
    printCodes(root, arr, 0);
}

char generateRandomChar() {
    return static_cast<char>('A' + rand() % 26);
}

unsigned generateRandomFreq() {
    return rand() % 1000;
}

int main() {
    srand(time(0));

    const int dataSize = 25;
    vector<char> items;
    vector<unsigned> freqs;

    for (int i = 0; i < dataSize; ++i) {
        char character = generateRandomChar();
        unsigned frequency = generateRandomFreq();
        items.push_back(character);
        freqs.push_back(frequency);
    }

    cout << "Letter\t|\tHuffman coded " << endl;
    cout << "******************************" << endl;
    generateCodes(items, freqs);

    return 0;
}
