#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "C:\Users\TX\Desktop\Data Structure\include\CharStack.h"
#include <cstdio>    // for FILE, fgetc, rewind
#include <vector>

// Huffman tree node definition
typedef struct HTNode {
    char word;
    int weight;
    int parent, lchild, rchild;
    int layer;
} HTNode, *HuffmanTree;

// Character and its weight
typedef struct {
    char ch;    // Character
    int weight; // Weight
} HuffmanCode;

void CreateHuffmanTree(HuffmanTree &HT, HuffmanCode *w, int n);
void Coding(HuffmanTree HT, int root, char **HC, CharStack &S);
void HuffmanCoding(HuffmanTree HT, int n, char **HC);
void Initialization(HuffmanTree &HT, HuffmanCode *&w, int &n);
void Encoding(HuffmanTree HT, char **HC, int n);
void Decoding(HuffmanTree HT, const char *encodedStr, int n);
int GetLayer(HuffmanTree HT, int n, int index);
void TreePrint(HuffmanTree HT, int n);
int count (FILE &fp);
int count_unique_chars(FILE *fp);
void countfrequency(HuffmanCode *&w, FILE *fp, int &n);
void printHuffmanTree(HuffmanTree HT, int nodeCount);

// Create a Huffman tree from weights array w[0..n-1]
void CreateHuffmanTree(HuffmanTree &HT, HuffmanCode *w, int n) {
    if (n <= 1 || w == nullptr) return;
    int m = 2 * n - 1;
    HT = new HTNode[m + 1];
    for (int i = 1; i <= n; i++) {
        HT[i].word = w[i - 1].ch;
        HT[i].weight = w[i - 1].weight;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    for (int i = n + 1; i <= m; i++) {
        HT[i].weight = 0;        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    for (int i = n + 1; i <= m; i++) {
        int s1 = 0, s2 = 0;
        for (int j = 1; j < i; j++) {
            if (HT[j].parent == 0) {
                if (s1 == 0 || HT[j].weight < HT[s1].weight) {
                    s2 = s1;
                    s1 = j;
                } else if (s2 == 0 || HT[j].weight < HT[s2].weight) {
                    s2 = j;
                }
            }
        }
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
}

// Generate Huffman codes for all leaf nodes using a stack
void Coding(HuffmanTree HT, int root, char **HC, CharStack &S) {
    if (!HT || root == 0) return;
    if (HT[root].lchild == 0 && HT[root].rchild == 0) {
        int len = StackLength_sq(S);
        HC[root] = new char[len + 1];
        for (int i = 0; i < len; ++i) {
            HC[root][i] = S.base[i];
        }
        HC[root][len] = '\0';
        return;
    }
    if (HT[root].lchild != 0) {
        Push_sq(S, '0');
        Coding(HT, HT[root].lchild, HC, S);
        char tmp;
        Pop_sq(S, tmp);
    }
    if (HT[root].rchild != 0) {
        Push_sq(S, '1');
        Coding(HT, HT[root].rchild, HC, S);
        char tmp;
        Pop_sq(S, tmp);
    }
}

// Generate Huffman codes for all leaf nodes
void HuffmanCoding(HuffmanTree HT, int n, char **HC) {
    CharStack S;
    InitStack_sq(S, n);
    for (int i = 1; i <= n; ++i) {
        HC[i] = nullptr;
    }
    Coding(HT, 2 * n - 1, HC, S);
    DestoryStack_sq(S);
}

// Encoding: generate Huffman codes
void Encoding(HuffmanTree HT, char **HC, int n) {
    HuffmanCoding(HT, n, HC);
}

void Decoding(HuffmanTree HT, const char *encodedStr, int n) {
    if (!HT || !encodedStr || n <= 0) {
        printf("Decoding failed: invalid input.\n");
        return;
    }
    int root = 2 * n - 1;
    int current = root;
    int i = 0, k = 0;
    char *sen = new char[100];
    for (i = 0; encodedStr[i] != '\0'; i++) {
        if (encodedStr[i] == '0') {
            current = HT[current].lchild;
        } else if (encodedStr[i] == '1') {
            current = HT[current].rchild;
        } else {
            printf("Decoding failed: invalid character in encoded string.\n");
            delete[] sen;
            return;
        }
        if (HT[current].lchild == 0 && HT[current].rchild == 0) {
            sen[k++] = HT[current].word;
            current = root; 
        }
    }
    sen[k] = '\0';
    FILE *fp = fopen("TextFile.txt", "w");
    if (!fp) {
        printf("Cannot open file TextFile.txt !\n");
        delete[] sen;
        return;
    }
    fprintf(fp, "%s", sen);
    fclose(fp);
    delete[] sen;
}

int GetLayer(HuffmanTree HT, int n, int index) {
    if (index < 1 || index > n) return -1;
    int layer = 0;
    int current = index;
    while (HT[current].parent != 0) {
        layer++;
        current = HT[current].parent;
    }
    return layer + 1; 
}

void TreePrint(HuffmanTree HT, int n) {
    if (!HT || n <= 0) {
        printf("Invalid Huffman tree.\n");
        return;
    }
    int totalNodes = 2 * n - 1;
    for (int i = 1; i <= totalNodes; i++) {
        HT[i].layer = GetLayer(HT, n, i);
    }
    printHuffmanTree(HT, totalNodes);
}

int count (FILE &fp) {
    int time = 0;
    char ch;
    char word[100] = {'\0'};
    int i;
    while ((ch = fgetc(&fp)) != EOF) {
        i  = 0;
        for (i = 0; i < 100; i++) {
            if (word[i] == ch) {
                break;
            }
        }
        if (i == 100) {
            word[time] = ch;
            time++;
        }
    }
    rewind(&fp); 
    return time;
}

// 统计文件中不同字符的个数，返回不同字符数并 rewind(fp)
int count_unique_chars(FILE *fp) {
    if (!fp) return 0;
    bool seen[256] = {false};
    int ch;
    int unique = 0;
    while ((ch = fgetc(fp)) != EOF) {
        unsigned char uc = static_cast<unsigned char>(ch);
        if (!seen[uc]) {
            seen[uc] = true;
            unique++;
        }
    }
    rewind(fp);
    return unique;
}

// 为每个不同字符构建 HuffmanCode 数组并统计权重
// w 以引用传入，函数会 new 出数组并设置 n 为字符个数
void countfrequency(HuffmanCode *&w, FILE *fp, int &n) {
    if (!fp) { w = nullptr; n = 0; return; }

    // 先统计频率到临时数组
    long freq[256] = {0};
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        unsigned char uc = static_cast<unsigned char>(ch);
        freq[uc]++;
    }

    // 统计有频率的字符数量
    std::vector<HuffmanCode> vec;
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            HuffmanCode hc;
            hc.ch = static_cast<char>(i);
            hc.weight = static_cast<int>(freq[i]);
            vec.push_back(hc);
        }
    }

    // 分配并拷贝到 w
    n = static_cast<int>(vec.size());
    if (n == 0) {
        w = nullptr;
        rewind(fp);
        return;
    }
    w = new HuffmanCode[n];
    for (int i = 0; i < n; ++i) {
        w[i] = vec[i];
    }

    rewind(fp);
}


