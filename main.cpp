#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

struct Node {
    char data;
    unsigned freq;
    Node* left, *right;
};

Node* createNode(char data, unsigned freq) {
    Node* temp = new Node();
    temp->left = temp->right = nullptr;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

struct compare {
    bool operator()(Node* l, Node* r) {
        return (l->freq > r->freq);
    }
};

Node* buildHuffmanTree(const vector<char>& items, const vector<unsigned>& freqs) {
    priority_queue<Node*, vector<Node*>, compare> pq;

    for (size_t i = 0; i < items.size(); ++i)
        pq.push(createNode(items[i], freqs[i]));

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        pq.push(top);
    }
    return pq.top();
}

void printHuffmanCodes(Node* root, string str, map<char, string>& huffmanCodes) {
    if (!root)
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

    cout << "Initial size: " << initialSize << " byes(octets)" << endl;
    cout << "Encoded size: " << encodedSize << " bits" << endl;
    cout << "Compression ratio: " << ((initialSize * 8) / static_cast<double>(encodedSize)) << "x" << endl;

    return 0;
}
