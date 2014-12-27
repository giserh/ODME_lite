/********************************************************************************
 * 
 *			GENERATE Maximum entropy ensembles for networks
 *
 *	This program generates networks (and statistics) with pre-defined constraints using a macro-canonical approach (poisson or geometric).
 *  This version considers the case in which one fixes the degree sequence.
 *  More details in
 * [1] O. Sagarra, C. J. Pérez Vicente, and A. Díaz-Guilera, "Statistical mechanics of multiedge networks" Phys. Rev. E 88, 062806 (2013)
 *
 * 
 * Author: Oleguer Sagarra, 2013.
 * 
 * 
 *	Output:
 *		
 *		*.tr: A file stored in list format of flows between given nodes
 * 			node i node j t_ij
 *		*.hist: Diverse statistics on the network (indicative names on the files)
 *		*.list: Node attributes average with average deviation (see files).
 * 
 * ******************************************************************************/




#include "main.h"


int main(int argc, char *argv[]){

	printf(\
	"################################\n"
	"########## Maximum entropy multi-edge networks: Fixed binary sequence ###########\n"
	"####################################\n");
 /***********************************************************************
	 we read the parameters and initialize the random generator	 N_nodes, Reps, Seed, s_sequence file, d_file),
 ************************************************************************/
	int N_nodes		= -1;
	int T	 		= -1;
	char* file_s;
//	char* file_d;
//    int meth		= 2;
	int seed		= 1;      			
    int opt_dir		= -1;			
    double bin_exp 	= -1;				
	int verbose		= 0;				
	int opt_clust 	= 0;			
	int self_opt	= 1;
	int header		= 1;
//	int dmax 		= 20000;
    int print_tr	=0;
//	double gamma	=0.;
    int reps		=100;
//	int opt_log		= 0; // logarithmic option
    int opt_indist  =0;
    int max_reps 	=100;
	int ch;
//	        while ((ch = getopt(argc, argv, "N:s:d:f:a:x:v:c:l:h:m:T:e:g:r:p:L:")) != -1) {
	        while ((ch = getopt(argc, argv, "N:s:d:f:x:v:c:l:h:T:g:r:p:D:e:")) != -1) {
	             switch (ch) {
	             case 's': /* seed */
	                     seed=atoi(optarg);
	                     break;
			     case 'N': /* N_nodes */
			             N_nodes=atoi(optarg);
						 break;
			     case 'p': /* print opt */
			             print_tr=atoi(optarg);
			             break;
				 case 'T': /* TOtal trips */
				 		 T=atoi(optarg);
			             break;
        		 case 'D': /* Distinguishable option */
        				 opt_indist=atoi(optarg);
        			     break;
	   			case 'e': /* method */
	   				 	 max_reps=atoi(optarg);
	   			         break;
				case 'r': /* reps */
			             reps=atoi(optarg);
					     break;
//				case 'g': /* gamma */
//			             gamma=atof(optarg);
//					     break;		 
	            case 'd': /* dir_opt */
	                     opt_dir=atoi(optarg);
	                     break;
	             case 'f': /* file adj */
	                     file_s=optarg;
	                     break;
//			     case 'a': /* file Dist */
//			             file_d=optarg;
//			             break;
	             case 'x': /* exp */
	                     bin_exp=atoi(optarg);
	                     break;
			     case 'v': /* verbose */
			             verbose=atoi(optarg);
			             break;
			     case 'c': /* clust */
			             opt_clust=atoi(optarg);
			             break;
			     case 'l': /* self loop */
			             self_opt=atoi(optarg);
			             break;
				case 'h': /* self loop */
					     header=atoi(optarg);
					     break;
//				case 'm': /* max_Distance */
//						 dmax = atof(optarg);
//						 break;			
//				case 'L': /* Log opt */
//						 opt_log=atoi(optarg);
//						 break;
	             default:
				 {
	                     printf("Unknown flag %c\n", ch);
	                     exit(EXIT_FAILURE);
	             }
	}
	}
//	if(meth==2)
//	{
//		T=1;
//	}
//	if(opt_log>0)
//	{
//		dmax = log(dmax);
//	}
	if((N_nodes<0)||(opt_dir<0)|| (T<0))
	{
 		fprintf(stderr,	"\nCorrect usage is: ./simus -args \n\nWhere:\n\n"
 				" *  Compulosry items:\n"
 				" *		-N N_nodes. Number of nodes (int)\n"
 				" *		-d dir_opt. Undirected (0) or Directed (1)\n"
 			    " *		-f file_xy Path to file with with lagrange multipliers id x y\n"
// 			    " *		-a file_dist Path to distance list in format node_i node_j d_ij\n"
				" *		-T Number of trips (int) [not needed for method=2] \n"
 				" *  Optional items: \n"
                " *		-D Inditinguishability option, set >0 if want weigths to be indistinguishable (default=0)"
//				" *		-e Ensemble method (0 canonical, 1 Grand Canonical Multinomial, 2 Grand Canonical POisson) (default=2)\n"				
 				" *		-s seed.initial seed for random generator (int) (default=1)\n"
 				" *		-x Exponent for log-binning (-1 for no log binning) (Default=-1)\n"
 				" *		-v Verbose (1 for on, 0 for off) (Default 0)\n"
 				" *		-c Clustering option (1 for yes) (warning: Depending on av_s makes simulations orders of magnitude slower) (Default=0)\n"
 				" *		-l Self-loop option (>0 for accepting them) (Default =1) \n"
				" *		-h Number of header lines in input files (default=1)\n"
//				" *		-m Maximum distance for binning (default= 20000) [in meters]\n"
//				" *		-g Gamma value (lagrange multiplier, default=0.0)\n"
				" *		-r Number of reps for averaging (default=100)\n"
				" *		-p Printing option of sample adjacency list (default=0)\n"
				" *		-L Log-dist option (to compute the logarithm of the cost matrix) [default=0]\n\n"
 				"Please, read the DOCS/README file for more info!\n");
 		return 0;
 	}

	/****** Check all in params are good ******/
	if(bin_exp<=1) bin_exp=1.05;
    if((opt_dir!=1)&&(opt_dir!=0))
    {
		printf("Select directed or undirected!, aborting...\n");
		abort();
	}
//	if((meth>2) || (meth<0))
//	{
//		printf("Select apropiate method (canonical, grandcanonical...), aborting ....\n");
//		abort();
//	}
	if((print_tr != 0) && (print_tr != 1))
	{
		printf("Select if you want to print the adj list, aborting ....\n");
		abort();
	}
	if(opt_clust==1)
	{
		if(opt_dir==1)
		{
			printf("Ignoring clustering option, only defined for undirected networks ....\n");
			opt_clust=0;
		}else{
//			printf("CLustering option selected! This may cause low performance for high <s>! ....\n");
			printf("CLustering option selected! This may cause low performance for high <k>! ....\n");
		}
	}
  /*** Set rand generator (uses GLS THAU) ***/ 
	//seed = seed + time(NULL); // Change for trully random generation
	gsl_rng * randgsl = gsl_rng_alloc(gsl_rng_taus);	/// we initialize the random generator of the gsl
	gsl_rng_set(randgsl,seed);

 /*** Print some info ***/

	printf("SEED=%i\n",seed);
	printf("Average over %d reps\n", reps);
	//printf("Iterations=%i\n",Reps);
	if(opt_indist>0)
	{
		printf("Indistinguishable weights selected (ZIG stats)\n");
	}else{
		printf("Distinguishable weights selected (ZIP stats)\n");	    
	}


	double X;
	//long double Tfake;
	double ** x2;
	double * x;
//	double** dist;
	double av_k,E_av,rho;
 /***********************************************************************
 	Allocating memory + reading distro
 ************************************************************************/ 	
//	dist = read_distances(file_d,N_nodes,header,opt_log);
	if(opt_dir==1)
	{
		x2 = read_node_list_double(file_s, N_nodes,header); // degree lagrange sequence (double)
		X=sum_vec_double(x2[0],N_nodes); // T is \sum_i s_i (for all cases)
		E_av = w_graph_compute_E_binary_directed(x2, N_nodes, self_opt);  	// compute expected degree, stop if E>T:

	}else{
		x = read_node_list_double_undir(file_s, N_nodes,header); // sdegree lagrange sequence sequence (double)
		X=sum_vec_double(x,N_nodes); // T is \sum_i s_i (for all cases)
		E_av = w_graph_compute_E_binary_undirected(x, N_nodes, self_opt); // compute expected degree, stop if E>T:
	}
	if(E_av>=T)
	{
		T = (int)E_av+1;
		printf("Expected edges larger than total sampling T! Correcting T to %d value!\n",T);
	}
	rho = w_graph_compute_rho(E_av,T,opt_indist);
	printf("Total expected edges: %f, Total sampling:%d, average edge weight: %f\n",E_av,T,(double)T/E_av);
	av_k=(double)T/(double)N_nodes;
	//Tfake = (long int)T;
	if(verbose==1)
	{
		if(opt_dir==1)
		{
			printf("-  x_max/X : (out) =%lf (in) %lf\n",max_value_double(x2[0],N_nodes)/X, max_value_double(x2[1],N_nodes)/X );
		}else{
			printf("-  x_max/X = %lf\n",max_value_double(x,N_nodes)/X );
		}
		if(self_opt>0)
		{
			printf("Warning! Self-loops accepted! \n");
			fflush(stdout);
		}
	}
	char cadena[100];
/***********************************************************************
	Preparing ensemble reps!
 ************************************************************************/ 	
	int r, len_acc_nodes;
	W_GRAPH* WG;
	double ** node_cont; //node info container for averaging
	double ** node_cont2; // node info std for averaging
	int ** node_nonzero; // node count non-zero strength/degree
	double * Tcont; // Total T container (av and std)
	int w_max;
	//int s_min;
	if(opt_dir==1)
	{
		len_acc_nodes=14; // in-out variables
		w_max=(int)(50*max_value_double(x2[0],N_nodes))*max_value_double(x2[1],N_nodes);
	}else{
		w_max=(int)(50*max_value_double(x,N_nodes))*max_value_double(x,N_nodes);
		if(opt_clust==1)
		{
			len_acc_nodes=9;
		}else{
			len_acc_nodes=7;
		}
	}
	w_max=w_max*2;
	if(w_max<1)
	{
		w_max=100;
	}
	if(verbose==1)
	{
		printf("-- wmax for histogram: %d\n",w_max);
	}
	node_cont=cast_mat_double(N_nodes,len_acc_nodes);
	node_cont2=cast_mat_double(N_nodes,len_acc_nodes);
	node_nonzero=cast_mat_int(N_nodes,2); // to count the number of times a node is non-connected (in-out)
	
	Tcont=cast_vec_double(2); // mean and std deviation
	//double* entropy_seq = cast_vec_double(reps); // entropy histogram
	gsl_histogram ** acc_ensemble= w_graph_all_stats_ensemble_allocate(opt_dir, 1, w_max*w_max/T, 1, w_max*w_max/T, w_max);
		
	/***********************************************************************
		Fill analytic calculation for degree
	 ************************************************************************/ 	
	int i;
	if(opt_dir==1)
	{
		double** k=w_graph_compute_k_binary_directed(x2, N_nodes, self_opt);
		for(i=0;i<N_nodes;i++)
		{
			node_cont[i][1]=k[0][i]*reps;
			node_cont2[i][1]=k[0][i]*k[0][i]*reps;
			node_cont[i][7]=k[1][i]*reps;
			node_cont2[i][7]=k[1][i]*k[1][i]*reps;
		}
		free_mat_double(k,2);
	}else{
		double* k=w_graph_compute_k_binary_undirected(x, N_nodes, self_opt);
		for(i=0;i<N_nodes;i++)
		{
			node_cont[i][1]=k[i]*reps;
			node_cont2[i][1]=k[i]*k[i]*reps;
		}
		free(k);
	}

	
/***********************************************************************
	Start of ensemble reps!
 ************************************************************************/
	for(r=0;r<reps;r++)
	{
		if(verbose==1) printf("============### ZIP Poisson model ####===========\n"); fflush(stdout);
		if(opt_dir==1)
		{
            if(opt_indist>0)
            {
                WG = fixedk_geometric_directed_graph(x2, rho, N_nodes, randgsl, verbose, self_opt, max_reps);
            }else{
                WG = fixedk_poisson_directed_graph(x2, rho, N_nodes, randgsl, verbose, self_opt, max_reps);                
            }
		}else{
            if(opt_indist>0)
            {
                WG = fixedk_geometric_undirected_graph(x,  rho, N_nodes, randgsl, verbose, self_opt, max_reps);                
            }else{
                WG = fixedk_poisson_undirected_graph(x,  rho, N_nodes, randgsl, verbose, self_opt, max_reps);                
            }
		}
		if(r==0) // if first rep, store all stats
		{
			/// extended stats with distance ////
			if (verbose>0) printf("... Graph stats ... \n");
			w_graph_all_stats(WG, N_nodes, r, bin_exp, av_k, opt_dir, self_opt, -1);
			if (verbose>0) printf("... Node stats ... \n");
			w_graph_node_stats_list(WG,N_nodes,0, av_k, opt_dir, opt_clust, self_opt);
            if(print_tr==1)
            {
                if(verbose>0)
                {
                    printf("Printing adj matrix\n");
                    fflush(stdout);
                }
                sprintf(cadena,"N%d_cust.tr",N_nodes);
                w_graph_print_adj_list(WG, N_nodes, cadena);
            }
		}
		if (verbose>0) printf("... Ensemble stats ... \n");
		/// extended stats with distance ////
		w_graph_node_stats_ensemble(WG,N_nodes,node_cont,node_cont2,node_nonzero, Tcont,opt_dir,opt_clust);
		w_graph_all_stats_ensemble_update(acc_ensemble, WG, N_nodes, opt_dir);
		//entropy_seq[r]= w_graph_entropy(node,N_nodes);
		w_graph_free(WG, N_nodes);
	}
	//printf("i am here!\n");fflush(stdout);
	//if(meth<2) free(ps);
/***********************************************************************
	 Print output
************************************************************************/ 	
	//int len=6;
	/*
	if(opt_dir==1)
	{
		len=10;
	}else{
		len=6;
	}
	*/
	/// extended stats with distance ////
	//fflush(stdout);
	if (verbose>0) printf("... Printing ensemble net stats ... \n");
	w_graph_all_stats_ensemble_print(acc_ensemble, 2, reps, N_nodes, av_k, opt_dir);
	if (verbose>0) printf("... Printing ensemble node stats ... \n");	
	w_graph_node_stats_ensemble_print(reps, N_nodes, Tcont, node_cont, node_cont2, node_nonzero, av_k, bin_exp,len_acc_nodes, opt_dir);
/*
	if(reps>10)
	{
		sprintf(cadena,"N%davs%8.5fentropies.hist",N_nodes,av_k);
		w_graph_print_entropy(entropy_seq,reps,cadena);
	}else{
		printf("If you want entropy histogram, do at least 10 reps!\n");
	}
*/
	gsl_rng_free (randgsl);
//	free(entropy_seq);
	return 0;
}
