/* ----------------------------------------------
    AUTHOR : POORNIMA BYRE GOWDA
	CS647  : ASSIGNMENT 2
	Date   : 2-28-17
	Filename: pg0018Cluster
-----------------------------------------------*/

/* HEADER FILE */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>  
#include <fstream>
#include <iomanip>
#include <ctime> 

/* Pre-processor vabriables */
#define Debug 0      							// Hiding the testing code


using namespace std;  							// Includes the "std" namespace

/* GLOBAL DECLARATIONS */ 

int rows = 1 ; 									// number of rows
int columns = 1; 								// number of columns
int exclude = 0;    							// holds the attribute number of the class attribute.
int attribute_count = 0; 						// That counts the number of attributes
char *attribute_name[20]; 						// character pointer arry that holds the attributes names
bool  Normalize = false;    					// Enables the Normalizing Data 
int sizeX = 2000; 								// number of rows for dynamica allocation of array
int sizeY = 2000; 								// number of columns for dynamica allocation of array
char iFile[20];                                 // input file name.

double ** data_attribute = new double*[sizeX]; 	// dynamic allocation of the data
double *mean = new double [20]; 				// dynamic allocation of mean calculation array
double *st_deviation = new double [20]; 		// dynamic allocation of standard deviation.

/* Function Decaration and Definition */

/* Function 1 "mean_standard_deviation_calculation" 
   input: int k , k implies kth attribute mean and standard calculation.
   return : no return. 
   functionality: calculate mean and standard deviation. 
*/
void mean_standard_deviation_calculation ( int k) {	

	mean[k] = 0; st_deviation[k] = 0;
	for (int i=1; i < rows; i++)
	mean[k]= mean[k] + data_attribute[i][k];
	mean[k]= mean[k] / (rows - 1);	
	for (int i=1; i < rows; i++)
	st_deviation[k] = st_deviation[k] + pow (( data_attribute[i][k] - mean[k] ), 2.0);
		
	st_deviation[k] = sqrt(st_deviation[k]/(rows-1));
	if (Debug){
	printf("%lf \t", mean[k] );
	printf("%lf \n", st_deviation[k] ); }
	
	}

	
/* Function : Random Number 
   Input: Number[], Array Address
          N, Number of clusters that has to be produced
   Output: No returns      
*/
	void Random_number (int Number[], int N)
{
    int random;
    srand((unsigned)time(0)); 

    for (int i = 1; i <= N; i++){
		 random = (rand()% (rows-1))+1; 
		 for(int j = 1; j<i; j++)
		{
		 if(random == Number[j]){
		  random = (rand()% (N-1))+1;
		}
		}
		
		 Number[i] = random;
        // cout << Number[i] << endl;
    }
}
	
 
	
