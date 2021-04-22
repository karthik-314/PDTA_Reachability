# Requirements
1. A C++ compiler with decent C++17 support (Clang >= 3.6 or GNU g++ >= 6 should work. Apple LLVM >= 10.0.0 works)
2. CMake (>= 2.8)
3. flex (>= 2.5.35)
4. bison (>= 3.0.4)
5. The Boost library (>= 1.65.0 -- probably works with earlier versions)
6. Doxygen (>= 1.8.15 -- probably works with earlier versions)
7. Catch2 (>= 2.7.0)

# Clone Repo
git clone https://github.com/karthik-314/TPDA-Zone-Reachability.git

# Build
1. cmake ../PDTA_Reachability -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
2. make -j5
3. make doc
4. sudo make install (Password : root)

# Executing
1. In order to execute the benchmarks simply run, 
	cd MyExamples/
2. For equivalence run,
	bash examples.sh eq
3. For simulation run,
	bash examples.sh sim
4. For running a specific example, say B2_10.txt, run
	tchecker <graph_controller> B2_10.txt
	Where <graph_controller> can either be "sim", or "eq" denoting simulation or equivalence respectively.

# File Format:
1. A documented sample file format has been given in MyExamples/sample.txt
2. The file format is similar to the TChecker file format with a few crucial differences like, no source invariants, clocks must be reset to 0 and not to any other value, and there can be only one process.

# Output Format:
The output will be of the format "[nodes, time(micro), states]", with nodes denoting the number of nodes in the entire graph, time denoting the total execution time in microseconds, and states denoting the set of states that are reachable via some well nested path in the PDTA.