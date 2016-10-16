Solve the GCCF problem on ISGs
===================

This is an implementation of the algorithm presented by Thomas Voice, Maria Polukarov, and Nicholas R. Jennings in “[Coalition Structure Generation over Graphs](https://www.jair.org/media/3715/live-3715-6552-jair.pdf)”, Journal of Artificial Intelligence Research (JAIR), pages 165-196, 2012.

Execution
----------
Voice must be executed by means of the [`voice.sh`](voice.sh) script, i.e.,
```
./voice.sh -i <filename> -o <filename>

-g	Input graph filename
-p	Outputs solution to file
```

Acknowledgements
----------
[LibTW](http://www.treewidth.com/treewidth) is employed to compute a tree decomposition of the input graph.