/* Function 1 "K-means Algorithm" 
   input: int N -> cluster Number
   return : no return. 
   functionality: Implementation of K-means Algorithm. 
*/	
  void  K_means(int N )
	{
		double before_check=0;
		double after_check=0;
		double centers [N][attribute_count];
		double relocate_centers[N][attribute_count+1];
		double distance[N];
		double min_distance = 10000000000000000;
		int cluster_index = 0; 
		int count = 0;
        ofstream outfile1;
		char output_line1[50];
		int Number[N];
		bool flag= true;
		
		/* create the k-random numbers */
		Random_number(Number,N);
         
		
		/* assign Initial centers */
		
		for (int i=1; i <= N; i++){
		int random = Number[i];
		for (int j=1; j <= attribute_count; j++){
		relocate_centers[i][j] = 0;	
        if(j!= exclude)		
		centers[i][j] = data_attribute[random][j]; // assigned the cluster centers
	    else
		centers[i][j] = 0;	
	    }
		}		 
		
		if (Debug){
		for (int i=1; i <= N; i++){
	    cout << "\n";
		for (int j=1; j <= attribute_count; j++)
	    cout << centers[i][j] << "\t" ; }}
	    
		
		/* execuet till the old centers not equal to new centers */ 
		
		
		while (flag){
			
		for (int i=1; i < rows; i++) {
			// cout << "\n";
			min_distance = 10000000000000000; cluster_index = 0;
			for (int j = 1; j <= N; j++){
			distance[j] = 0;
			for (int k = 1; k <= attribute_count; k++ )  
			{
				if(k != exclude ){
				double absolute_value = ( data_attribute[i][k] - centers[j][k]);
				double squared_value = pow(absolute_value,2.0 );
				distance[j] = distance[j] + squared_value ;
				}
			}
				distance[j]= sqrt(distance[j]);
				
				if(Debug)
				cout << distance[j] << "\t";
				
				bool Lesser_than =  ( (min_distance > distance[j])  )  ? 1 : 0;
				if ( Lesser_than )
				{
					min_distance = distance[j];
					cluster_index = j;
				}   
			}
			 data_attribute[i][attribute_count + 1] = (double) cluster_index ;
   		
		}	
		
		// cout << "select " << cluster_index <<  "\t";
		
		
		/* copy old centers values */
		
		for (int i=1; i<=N; i++){
		for(int j = 1; j <=attribute_count; j++){	
		  before_check =+ centers[i][j];
		}}
		
		before_check = abs(before_check);
		
		/* calulate the mean centers*/
		
        int cluster_number[N];
		for (int i = 1; i<=N; i++)
		cluster_number[i] = 0;
		
		for ( int i = 1; i < rows ; i++ )
		{
			int j = data_attribute[i][attribute_count+1];
			cluster_number[j] += 1;
			for (int k= 1; k<= attribute_count; k++){
		    if(k!= exclude)
			relocate_centers[j][k] += data_attribute[i][k];
		    }
		}
		
		for (int i=1; i<=N; i++){
			// cout <<"\n" << "count " <<  cluster_number[i] << "\n";
		 for(int j = 1; j <=attribute_count; j++){	
			if ( cluster_number[i] != 0)
			relocate_centers[i][j] = relocate_centers[i][j] / cluster_number[i]  ;
		    centers[i][j] = relocate_centers[i][j] ;
		    // cout <<  relocate_centers[i][j] << "\t";
			relocate_centers[i][j] = 0;
		}}
		
		/* copy new centers values */
		
		for (int i=1; i<=N; i++){
		 for(int j = 1; j <=attribute_count; j++){	
		  after_check =+ centers[i][j];
		}}
		
		after_check = abs(after_check);
		
		/* compare old centers rs new centers*/ 
		
		if(before_check == after_check)
			flag = false;
		
		count ++ ;
		
		for (int i = 1; i<=N; i++)
		cluster_number[i] = 0;
	
		}

		
	/* printing the cluster centers both noralised nad Un-Normalized format*/ 
	
	if (Normalize)
	strcpy(output_line1,"pg0018ClusterCenterNomalizedBasic");
    else
	strcpy(output_line1,"pg0018ClusterCenterBasic");	
	
	char append[2];
	sprintf(append,"%d",N); // put the int into a string
	strcat(output_line1,append);
	strcat(output_line1,iFile);
	
	outfile1.open(output_line1, ofstream::out);
    outfile1 << "@relation ClusterCenter_" << iFile << endl;
	for (int i = 1; i<= attribute_count;i++)
    outfile1 << "@attribute  " << attribute_name[i] << "  real" << endl;
	outfile1 << "@attribute cluster real" << endl;
	outfile1 << "@data" << endl;
	for (int i = 1; i<= N ;i++){
    outfile1 << "\n" << endl;
	for (int j = 1; j<= attribute_count;j++)
	outfile1 << centers[i][j] << "\t" ;
	}
	
	outfile1.close();
	
	if(Normalize){
	 output_line1[0] = 0;
	 append[0] = 0;
	 strcpy(output_line1,"pg0018ClusterCenterUnomalizedBasic");
	 sprintf(append,"%d",N); // put the int into a string
	 strcat(output_line1,append);
	 strcat(output_line1,iFile);
	
	 outfile1.open(output_line1, ofstream::out);
     outfile1 << "@relation ClusterCenter_" << iFile << endl;
	 for (int i = 1; i<= attribute_count;i++)
     outfile1 << "@attribute  " << attribute_name[i] << "  real" << endl;
	 outfile1 << "@attribute cluster real" << endl;
	 outfile1 << "@data" << endl;
	 for (int i = 1; i<= N ;i++){
     outfile1 << "\n" << endl;
	 for (int j = 1; j<= attribute_count;j++)
	 outfile1 << (centers[i][j] * st_deviation[j] ) + mean[j] << "\t" ;
	}	
		
	}
	outfile1.close();

	}
	
	
	/* main function */ 	
	
    int main (int argc, char *argv[])
    {
		FILE *inputfile; 			// input file pointer 
		char *line = NULL; 			// char pointer.
		size_t len = 0;  			// length of the string obtained each time.
		ssize_t read_length; 		// length of the line read. -1 if error. 
        char exclude_attribute[20]; // attribute name that has to be excluded.
		int N = 0;                  // number of K clusters
        ofstream outfile2;      // output file 2
		ofstream outfile3;      // output file 3
		
		/* copying the input arguements  */
		
	    for (int i = 1; i < argc ; i++){
			
	    if (strcmp(argv[i],"-i") == 0 ){
		strcpy(iFile,argv[i+1]);
	    } 
		
		else if (strcmp(argv[i],"-K") == 0 ){
		  N = atoi(argv[i+1]);
	    }
		
	    else if (strcmp(argv[i],"-c") == 0 ){
		strcpy(exclude_attribute,argv[i+1]);
	    }
		 
		else if (strcmp(argv[i],"-normalize") == 0 ){

		   Normalize = true;
	    }
		
	    }
		  
		/* Opening the file */
		inputfile = fopen(iFile, "rb");
        if (inputfile == NULL)
        {
           exit(EXIT_FAILURE); // If the input file doesnot exist.
        }
		   
	    /* Reading the file and storing the input attributes names and data*/
		 
        while ((read_length = getline(&line, &len, inputfile)) != -1)  // returns -1 when there is error.
         {
            if(read_length > 2 && line[0] != '#')  // Skip any blank lines and comment line
            {
			    /* Reading the attributes*/
				if( line[0] == '@' )
				{
					char *tok;
				    tok = strtok (line," ,.-");

					if( strcmp(tok,"@attribute") == 0 )
					{
						attribute_count = attribute_count + 1;
						tok = strtok (NULL," ,.-");
						if( strcmp(tok,exclude_attribute) == 0)
						{
							exclude = attribute_count;
									
						}
						attribute_name[attribute_count] = (char * ) malloc (15 * sizeof(char));
						strcpy(attribute_name[attribute_count],tok);								
								
					}
				}
				/* Redang the data*/
				else if (line[0] != '@')
				{

					data_attribute[rows] = new double [sizeY];; // dynamically create the 2 dimentional array.
				    char * data;
                    data = strtok (line," ,-");
					columns = 1;
                    while (data != NULL)
                    {
                      data_attribute[rows][columns] = atof(data);
                      data = strtok (NULL, " ,-");
					  columns ++ ;
                    }   
                      rows++;
							
                }							
            }
		else 
		{
						
		}
    }
			 
		/* close the file */
		fclose(inputfile); 
		
		
		/* Debugging the code */ 	
		
		if(Debug)
		printf("%d\n", attribute_count);
		
		if(Debug){
		for (int i = 1; i< rows;i++){
		cout << "\n" << endl;
		for (int j= 1; j <= attribute_count; j++)
		printf("%lf \t", data_attribute[i][j] );
		}}
		
	   /* If Normalisation is required for the Data */
	   
	    // cout << Normalize;
	   /* Calling the function mean_standard_deviation_calculation */
        if(Normalize){
		for (int j=1; j <= attribute_count; j++)
		mean_standard_deviation_calculation(j);
		mean[exclude] = 0; st_deviation[exclude] =1;
		 
		for (int i = 1; i<rows ; i++)
		for (int j=1; j <= attribute_count; j++)
		data_attribute[i][j] =  ( data_attribute[i][j] - mean[j] ) / st_deviation[j] ;
	    }
	
		 
	    /* K-means */
	    K_means(N);
		
		
		
		/* Free all the dynamically allocated data */
		char output_line2[50];
		char output_line3[50];
		
		
		if(Normalize)
		strcpy(output_line2,"pg0018ClusteringNormalizedBasic");	
		else 
		strcpy(output_line2,"pg0018ClusteringBasic");
	
	
		char append[2];
	    sprintf(append,"%d",N); // put the int into a string
		strcat(output_line2,append);
		strcat(output_line2,iFile);
		
        outfile2.open(output_line2, ofstream::out);
		outfile2 << "@relation Clustering_" << iFile << endl;
		for (int i = 1; i<= attribute_count;i++)
		outfile2 << "@attribute  " << attribute_name[i] << "  real" << endl;
		outfile2 << "@attribute cluster real" << endl;
		outfile2 << "@data" << endl;
		
		for(int i=1; i < rows; i++)
		{
			
		outfile2 << "\n";
		for (int j = 1; j <= attribute_count+1 ; j++)
		outfile2 <<  setw(10) <<   data_attribute[i][j]  << setprecision(6)  <<  "\t";
	    outfile2 <<  "\n";
		
		} 
		
		/* printinf the normalised data into output file */ 
		if (Normalize){
		strcpy(output_line3,"pg0018ClusteringUnnormalizedBasic");	
		char append[2];
	    sprintf(append,"%d",N); // put the int into a string
		strcat(output_line3,append);
		strcat(output_line3,iFile);
		
        outfile3.open(output_line3, ofstream::out);
		outfile3 << "@relation Clustering_" << iFile << endl;
		for (int i = 1; i<= attribute_count;i++)
		outfile3 << "@attribute  " << attribute_name[i] << "  real" << endl;
		outfile3 << "@attribute cluster real" << endl;
		outfile3 << "@data" << endl;
		double local ;
		for(int i=1; i < rows; i++)
		{
			
		outfile3 << "\n";
		for (int j = 1; j <= attribute_count + 1 ; j++){
			if (j == attribute_count + 1)
			local =  data_attribute[i][j];
		    else
			local = ( data_attribute[i][j] * st_deviation[j] ) + mean[j] ;
		outfile3 <<  setw(10) << local   << setprecision(6)  <<  "\t"; }
	    outfile3 <<  "\n";
		
		} 
		
		}
		 
	    outfile2.close();
		
		for(int i = 1; i < sizeY; ++i) 
		{
	     delete [] data_attribute[i];
		}
		delete [] data_attribute;
			 
	    delete [] mean;  // When done, free memory pointed to by mean.
        mean = NULL;     // Clear a to prevent using invalid memory reference.
		
		delete [] st_deviation;  // When done, free memory pointed to by st_deviation.
        st_deviation = NULL;     // Clear a to prevent using invalid memory reference.
		  
		free(line);
		return 0;
			
 }