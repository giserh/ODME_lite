Origin-Destination Multi-Edge Package. Module 1: Network analyzer
========================================================================

 Copyright 2014 Oleguer Sagarra. All rights reserved. Code under License GPLv3.
______________________________________________________________________________________

The present program may be used to analyze any kind of weighted networks with integer weights and (optionally) with a provided general cost matrix.
If you use our software, please do cite us.


## References 

[1] Supersampling and network reconstruction of urban mobility.
	Sagarra, O., Szell, M., Santi, P., and Ratti, C. Arxiv Preprint (2015)
	[arXiv:1504.01939v1](http://arxiv.org/abs/1504.01939)
    [PLoS One 10, e0134508 (2015)](http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0134508)

## Requirements and Installation

  You only need to install the GSL library: http://www.gnu.org/software/gsl/
  
  How to tutorial: http://www.brianomeara.info/tutorials/brownie/gsl

## Compilation

  Simply run:

    $ make 

  The executable created is called **MultiEdgeAnalyzer**


## Execution

The executable is called **MultiEdgeAnalyzer**. The program has a set of options. To introduce on option you must introduce first its option key (preceded by a dash), one white space and then the argument value. The only compulsory option is the input network file (-f) in weighted adjacency list format, the number of nodes (-N) and the directed/undirected option (-d).
Arguments can appear in any order. If an argument does not appear the program gets the default value:

Examples
______________
 Analyze the given network file *sample.tr* with *N=94* nodes and a provided cost matrix *cost_matrix.dists* ignoring the first line of the files for the header.
 	
 	$ ./MultiEdgeAnalyzer -N 1000 -d 1 -f ../tests/sample.tr -z 1 -a ../tests/cost_matrix.dists  -h 1 
 
 
## Options and Arguments

#### Options
```
 *  Compulosry items:
 *		-N N_nodes. Number of nodes (int)
 *		-d dir_opt. Undirected (0) or Directed (1)
 *		-f file_adj Path to file with adj format node_i node_j t_ij
 *  Optional items: 
 *		-z Distance_option: Include distances in analysis? [default=0, 1 for yes]
 *		-a file_dist Path to distance list in format node_i node_j d_ij [default, no distance]
 *		-s seed.initial seed for random generator (int) (default=1)
 *		-x Exponent for log-binning (-1 for no log binning) (Default=-1)
 *		-v Verbose (1 for on, 0 for off) (Default 0)
 *		-c Clustering option (1 for yes) (warning: Depending on av_s makes simulations orders of magnitude slower) (Default=0)
 *		-l Self-loop option (>0 for accepting them) (Default =1) 
 *		-h Number of header lines in input files (default=1)
 *		-m Maximum distance for binning (default= 20000) [in meters]
 *		-L Log-dist option (to compute the logarithm of the cost matrix) [default=0]
 *     -F Implement Graph-filtering (applies Filter to graph according to null model with fixed strengths) [default=0]
 *         -C case Null model type (ME=0, B=1, W=2) [default=0]
 *         -M layers Number of layers [default=1] 
 *         -Z alpha Confidence level [default=0.95] 
 *         -X Path to file containing lagrange multiplier values xy
```

## Brief description of the Multi-Edge Network Analyzer

This program analyzes from a topological perespective any given weighted network with integer weights. It provides statistics related with the weight distrbiution, node attributes and link dependence with associated costs.

## Outputs

For both directed and undirected cases:

The generated outputs are files with extension *.hist* if they correspond to network statistics (binned) and *.list* for the node list with different features.


**.list files**
   The file *node_list.list* contains different node features.
   For the undirected case: (Note that the clustering only appears if -c flag is set)
   

```
Node_num k k_analitic s Y2 k_nn k^w_nn s^w_nn (optionally  clust c^w)
```
Explicit formulas: (in latex)
```
	k_i = \sum \Theta(t_ij)
	k^{anal}_i = \sum (1-e^{-s_is_j/T})
	s_i = \sum t_{ij}
	Y2 = \sum t_ij^2 / s_i
	k_nn_i = \sum \Theta(t_{ij}) k_j / k_i
	k^w_nn_i = \sum t_{ij} k_j /s_i
	s^w_nn = \sum t_{ij} s_j / s_i
	c = \sum_{jk} \Theta(t_ij)\Thetat(t_jk)\Theta(t_ki) / k_i(k_i-1)
	c^w = \sum_{jk} (t_ij + t_ik) \Theta(t_ij) \Thetat(t_jk)\Theta(t_ki) / [2s_i(k_i-1)]
```

For the directed case: (In this case the clustering is not defined)

```
Node_num k k_analitic s Y2 k_nn k^w_nn s^w_nn 
```
First for the out case, then for the in case. 

Explicit formulas (in latex):

```
Out:
	k_i^{out} = \sum_j \Theta(t_ij)
	k^{anal}_i^{out} = \sum_j (1-e^{-s_i^out}s_j^{in}/T})
	s_i^{out} = \sum_j t_{ij}, 
	Y2^{out} = \sum_j t_ij^2 / s_i^{out}
	k_nn_i^{out} = \sum_j \Theta(t_{ij}) k_j^{in} / k_i^{out}
	k^w_nn_i = \sum_j t_{ij} k_j^{in} /s_i^{out}, 
	s^w_nn = \sum_j t_{ij} s_j^{in} / s_i^{out}
In:
	k_j^{in} = \sum_j \Theta(t_ij)
	k^{anal}_j = \sum_j (1-e^{-s_i^out s_j^in/T})
	s_j^in = \sum_i t_{ij}
	Y2^{in} = \sum_i t_ij^2 / s_j^{in}
	k_nn^{in} = \sum_i \Theta(t_{ij}) k_i^{out} / k_j^{in}
	k^w_nn = \sum_i t_{ij} k_i^{out} /s_j^{in}, 
	s^w_nn^{in} = \sum_i t_{ij} s_i^{out} / s_j^{in}
```

**w_s_io.hist, p_s_io.hist, wp_s_io.hist and wp_k_io.hist files**

This files contain the graph-average occupation number, existing occupation (w) number (wp) and binary connection probability (p) as function of the product of strenghts or degrees in the format:
```
x^{out}_i*x^{in}_j \bar{t_ij} \std{\bar{t_ij}}
```
Where x stands for degree and strengths respectively and the averages $\bar{x}$ are taken over a single graph realization (and the standard deviations also). The cases *w_s_i.hist* and *w_s_o.hist* only present the average weight of links according to the strength of node origin or destination respectively.

**_w.hist files**
This files contain the distribution of existing occupation numbers in the format:
```
bin_id bin_min bin_max Bin_count Bin_std CCDF
```
The histogram is not normalized.

#### Additional files
If the flag *-z* is set to *>0*, then also some distance related quantities are computed.

**d_edges.hist and d_trips.hist and p_ij.hist files**

Distribution of binary edge costs costs and trip costs. Probability of binary connection as a function of cost between nodes.

**w_dij.hist**

Graph average value of existing weights as a function of cost.

**Example**:

The following command
```
time ./MultiEdgeAnalyzer -N 1000 -d 1 -f ../tests/sample.tr -z 0 -a ../tests/cost_matrix.dists  -h 1
```
takes on a intel® Core™ i5-2500 CPU @ 3.30GHz × 4 with 12 GM RAM the following time
```
real	0m0.258s
user	0m0.165s
sys	0m0.026s
```
on a and generates the following files:

```
N94avs37746.57447_d_edges.hist		- Binary edge cost histogram
N94avs37746.57447_d_trips.hist		- Event cost histogram
N94avs37746.57447_pij.hist			- Binary connection probability
N94avs37746.57447_w_dij.hist		- Average weight as function of cost
N94avs37746.57447_w_k_oi.hist		- Average Weight as a function of product of degrees
N94avs37746.57447_w_s_i.hist		- Average Weight as a function of incoming strength
N94avs37746.57447_w_s_o.hist		- Average Weight as a function of outgoing strength
N94avs37746.57447_p_s_oi.hist		- Average binary connection probability as a function of product of strengths
N94avs37746.57447_wp_s_oi.hist		- Average existing Weight as a function of product of strengths
N94avs37746.57447_w_s_oi.hist		- Average Weight as a function of product of strengths
N94avs37746.57447node_list.list		- List of node attributes
N94avs37746.57447_w.hist			- Weight distribution P(w)
```

The undirected version of the algorithm produces equivalent files for the *in* and *out* values.


## Acknowledgements

I would like to thank Pol Colomer and Sergio Oller for their very useful comments and suggestions.



## License

(C) Copyright 2014 Oleguer Sagarra.
All rights reserved. 
Code under License GPLv3.

This module is a part of the ODME package, and thus it is distributed under its same license (see main README).


## Roadmap

The software is for the moment ready and works smoothely under apropiate conditions.
If you wish to contribute please contact osagarra@ub.edu.



## Known issues 

When the Log option is activated, the plots referring to distances are not correctly binned.

## Additional notes

Notes:

Check the DOCS/requirements to see the dependencies

This soft has been tested on Linux Ubuntu 12.10 and MacOS X 6.x

Recommended compiler for mac is Clang while icc for Linux.
