// graphics/complexity_handler.cpp
#include "complexity_handler.h"

// Using declarations for std types in .cpp file
using std::string;
using std::pair;
using std::map;
// No custom namespace for av::ComplexityHandler

ComplexityHandler::ComplexityHandler() {
    initializeComplexities();
}

void ComplexityHandler::initializeComplexities() {
    // Algorithm is now global enum from types.h
    complexitiesMap.clear(); // Using renamed member variable

    complexitiesMap[Algorithm::BubbleSort] = {"O(n^2)", "O(1)"};
    complexitiesMap[Algorithm::HeapSort] = {"O(n log n)", "O(1)"};
    complexitiesMap[Algorithm::InsertionSort] = {"O(n^2)", "O(1)"};
    complexitiesMap[Algorithm::MergeSort] = {"O(n log n)", "O(n)"};
    complexitiesMap[Algorithm::QuickSort] = {"O(n log n)", "O(log n)"};
    complexitiesMap[Algorithm::RadixSort] = {"O(d(n+k))", "O(n+k)"};
    complexitiesMap[Algorithm::SelectionSort] = {"O(n^2)", "O(1)"};

    complexitiesMap[Algorithm::BinarySearch] = {"O(log n)", "O(1)"};
    complexitiesMap[Algorithm::LinearSearch] = {"O(n)", "O(1)"};
    complexitiesMap[Algorithm::TernarySearch] = {"O(log n)", "O(1)"};

    complexitiesMap[Algorithm::BellmanFord] = {"O(VE)", "O(V)"};
    complexitiesMap[Algorithm::BFS] = {"O(V+E)", "O(V)"};
    complexitiesMap[Algorithm::DFS] = {"O(V+E)", "O(V)"};
    complexitiesMap[Algorithm::Dijkstra] = {"O(E log V)", "O(V+E)"};
    complexitiesMap[Algorithm::FloydWarshall] = {"O(V^3)", "O(V^2)"}; // Changed to V^3 assuming V is n
    complexitiesMap[Algorithm::Kruskal] = {"O(E log E)", "O(V+E)"};
    complexitiesMap[Algorithm::Prim] = {"O(E log V)", "O(V+E)"};

    complexitiesMap[Algorithm::InOrder] = {"O(n)", "O(h)"};
    complexitiesMap[Algorithm::PostOrder] = {"O(n)", "O(h)"};
    complexitiesMap[Algorithm::PreOrder] = {"O(n)", "O(h)"};

    complexitiesMap[Algorithm::HuffmanEncoding] = {"O(n log n)", "O(n)"};
    complexitiesMap[Algorithm::None] = {"N/A", "N/A"};
}

// Return type uses std::pair
std::pair<string, string> ComplexityHandler::getComplexity(Algorithm alg) const {
    // Algorithm is now global enum
    auto it = complexitiesMap.find(alg); // Using renamed member variable
    if (it != complexitiesMap.end()) { // Using renamed member variable
        return it->second;
    }
    return {"N/A", "N/A"};
}
