# Blocks-World
This program is made to solve the BlocksWorld program and implements A* Search to accomplish that.

The program can be compiled using 'make' in the terminal in the same directory as the makefile.

In order the execute the program you need the name of the executable and the filename of the problem you're wanting to solve.

For example:

  ./run probA03
  
(Note that neither the directory housing the problems nor the extension should be specified)

There are 4 heuristics implemented running the program without additional flags runs heuristic 1 (h1) by default:

  (h0): BFS
  
  (h1): Blocks out of place with a stack depth multiplier
  
  (h2): Blocks out of place weight by 2 and blocks on directly on top of their desired destination weighted by 4
  
  (h3): Count the number of neighbors out of places
 
In order to execute with a specified heuristic use the -H flag.

For example:

  ./run probA03 -H h3
  
## File structure
(Number of Stacks) (Number of Blocks) (Amount of randomization [Not applicable])
\>>>>>>>>>>>
Initial State
\>>>>>>>>>>>
End State
\>>>>>>>>>>>

### Example (probA03)
3 5 3 <br />
\>>>>>>>>>>
CE <br /> 
AD <br /> 
B <br />
\>>>>>>>>>
<br /> 
ADBC <br />
E <br />
>>>>>>>>>>

  
No known limitations the maximum number of iterations for A* Search is set at 200,000.
