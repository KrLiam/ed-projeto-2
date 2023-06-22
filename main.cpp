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

struct TrieNode {
    unsigned long pos;
    unsigned long length;
    TrieNode* children[26];

    TrieNode() : pos(0), length(0), children() {}

    TrieNode(unsigned long pos, unsigned long length)
        : pos(pos), length(length), children() {}
    
    ~TrieNode() {
        for (int i = 0; i < 26; i++) {
            if (children[i]) delete children[i];
        }
    }


    TrieNode* getChild(char ch) {
        int i = ch - 'a';

        if (!children[i]) {
            children[i] = new TrieNode();
        }
        return children[i];
    }
};

class Trie {
    TrieNode* root;

public:

    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        delete root;
    }

    // get(const std::string& key) {
    // }
}

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

int main() {
    // char fileName[100];
    // std::cout << "file name: ";
    // std::cin >> fileName;  // entrada
    std::string fileName = "dicionario1.dic";

    parse_trie(fileName.c_str());

    return 0;
}
