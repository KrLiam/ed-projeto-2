#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include <cctype>
#include <cstring>


std::string read_file(
    const char* fileName, unsigned long offset = 0, unsigned long length = 0
) {    
    std::ifstream t(fileName);

    if (!length) {
        t.seekg(offset, std::ios::end);
        length = t.tellg();
        length -= offset;
    }

    std::string buffer(length, ' ');

    t.seekg(offset);
    t.read(&buffer[0], length);

    return buffer;
}

template<typename T>
class TrieNode {
    T data;
    bool empty;
    TrieNode* children[26];

public:

    TrieNode() : data(), empty(true), children() {}

    TrieNode(T& data) : data(data), empty(false), children() {}
    
    ~TrieNode() {
        for (int i = 0; i < 26; i++) {
            if (children[i]) delete children[i];
        }
    }

    bool isEmpty() {
        return empty;
    }

    const T& getData() {
        return data;
    }

    void setData(const T& value) {
        data = value;
        empty = false;
    }

    TrieNode* getChild(char ch) {
        int i = ch - 'a';

        if (!children[i]) {
            children[i] = new TrieNode();
        }
        return children[i];
    }
};

template<typename T>
class Trie {
    TrieNode<T>* root;

    TrieNode<T>* getNode(const std::string& key) {
        TrieNode<T>* node = root;
        for (char ch : key) {
            node = node->getChild(ch);
        }
        return node;
    }

public:

    Trie() {
        root = new TrieNode<T>();
    }

    ~Trie() {
        delete root;
    }

    const T& get(const std::string& key) {
        return getNode(key)->getData();
    }
};

void parse_trie(const char* fileName) {
    std::string str = read_file(fileName);

    std::cout << str.length() << std::endl;

    unsigned long pos = 0;

    while (pos < str.length()) {
        unsigned long end_pos = str.find('\n', pos);
        if (end_pos == std::string::npos) {
            end_pos = str.length();
        }

        pos++; // pula [

        unsigned long key_end_pos = str.find(']', pos);

        std::string key = str.substr(pos, key_end_pos - pos);
        unsigned long start_pos = key_end_pos + 1;
        unsigned long lenght = end_pos - key_end_pos - 1;

        pos = end_pos + 1; // pula caractere de nova linha
    }
}


struct DictEntry {
    unsigned long pos;
    unsigned long length;
};

int main() {
    // char fileName[100];
    // std::cout << "file name: ";
    // std::cin >> fileName;  // entrada
    std::string fileName = "dicionario1.dic";

    parse_trie(fileName.c_str());

    Trie<DictEntry> trie;

    auto data = trie.get("word");
    data.pos = 123;
    data.length = 49;

    return 0;
}
