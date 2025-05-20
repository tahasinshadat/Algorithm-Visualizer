# Makefile (cross-platform)
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
TARGET := algorithm_visualizer
EXE_EXT := .exe

# Manually listed source files (unchanged)
SRC := \
main.cpp \
tests/test_visualizer.cpp \
visualizer/renderer.cpp \
visualizer/sfml_handler.cpp \
visualizer/node_drawer.cpp \
visualizer/animations.cpp \
visualizer/ui.cpp \
data_structures/trees/binary_tree.cpp \
data_structures/trees/binary_search_tree.cpp \
data_structures/trees/avl_tree.cpp \
data_structures/trees/red_black_tree.cpp \
data_structures/trees/segment_tree.cpp \
data_structures/trees/fenwick_tree.cpp \
data_structures/lists/linked_list.cpp \
data_structures/lists/skip_list.cpp \
data_structures/queues_stacks/stack.cpp \
data_structures/queues_stacks/queue.cpp \
data_structures/queues_stacks/deque.cpp \
data_structures/maps_sets/hash_table.cpp \
data_structures/maps_sets/disjoint_set.cpp \
data_structures/maps_sets/trie.cpp \
data_structures/graphs/graph.cpp \
algorithms/sorting/bubble_sort.cpp \
algorithms/sorting/selection_sort.cpp \
algorithms/sorting/insertion_sort.cpp \
algorithms/sorting/merge_sort.cpp \
algorithms/sorting/quick_sort.cpp \
algorithms/sorting/heap_sort.cpp \
algorithms/sorting/radix_sort.cpp \
algorithms/searching/binary_search.cpp \
algorithms/searching/linear_search.cpp \
algorithms/searching/ternary_search.cpp \
algorithms/graph/dfs.cpp \
algorithms/graph/bfs.cpp \
algorithms/graph/dijkstra.cpp \
algorithms/graph/bellman_ford.cpp \
algorithms/graph/floyd_warshall.cpp \
algorithms/graph/kruskal.cpp \
algorithms/graph/prim.cpp \
algorithms/dynamic_programming/knapsack.cpp \
algorithms/dynamic_programming/lcs.cpp \
algorithms/dynamic_programming/lis.cpp \
algorithms/dynamic_programming/matrix_chain.cpp \
algorithms/dynamic_programming/rod_cutting.cpp \
algorithms/number_theory/gcd.cpp \
algorithms/number_theory/lcm.cpp \
algorithms/number_theory/modular_exponentiation.cpp \
algorithms/number_theory/sieve.cpp \
algorithms/number_theory/extended_euclidean.cpp \
algorithms/greedy/activity_selection.cpp \
algorithms/greedy/fractional_knapsack.cpp \
algorithms/greedy/huffman_encoding.cpp \
algorithms/miscellaneous/backtracking.cpp \
algorithms/miscellaneous/recursion.cpp \
algorithms/miscellaneous/divide_and_conquer.cpp \
algorithms/miscellaneous/sliding_window.cpp \
algorithms/miscellaneous/two_pointers.cpp \
algorithms/tree_traversals/in_order.cpp \
algorithms/tree_traversals/pre_order.cpp \
algorithms/tree_traversals/post_order.cpp

.PHONY: build run clean

build: clean
	@echo "Cleaning..."
	@rm -f $(TARGET)$(EXE_EXT)
	@echo "Compiling..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)$(EXE_EXT)
	@echo "Build complete: ./$(TARGET)$(EXE_EXT)"

run:
	@./$(TARGET)$(EXE_EXT)

clean:
	@rm -f $(TARGET)$(EXE_EXT)
