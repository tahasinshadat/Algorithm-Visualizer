import os

base = "algorithm_visualizer"

folders = {
    "": ["main.cpp", "CMakeLists.txt"],
    "common": [
        "node_tree.h", "node_list.h", "node_graph.h",
        "edge.h", "utilities.h"
    ],
    "data_structures/trees": [
        "binary_search_tree.cpp", "binary_search_tree.h",
        "binary_tree.cpp", "binary_tree.h",
        "avl_tree.cpp", "avl_tree.h",
        "red_black_tree.cpp", "red_black_tree.h",
        "segment_tree.cpp", "segment_tree.h",
        "fenwick_tree.cpp", "fenwick_tree.h"
    ],
    "data_structures/lists": [
        "linked_list.cpp", "linked_list.h",
        "skip_list.cpp", "skip_list.h"
    ],
    "data_structures/queues_stacks": [
        "stack.cpp", "stack.h",
        "queue.cpp", "queue.h",
        "deque.cpp", "deque.h"
    ],
    "data_structures/maps_sets": [
        "hash_table.cpp", "hash_table.h",
        "disjoint_set.cpp", "disjoint_set.h",
        "trie.cpp", "trie.h"
    ],
    "data_structures/graphs": [
        "graph.cpp", "graph.h"
    ],
    "algorithms/sorting": [
        "bubble_sort.cpp", "selection_sort.cpp", "insertion_sort.cpp",
        "merge_sort.cpp", "quick_sort.cpp", "heap_sort.cpp", "radix_sort.cpp"
    ],
    "algorithms/searching": [
        "binary_search.cpp", "linear_search.cpp", "ternary_search.cpp"
    ],
    "algorithms/graph": [
        "dfs.cpp", "bfs.cpp", "dijkstra.cpp", "bellman_ford.cpp",
        "floyd_warshall.cpp", "kruskal.cpp", "prim.cpp"
    ],
    "algorithms/dynamic_programming": [
        "knapsack.cpp", "lcs.cpp", "lis.cpp", "matrix_chain.cpp", "rod_cutting.cpp"
    ],
    "algorithms/number_theory": [
        "gcd.cpp", "lcm.cpp", "modular_exponentiation.cpp", "sieve.cpp", "extended_euclidean.cpp"
    ],
    "algorithms/greedy": [
        "activity_selection.cpp", "fractional_knapsack.cpp", "huffman_encoding.cpp"
    ],
    "algorithms/miscellaneous": [
        "backtracking.cpp", "recursion.cpp", "divide_and_conquer.cpp",
        "sliding_window.cpp", "two_pointers.cpp"
    ],
    "algorithms/tree_traversals": [
        "in_order.cpp", "pre_order.cpp", "post_order.cpp"
    ],
    "visualizer": [
        "renderer.cpp", "renderer.h",
        "sfml_handler.cpp", "sfml_handler.h",
        "node_drawer.cpp", "node_drawer.h",
        "animations.cpp", "animations.h",
        "ui.cpp", "ui.h"
    ],
    "assets": [],
    "tests": ["test_visualizer.cpp"]
}

for folder, files in folders.items():
    path = os.path.join(base, folder)
    os.makedirs(path, exist_ok=True)
    for file in files:
        open(os.path.join(path, file), 'a').close()
