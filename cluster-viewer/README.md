This code generates an HTML viewer for the clustering tree generated, similar to [this clustering of the words in a corpus of English Twitter data](http://www.ark.cs.cmu.edu/TweetNLP/cluster_viewer.html).

## Instructions

The `wcluster` tool generates a directory with a file called `paths` that contains the bit string representations of the clustering tree, e.g.

        000000  Westfalenpokalfinale    10
        000000  Heimpunktspiel  10
        000000  Jugendhallenturnier     10
        ...

The script `cluster-viewer/build-viewer.sh` creates an HTML visualization of the contents of this file. You can run it with as follows:

	./cluster-viewer/build-viewer.sh corpus.out/paths

This command creates a directory called `clusters/` containing the HTML viewer. Specify an alternative directory as follows:

	./cluster-viewer/build-viewer.sh corpus.out/paths /some/other/output-dir

## Requirements

 * Python must be in your path

## Acknowledgements

These scripts were originally written by [Brendan O'Connor](http://brenocon.com/) and extended by [Chris Dyer](http://www.cs.cmu.edu/~cdyer/).
