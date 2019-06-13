# Graph-Theory-Project-Template
Project template of course - Graph Theory (EE6622E) in National Cheng Kung University.

## How to run

### Linux

```
# clone first (with submodule)
$ git clone --recursive https://github.com/<your-name>/Graph-Theory-Project-Template.git
# (Option) build libfakemn.a
$ cd fake-mininet && make lib && cd .. && cp fake-mininet/libfakemn.a .
# build
$ make 
# run 
$ ./main.out "FileName"
# generate png file
$ make png
```

### Windows

If your PC is `window 10`, then you need to install the related dependencies. For example, if you are using `VSCode` as your IDE, then you can install the plugin of `C/C++` in your vscode. And then install the following programs/packages:
* mingw
* git

Step by step to create `c_cpp_properties.json` and `tasks.json`:
* `Ctrl+Shift+P` -> `C/C++: Edit Configuration` -> then you have the first JSON file.
* `Ctrl+Shift+P` -> `Tasks: Configure Task` -> then you have the second JSON file.

Here is the setting of `c_cpp_properties.json` and `tasks.json`:
```json
# c_cpp_properties.json (If your mingw installed in another folder, then you have to change the value in `compilterPath`)
{
    "configurations": [
        {
            "name": "MinGW",
            "intelliSenseMode": "gcc-x64",
            "includePath": [
                "$(workspaceFolder)"
            ],
            "compilerPath": "C:/mingw/bin/gcc.exe",
            "cStandard": "c11",
            "cppStandard": "c++11"
        }
    ],
    "version": 4
}

# tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "g++",
            "args": [
                "-g",
                "main.cc",
                "-o",
                "main.exe",
                "-Ifake-mininet/lib",
                "-g3",
                "-L.",
                "-lfakemn_win",
                "-std=c++11"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

After all the settings have been done, we can press `Ctrl+Shift+B` to run the task, if your settings is correct, then there will have an executable file which named `main.exe`.

Or you can just [DOWNLOAD a pre-built VM images](http://gofile.me/39GpL/XU5tznyO6) provided by TA.

> [Guide of environment setting](https://hackmd.io/-5WZQC-1QqOeV3KUX65tEw?view) on Windows.

## TODO - Your descriptions about solutions/algorithms/results

### Abstrct
Chinese postman problem is a problem about how to travel all edges and back to one start node in a graph.  
What I've done in this project is a bi-directional and no-weighted version.  
**Note: The input file must be a simple graph.**  

### Results & Files
1. `main.cc` The main program  
2. `graph` The demo graph in project slide  
3. `graph.dot` The dot file of original graph 
4. `graph.png` The png file of original graph 
5. `result.txt` The txt file of final result  
6. `result.dot` The dot file of final result  
7. `result.png` The png file of final result  

### Algorithms & Solutions
1. Find out all nodes with odd degree in this graph. And put them into a vector called `odd_nodes`.
2. Find the shortest paths between every two odd nodes. In this step, I use a double vector being a matrix to store the shortest path length, called `shortest_path_length`. For example, shortest_path_length[i][j] = the shortest length from *ith* odd node to *jth* odd node. As for finding shortest paths and calculating minimum lengths of two odd nodes. I use the `find_path` function which is provided by TA's API.
3. Find a minimum-length pairwise matching of all odd nodes. In this step, I write a recursive function `Perm` to prodece the all possible permutation of odd nodes. According to this, we can easily find the optimal pairwise matching in function `find_optimal_pairs`.
4. For each matching in 3, add the shortest paths to the original graph.
5. Find Euler circuit in the modified graph. In this step, I write `find_euler_circuit` by Hierholzer’s algorithm. It's an algorithm can find the Euler circuit if all nodes in the graph had even degree. First, put an arbitrary node to a stack `tempPath`. Second, if there were any unmarked incident edge next to `tempPath`'s top node, mark the incident edge and push its neighbor to `tempPath`. Third, if there were NO unmarked incident edge with `tempPath`'s top node, push it to a stack `finalPath` and pop `tempPath`. Finally, we can get an Euler circuit in `finalPath` if the `tempPath` is empty.
6. The final answer is just the Euler circuit we find.