void printHuffmanTree(HuffmanTree HT, int nodeCount) {
    if (!HT || nodeCount <= 0) return;

    int maxLayer = 0;
    for (int i = 1; i <= nodeCount; i++) {
        if (HT[i].layer > maxLayer) {
            maxLayer = HT[i].layer;
        }
    }

    int maxWidth = (1 << maxLayer) * 2; 

    for (int layer = 1; layer <= maxLayer; layer++) {
        int nodesInLayer = 1 << (layer - 1); 
        int spaceBetweenNodes = maxWidth / nodesInLayer; 

        for (int i = 0; i < spaceBetweenNodes / 2; i++) {
            std::cout << " ";
        }

        for (int i = 1; i <= nodeCount; i++) {
            if (HT[i].layer == layer) {
                if (HT[i].lchild == 0 && HT[i].rchild == 0) {
                    std::cout << HT[i].word << "(" << HT[i].weight << ")";
                } else {
                    std::cout << "*" << "(" << HT[i].weight << ")";
                }
                for (int j = 0; j < spaceBetweenNodes - 1; j++) {
                    std::cout << " ";
                }
            }
        }
        std::cout << std::endl;

        if (layer < maxLayer) {
            for (int i = 0; i < spaceBetweenNodes / 2 - 1; i++) {
                std::cout << " ";
            }
            for (int i = 1; i <= nodeCount; i++) {
                if (HT[i].layer == layer) {
                    if (HT[i].lchild != 0) {
                        std::cout << "/";
                    } else {
                        std::cout << " ";
                    }
                    for (int j = 0; j < spaceBetweenNodes - 2; j++) {
                        std::cout << " ";
                    }
                    if (HT[i].rchild != 0) {
                        std::cout << "\\";
                    } else {
                        std::cout << " ";
                    }
                    for (int j = 0; j < spaceBetweenNodes - 1; j++) {
                        std::cout << " ";
                    }
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

#endif
