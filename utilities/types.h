// utilities/types.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <limits>

// Enums and constants are now in the global namespace

enum class DataStructure {
    None,
    Array, Vector,
    Graph, LinkedList, DisjointSet, HashTable, Trie,
    Deque, Queue, Stack,
    AVLTree, BinarySearchTree, BinaryTree,
    FenwickTree, MaxHeap, MinHeap,
    RedBlackTree, SegmentTree
};

const std::string ARRAY_VECTOR_DISPLAY_NAME = "Array / Vector";
inline const std::vector<std::string> DATA_STRUCTURE_NAMES = {
    "None",
    ARRAY_VECTOR_DISPLAY_NAME, // For Array
    ARRAY_VECTOR_DISPLAY_NAME, // For Vector (filtered in GUI)
    "Graph","Linked List","Disjoint Set","Hash Table","Trie",
    "Deque","Queue","Stack",
    "AVL Tree","Binary Search Tree","Binary Tree",
    "Fenwick Tree","Max Heap","Min Heap",
    "Red Black Tree","Segment Tree"
};

enum class Algorithm {
    None,
    BubbleSort, HeapSort, InsertionSort, MergeSort, QuickSort, RadixSort, SelectionSort,
    BinarySearch, LinearSearch, TernarySearch,
    BellmanFord, BFS, DFS, Dijkstra, FloydWarshall, Kruskal, Prim,
    InOrder, PostOrder, PreOrder,
    HuffmanEncoding
};

inline const std::vector<std::string> ALGORITHM_NAMES = {
    "None",
    "Bubble Sort","Heap Sort","Insertion Sort","Merge Sort","Quick Sort","Radix Sort","Selection Sort",
    "Binary Search","Linear Search","Ternary Search",
    "Bellman-Ford","BFS","DFS","Dijkstra","Floyd-Warshall","Kruskal","Prim",
    "In-Order","Post-Order","Pre-Order",
    "Huffman Encoding"
};

inline const std::map<DataStructure, std::set<Algorithm>> DS_TO_ALGORITHMS_MAP = {
    {DataStructure::Array, {
        Algorithm::BubbleSort, Algorithm::HeapSort, Algorithm::InsertionSort, Algorithm::MergeSort,
        Algorithm::QuickSort, Algorithm::RadixSort, Algorithm::SelectionSort,
        Algorithm::BinarySearch, Algorithm::LinearSearch, Algorithm::TernarySearch
    }},
    {DataStructure::Vector, { // Same as Array for this map's purpose
        Algorithm::BubbleSort, Algorithm::HeapSort, Algorithm::InsertionSort, Algorithm::MergeSort,
        Algorithm::QuickSort, Algorithm::RadixSort, Algorithm::SelectionSort,
        Algorithm::BinarySearch, Algorithm::LinearSearch, Algorithm::TernarySearch
    }},
    {DataStructure::Graph, {
        Algorithm::BellmanFord, Algorithm::BFS, Algorithm::DFS, Algorithm::Dijkstra,
        Algorithm::FloydWarshall, Algorithm::Kruskal, Algorithm::Prim
    }},
    {DataStructure::LinkedList, { Algorithm::LinearSearch, Algorithm::MergeSort }},
    {DataStructure::BinarySearchTree, { Algorithm::BinarySearch, Algorithm::InOrder, Algorithm::PostOrder, Algorithm::PreOrder }},
    {DataStructure::BinaryTree, { Algorithm::InOrder, Algorithm::PostOrder, Algorithm::PreOrder, Algorithm::BFS, Algorithm::DFS }},
    {DataStructure::AVLTree, { Algorithm::InOrder, Algorithm::PostOrder, Algorithm::PreOrder, Algorithm::BinarySearch }},
    {DataStructure::RedBlackTree, { Algorithm::InOrder, Algorithm::PostOrder, Algorithm::PreOrder, Algorithm::BinarySearch }},
    {DataStructure::MaxHeap, { Algorithm::HeapSort }},
    {DataStructure::MinHeap, { Algorithm::HeapSort }},
    {DataStructure::Queue, {}},
    {DataStructure::Stack, {}},
    {DataStructure::Deque, {}},
    {DataStructure::HashTable, { Algorithm::LinearSearch }},
    {DataStructure::Trie, { Algorithm::DFS, Algorithm::BFS }},
    {DataStructure::SegmentTree, {}},
    {DataStructure::FenwickTree, {}},
    {DataStructure::DisjointSet, {}}
};

inline double evaluateSimpleComplexity(const std::string& bigO, double n) {
    if (n <= 0) n = 1e-9;
    if (bigO == "O(1)") return 1.0;
    if (bigO == "O(log n)") return std::log2(n);
    if (bigO == "O(n)") return n;
    if (bigO == "O(n log n)") return n * std::log2(n);
    if (bigO == "O(n^2)") return n * n;
    if (bigO == "O(n^3)") return n * n * n;
    if (bigO == "O(2^n)") {
        if (n > 20) return std::numeric_limits<double>::infinity();
        return std::pow(2.0, n);
    }
    return std::numeric_limits<double>::quiet_NaN();
}
