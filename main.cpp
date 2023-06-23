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

    int count() {
        int count = 0;
        for (int i = 0; i < 26; i++) {
            if (children[i]) count += children[i]->count();
        }
        return count + !isEmpty();
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

    void set(const std::string& key, const T& data) {
        getNode(key)->setData(data);
    }

    int countPrefix(const std::string& key) {
        return getNode(key)->count();
    }
};

struct DictEntry {
    unsigned long pos;
    unsigned long length;
};

Trie<DictEntry>* parse_trie(const char* fileName) {
    Trie<DictEntry>* trie = new Trie<DictEntry>();

    std::ifstream stream(fileName);

    while (!stream.eof()) {
        unsigned long pos = stream.tellg();

        std::string line;
        std::getline(stream, line);
        if (!line.length()) continue;

        int key_end = line.find(']');
        std::string key = line.substr(1, key_end - 1);
        trie->set(key, {pos, line.length()});
    }

    return trie;
}


int main() {
    // char fileName[100];
    // std::cout << "file name: ";
    // std::cin >> fileName;  // entrada
    std::string fileName = "dicionario2.dic";

    Trie<DictEntry>* trie = parse_trie(fileName.c_str());

    std::cout << "b " << trie->countPrefix("b") << std::endl;
    std::cout << "s " << trie->countPrefix("s") << std::endl;
    std::cout << "se " << trie->countPrefix("se") << std::endl;
    std::cout << "sell " << trie->countPrefix("sell") << std::endl;
    std::cout << "d " << trie->countPrefix("d") << std::endl;

    return 0;
}
