#include <bits/stdc++.h>
using namespace std;

//Ukkonen's Algorithm
struct SuffixTree {
    struct Node {
        map<char, Node*> next;
        Node *suffixLink;
        int start, *end;
        int suffixIndex;
        Node(int s, int *e) : start(s), end(e), suffixLink(nullptr), suffixIndex(-1) {}
    };

    string text;
    Node *root;
    Node *lastNewNode;
    Node *activeNode;

    int remainingSuffixCount;
    int activeEdge, activeLength;
    int leafEnd;
    int *rootEnd, *splitEnd;
    int size;

    SuffixTree(const string &s) {
        text = s;
        size = s.size();
        rootEnd = new int(-1);
        root = new Node(-1, rootEnd);
        activeNode = root;
        build();
    }

    int edgeLength(Node *n) {
        return *(n->end) - n->start + 1;
    }

    bool walkDown(Node *n) {
        if (activeLength >= edgeLength(n)) {
            activeEdge += edgeLength(n);
            activeLength -= edgeLength(n);
            activeNode = n;
            return true;
        }
        return false;
    }

    void extend(int pos) {
        leafEnd = pos;
        remainingSuffixCount++;
        lastNewNode = nullptr;

        while (remainingSuffixCount > 0) {
            if (activeLength == 0) activeEdge = pos;

            if (activeNode->next.find(text[activeEdge]) == activeNode->next.end()) {
                activeNode->next[text[activeEdge]] = new Node(pos, &leafEnd);
                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
            } else {
                Node *next = activeNode->next[text[activeEdge]];
                if (walkDown(next)) continue;

                if (text[next->start + activeLength] == text[pos]) {
                    if (lastNewNode != nullptr && activeNode != root) {
                        lastNewNode->suffixLink = activeNode;
                        lastNewNode = nullptr;
                    }
                    activeLength++;
                    break;
                }

                splitEnd = new int(next->start + activeLength - 1);
                Node *split = new Node(next->start, splitEnd);
                activeNode->next[text[activeEdge]] = split;

                split->next[text[pos]] = new Node(pos, &leafEnd);
                next->start += activeLength;
                split->next[text[next->start]] = next;

                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = split;
                }
                lastNewNode = split;
            }

            remainingSuffixCount--;
            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = pos - remainingSuffixCount + 1;
            } else if (activeNode != root) {
                activeNode = activeNode->suffixLink ? activeNode->suffixLink : root;
            }
        }
    }

    void build() {
        leafEnd = -1;
        remainingSuffixCount = 0;
        activeEdge = -1;
        activeLength = 0;

        for (int i = 0; i < size; i++) extend(i);
    }

    void setSuffixIndex(Node *n, int labelHeight) {
        if (n == nullptr) return;
        bool leaf = true;
        for (auto &p : n->next) {
            leaf = false;
            setSuffixIndex(p.second, labelHeight + edgeLength(p.second));
        }
        if (leaf) {
            n->suffixIndex = size - labelHeight;
        }
    }
};

int main() {
    string s = "banana$";
    SuffixTree st(s);
    st.setSuffixIndex(st.root, 0);
    cout << "Suffix tree built for " << s << "\n";
}
