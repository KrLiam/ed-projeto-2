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
    std::string fileName;
    std::cin >> fileName;

    Trie<DictEntry>* trie = parse_trie(fileName.c_str());

    while (1) {
        std::string word;
        std::cin >> word;

        if (word.length() == 1 && word[0] == '0') break;

        int count = trie->countPrefix(word);
        if (count > 0) {
            std::cout << word << " is prefix of " << count << " words" << std::endl;
        } else {
            std::cout << word << " is not prefix" << std::endl;
        }

        DictEntry entry = trie->get(word);
        if (entry.length) {
            std::cout << word << " is at (" << entry.pos << "," << entry.length << ")" << std::endl;
        }
    }

    return 0;
}
