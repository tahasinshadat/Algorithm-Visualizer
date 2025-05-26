// types.h
#pragma once
#include <string>
#include <vector>

enum class DataStructure {
    None,
    Graph, LinkedList, DisjointSet, HashTable, Trie,
    Deque, Queue, Stack,
    AVLTree, BinarySearchTree, BinaryTree,
    FenwickTree, MaxHeap, MinHeap,
    RedBlackTree, SegmentTree
};
inline const std::vector<std::string> DATA_STRUCTURE_NAMES = {
    "None","Graph","Linked List","Disjoint Set","Hash Table","Trie",
    "Deque","Queue","Stack",
    "AVL Tree","Binary Search Tree","Binary Tree",
    "Fenwick Tree","Max Heap","Min Heap","Red Black Tree","Segment Tree"
};

enum class Algorithm {
    None,
    BellmanFord, BFS, DFS, Dijkstra, FloydWarshall, Kruskal, Prim,
    HuffmanEncoding,
    BinarySearch, LinearSearch, TernarySearch,
    BubbleSort, HeapSort, InsertionSort, MergeSort, QuickSort, RadixSort, SelectionSort,
    InOrder, PostOrder, PreOrder
};
inline const std::vector<std::string> ALGORITHM_NAMES = {
    "None",
    "Bellman-Ford","BFS","DFS","Dijkstra","Floyd-Warshall","Kruskal","Prim",
    "Huffman Encoding",
    "Binary Search","Linear Search","Ternary Search",
    "Bubble Sort","Heap Sort","Insertion Sort","Merge Sort",
    "Quick Sort","Radix Sort","Selection Sort",
    "In-Order","Post-Order","Pre-Order"
};
