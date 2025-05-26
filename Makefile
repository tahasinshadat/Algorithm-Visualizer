# Makefile (cross-platform)
CXX := g++
INCLUDES := -Igraphics -Iutilities
CXXFLAGS := -std=c++17 -Wall -Wextra $(INCLUDES)
TARGET := algorithm_visualizer
EXE_EXT := .exe

SFML := -lsfml-graphics -lsfml-window -lsfml-system

SRC := \
main.cpp \
graphics/renderer.cpp \
graphics/visualization_handler.cpp \
graphics/visualizer.cpp \
graphics/complexity_handler.cpp \
graphics/gui.cpp \
data_structures/graphs/graph.cpp \
data_structures/lists/linked_list.cpp \
data_structures/maps_sets/disjoint_set.cpp \
data_structures/maps_sets/hash_table.cpp \
data_structures/maps_sets/trie.cpp \
data_structures/queues_stacks/deque.cpp \
data_structures/queues_stacks/queue.cpp \
data_structures/queues_stacks/stack.cpp \
data_structures/trees/avl_tree.cpp \
data_structures/trees/binary_search_tree.cpp \
data_structures/trees/binary_tree.cpp \
data_structures/trees/fenwick_tree.cpp \
data_structures/trees/max_heap.cpp \
data_structures/trees/min_heap.cpp \
data_structures/trees/red_black_tree.cpp \
data_structures/trees/segment_tree.cpp \
algorithms/graph/bellman_ford.cpp \
algorithms/graph/bfs.cpp \
algorithms/graph/dfs.cpp \
algorithms/graph/dijkstra.cpp \
algorithms/graph/floyd_warshall.cpp \
algorithms/graph/kruskal.cpp \
algorithms/graph/prim.cpp \
algorithms/greedy/huffman_encoding.cpp \
algorithms/searching/binary_search.cpp \
algorithms/searching/linear_search.cpp \
algorithms/searching/ternary_search.cpp \
algorithms/sorting/bubble_sort.cpp \
algorithms/sorting/heap_sort.cpp \
algorithms/sorting/insertion_sort.cpp \
algorithms/sorting/merge_sort.cpp \
algorithms/sorting/quick_sort.cpp \
algorithms/sorting/radix_sort.cpp \
algorithms/sorting/selection_sort.cpp \
algorithms/tree_traversals/in_order.cpp \
algorithms/tree_traversals/post_order.cpp \
algorithms/tree_traversals/pre_order.cpp

.PHONY: build run clean

build: clean
	@echo "Cleaning..."
	@rm -f $(TARGET)$(EXE_EXT)
	@echo "Compiling..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)$(EXE_EXT) $(SFML)
	@echo "Build complete: ./$(TARGET)$(EXE_EXT)"

run:
	@./$(TARGET)$(EXE_EXT)

clean:
	@rm -f $(TARGET)$(EXE_EXT)