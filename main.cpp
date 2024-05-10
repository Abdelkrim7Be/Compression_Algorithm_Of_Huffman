#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

struct Node {
    char data;
    unsigned freq;
    Node* next;
    Node* left;
    Node* right;
};

Node* createNode(char data, unsigned freq) {
    Node* temp = new Node();
    temp->data = data;
    temp->freq = freq;
    temp->next = nullptr;
    temp->left = nullptr;
    temp->right = nullptr;
    return temp;
}

void insertSorted(Node** head, Node* newNode) {
    if (*head == nullptr || (*head)->freq >= newNode->freq) {
        newNode->next = *head;
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != nullptr && current->next->freq < newNode->freq) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

Node* buildHuffmanTree(const vector<char>& items, const vector<unsigned>& freqs) {
    Node* head = nullptr;
    for (size_t i = 0; i < items.size(); ++i) {
        Node* newNode = createNode(items[i], freqs[i]);
        insertSorted(&head, newNode);
    }

    while (head->next != nullptr) {
        Node* left = head;
        Node* right = head->next;
        head = head->next->next;

        Node* top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertSorted(&head, top);
    }

    return head;
}

void printHuffmanCodes(Node* root, string str, map<char, string>& huffmanCodes) {
    if (root == nullptr)
        return;

    if (root->data != '$')
        huffmanCodes[root->data] = str;

    printHuffmanCodes(root->left, str + "0", huffmanCodes);
    printHuffmanCodes(root->right, str + "1", huffmanCodes);
}

string encodeText(const string& text, const map<char, string>& huffmanCodes) {
    string encodedText = "";
    for (char c : text)
        encodedText += huffmanCodes.at(c);
    return encodedText;
}

void writeEncodedTextToFile(const string& encodedText, const string& fileName) {
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << encodedText;
        outFile.close();
        cout << "Encoded text written to file: " << fileName << endl;
    }
    else {
        cerr << "Unable to open file for writing." << endl;
    }
}

void serializeHuffmanTree(Node* root, ofstream& outFile) {
    if (root == nullptr) {
        outFile << "N\n";
        return;
    }

    outFile << root->data << ":" << root->freq << "\n";
    serializeHuffmanTree(root->left, outFile);
    serializeHuffmanTree(root->right, outFile);
}


Node* deserializeHuffmanTree(ifstream& inFile) {
    string line;
    getline(inFile, line);
    if (line == "N") {
        return nullptr;
    }

    istringstream iss(line);
    char data;
    unsigned freq;
    iss >> data;
    iss.ignore();
    iss >> freq;

    Node* root = createNode(data, freq);
    root->left = deserializeHuffmanTree(inFile);
    root->right = deserializeHuffmanTree(inFile);

    return root;
}

int main() {

cout<<  "___                    .-.       .-."   << endl;
cout << "(   )                  /    \    /    \       " << endl;
cout << " | | .-.    ___  ___   | .`. ;   | .`. ;   ___ .-. .-.     .---.   ___ .-. " << endl;
cout << " | |/   \  (   )(   )  | |(___)  | |(___) (   )   '   \   / .-, \ (   )   \ "<< endl;
cout << "|  .-. .   | |  | |   | |_      | |_      |  .-.  .-. ; (__) ; |  |  .-. . "<< endl;
cout << "| |  | |   | |  | |  (   __)   (   __)    | |  | |  | |   .'`  |  | |  | | "<< endl;
cout << "| |  | |   | |  | |   | |       | |       | |  | |  | |  / .'| |  | |  | | "<< endl;
cout << "| |  | |   | |  | |   | |       | |       | |  | |  | | | /  | |  | |  | | "<< endl;
cout << "| |  | |   | |  ; '   | |       | |       | |  | |  | | ; |  ; |  | |  | | "<< endl;
cout << "| |  | |   ' `-'  /   | |       | |       | |  | |  | | ' `-'  |  | |  | | "<< endl;
cout << "(___)(___)   '.__.'   (___)     (___)     (___)(___)(___)`.__.'_. (___)(___) "<< endl;


cout << "\n\n\n\n";

    string paragraph = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                       "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
                       "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. "
                       "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
                       "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    cout << "Paragraph à compresser:" << endl;
    cout << paragraph << endl << endl;

    map<char, unsigned> freqMap;
    for (char c : paragraph)
        freqMap[c]++;

    vector<char> items;
    vector<unsigned> freqs;
    for (const auto& entry : freqMap) {
        items.push_back(entry.first);
        freqs.push_back(entry.second);
    }

    Node* root = buildHuffmanTree(items, freqs);

    map<char, string> huffmanCodes;
    printHuffmanCodes(root, "", huffmanCodes);

    string encodedText = encodeText(paragraph, huffmanCodes);

    writeEncodedTextToFile(encodedText, "encoded_text.txt");

    size_t initialSize = paragraph.size();
    size_t encodedSize = encodedText.size();

    cout << "Initial size: " << initialSize << " bytes(octets)" << endl;
    cout << "Encoded size: " << encodedSize << " bits" << endl;
    cout << "Compression ratio: " << ((initialSize * 8) / static_cast<double>(encodedSize)) << "x" << endl;

    ofstream outFile("huffman_tree.txt");
    serializeHuffmanTree(root, outFile);
    outFile.close();

    ifstream inFile("huffman_tree.txt");
    Node* deserializedRoot = deserializeHuffmanTree(inFile);
    inFile.close();

    cout << "Deserialized Huffman tree:" << endl;
    printHuffmanCodes(deserializedRoot, "", huffmanCodes);
    for (const auto& code : huffmanCodes) {
    cout << code.first << " : " << code.second << endl;
}


    return 0;
}
