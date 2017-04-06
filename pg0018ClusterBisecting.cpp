/* ----------------------------------------------
    AUTHOR 	: POORNIMA BYRE GOWDA
	CS647  	: ASSIGNMENT 2
	Date   	: 2-28-17
	Filename: pg0018ClusterBisecting	
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

double data_attribute[1500][25]; 
double *mean = new double [20]; 				// dynamic allocation of mean calculation array
double *st_deviation = new double [20]; 		// dynamic allocation of standard deviation.
double SSE[100]; // consider SSE for the 2 clusters
double Final_centers[20][25];



/* Function 1 Reading_files
   input: no input.
   return : no return. 
   functionality: Reading from the files recursively 
*/

void Reading_files(int leaf_Node_list[], int N)
{
	ofstream outfile1;
	ofstream outfile2;
	
	char output[150];
	char append3[3];
	if(Normalize)
	strcpy(output,"pg0018ClusteringNormalizedBisecting");
    else
    strcpy(output,"pg0018ClusteringBisecting");

	sprintf(append3,"%d",N);
	strcat(output,append3); 
	strcat(output,iFile);
	
	outfile1.open(output, ofstream::out);
	outfile1 << "@relation Clustering_" << iFile << endl;
    for (int i = 1; i<= attribute_count;i++)
	outfile1 << "@attribute  " << attribute_name[i] << "  real" << endl;
	outfile1 << "@attribute cluster real" << endl;
	outfile1 << "@data" << endl;
	
	outfile2.open("Normalized.txt", ofstream::out);
	
	ifstream infile;
	char input[25];
	char append[2];
	char line[501];
	
	for(int i = 1; i<=N; i++){
		strcpy(input,"output");
		sprintf(append,"%d",leaf_Node_list[i]);
		strcat(input,append);
		strcat(input,".txt");
		
		infile.open(input, ofstream::in);
		while (infile.good())
		{   
			infile.getline(line, 500); 
			if(strlen(line) > 2){
			outfile1 << line << "\t" << i<< endl;
		    outfile2 << line << "\t" << i<< endl;
			}
		}
		infile.close();
		input[0] = 0;
    }
	
	outfile1.close();
	outfile2.close();
	
}

/* Function		: "cleanData" 
   input		: no input.
   return 		: no return. 
   functionality: cleans the data_attribute. 
*/
void cleanData()
{
	for(int i= 1;i<rows;i++)
	for(int j= 1;j<attribute_count+1;j++)
	data_attribute[i][j]= 0;
}


/* 
Function 		: "Writing_UnNormalized_file"
input    		: N indicates the number of clusters produced.
return   		: no return. 
functionality	: Reading from the Normalized data file and write to Unnormalized file
*/

void Writing_UnNormalized_file( int N)
{
	rows = 1;
    columns	= 1;
	
	ofstream outfile1;
	
	char output[150];
	char append[3];
	char line[501];
	double array[2000][20];
	
	strcpy(output,"pg0018ClusteringUnormalizedBisecting");
	sprintf(append,"%d",N);
	strcat(output,append); 
	strcat(output,iFile);
	
	outfile1.open(output, ofstream::out);
	
    outfile1 << "@relation Clustering_" << iFile << endl;
    for (int i = 1; i<= attribute_count;i++)
	outfile1 << "@attribute  " << attribute_name[i] << "  real" << endl;
	outfile1 << "@attribute cluster real" << endl;
	outfile1 << "@data" << endl;
	
	ifstream infile;
    infile.open("Normalized.txt", ofstream::in);
		
		while (infile.good())
		{   
	       //cout << endl;
	        infile.getline(line, 500); 
				if(strlen(line)>2){
					char *data;
					data = strtok (line, " \t ");
					columns = 1;
					while (data != NULL)
				   {
					array[rows][columns] = atof(data);
					data = strtok (NULL, " \t ");
					columns ++ ;
				   }    
				   rows++;  
			    } 
		}
		
		infile.close();
		mean[columns-1]=0; st_deviation[columns-1]=1;
		for(int i = 1; i<rows; i++){
		//cout << endl;
		outfile1 << endl;
		for (int j=1; j< columns; j++){
		if(strcmp(iFile,"kmtest.arff")==0)
		outfile1 <<  setw(10) <<  round((array[i][j] * st_deviation[j]) + mean[j]) << setprecision(6)  <<  "\t";
		else
        outfile1 <<  setw(10) <<  (array[i][j] * st_deviation[j]) + mean[j] << setprecision(6)  <<  "\t";	
	
	    //cout <<  round((array[i][j] * st_deviation[j]) + mean[j]) << "\t";	
		}
	    }
    
	    outfile1.close();
		
		cleanData();

	
}

/* 
Function 		: "Print_cluster_centers"
input    		: leaf_Node_list indicate all the leaf nodes 
                  N indicates the number of clusters produced.
return   		: no return. 
functionality	: print tye cluster centers of the leaf nodes
*/


void Print_cluster_centers(int leaf_Node_list[],int N)
{
	
	ofstream outfile2;
	char output_line2[60];
	
	if(Normalize)
	strcpy(output_line2,"pg0018ClusterCenterNormalizedBisecting");
    else
	strcpy(output_line2,"pg0018ClusterCenterBisecting");

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
	
	
	for(int i = 1; i<=N; i++){
		outfile2 << endl;
		for (int j=1; j<=attribute_count; j++)
		{
			
			int k = leaf_Node_list[i];
			outfile2 <<  setw(15) <<  Final_centers[k][j] << setprecision(6)  <<  "\t";
		}
		
	}
	outfile2 << endl;
	outfile2.close();
	
	if(Normalize)
	{
		output_line2[0]=0;
		strcpy(output_line2,"pg0018ClusterCenterUnnormalizedBisecting");
		
		append[0] = 0;
		sprintf(append,"%d",N); // put the int into a string
		strcat(output_line2,append);
		strcat(output_line2,iFile);
			
		outfile2.open(output_line2, ofstream::out);
		outfile2 << "@relation Clustering_" << iFile << endl;
		for (int i = 1; i<= attribute_count;i++)
		outfile2 << "@attribute  " << attribute_name[i] << "  real" << endl;
		outfile2 << "@attribute cluster real" << endl;
		outfile2 << "@data" << endl;
		
		
		for(int i = 1; i<=N; i++){
			outfile2 << endl;
			for (int j=1; j<=attribute_count; j++)
			{
				int k = leaf_Node_list[i];
				outfile2 <<  setw(15) <<  (Final_centers[k][j] * st_deviation[j] ) + mean[j] << setprecision(6)  <<  "\t";
			}
			
		}
		outfile2 << endl;
		outfile2.close();
		
	}
	
}



/* 
Function 		: "printing_two_cluster"
input    		: m -> mth output clustered data
                  n -> nth output clustered data
return   		: no return. 
functionality	: printing the clustered data into the respective clustered file
*/


void printing_two_cluster(int m, int n){
	
    ofstream outfile1; ofstream outfile2;
	char output_File1[25];
	char output_File2[25];
	char append1[2];char append2[2];
	
	strcpy(output_File1,"output"); strcpy(output_File2,"output");
	sprintf(append1,"%d",m); // put the int into a string
	strcat(output_File1,append1 ); strcat(output_File1,".txt");
	
	sprintf(append2,"%d",n); // put the int into a string
	strcat(output_File2,append2 ); strcat(output_File2,".txt");
	
	outfile1.open(output_File1, ofstream::out);
	outfile2.open(output_File2, ofstream::out);
	
	for (int i = 1; i< rows; i++){
		
    if(data_attribute[i][attribute_count+1] == 1 ){
	for (int j = 1; j <= attribute_count ; j++)
	outfile1 <<  setw(10) <<   data_attribute[i][j]  << setprecision(6)  <<  "\t";
	outfile1 <<  "\n";
    }
	else if (data_attribute[i][attribute_count+1] == 2){
    for (int j = 1; j <= attribute_count ; j++)
	outfile2 <<  setw(10) <<   data_attribute[i][j]  << setprecision(6)  <<  "\t";
	outfile2 <<  "\n";
	
	}}
	
}


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


/* Function : Deleting_files
   Input: int count holds number of the file generated
   Output: No returns      
*/

void Deleting_files(int count){
	
	char File_name[25];
	char append[2];
	
	for(int i=0; i<= count; i++){
	strcpy(File_name,"output");
	sprintf(append,"%d",i);
	strcat(File_name,append);
	strcat(File_name,".txt");
	
	if( remove(File_name) != 0 )
    perror( "Error deleting file" );
	File_name[0] = 0;
    
	}
	
	if( remove("Normalized.txt") != 0 )
    perror( "Error deleting file" );
	
}


/* Function 1 "K-means Algorithm" 
   input: int N -> cluster Number
   return : no return. 
   functionality: Implementation of K-means Algorithm. 
*/	
  void  K_means(int N, int m , int n )
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
		bool flag = true;
		
		/*Random Numbers */
		Random_number(Number,N);

		
		/* Assign inintial centers */
		
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
	    cout << "\n" << "centers " << "\n";
		for (int j=1; j <= attribute_count; j++)
	    cout << centers[i][j] << "\t" ; }}
	
	
	    /* While loop interates till the initial centers points are equal to iterative mean centers  */
		
		while (flag){
			
		for (int i=1; i < rows; i++) {
			 // cout << "\n";
			min_distance = 10000000000000000; cluster_index = 0;
			for (int j = 1; j <= N; j++){
				distance[j] = 0;
				for (int k = 1; k <= attribute_count; k++ )  
				{
					//cout <<  "data " << data_attribute[i][k] << "\t" << centers[j][k] << " j " << j<< endl;
				   if(k != exclude ){
				   double absolute_value = ( data_attribute[i][k] - centers[j][k]);
				   double squared_value = pow(absolute_value,2.0 );
				   distance[j] = distance[j] + squared_value ;
				   //cout << absolute_value <<  "\t" << squared_value << endl;
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
			
			 // cout << "\n" << "selected cluster " << cluster_index <<"\t";
			 data_attribute[i][attribute_count + 1] = (double) cluster_index ;
   		
		}	
		
		/* Previos centers value*/
		
        for (int i=1; i<=N; i++){
		 for(int j = 1; j <=attribute_count; j++){	
		  before_check =+ centers[i][j];
		}}
		
		before_check = abs(before_check);
		
		
		/* code to find the mean centers */
		
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
		
         for (int i=1; i<=N; i++){
		 for(int j = 1; j <=attribute_count; j++){	
		  after_check =+ centers[i][j];
		}}
		
		after_check = abs(after_check);
		
		if(before_check == after_check)
			flag = false;
		count ++ ;
		
		for (int i = 1; i<=N; i++)
		cluster_number[i] = 0;
	
	    
	}
	
	if(Debug){
	cout << "rows" << rows << "coulmns" << columns << endl;
	for(int i=1; i< rows;i++){
	cout << endl;
	for(int j=1; j<= attribute_count+1;j++)
	cout << data_attribute[i][j] << "\t"; } }


    /* code to calculate SSE */
	
	for (int i = 1; i<= rows; i++ ){
		if (data_attribute[i][attribute_count+1] == 1){
		for (int j = 1; j<= attribute_count; j++){
			// cout << centers[1][j] << "\t" << data_attribute[i][j]<< "\t" ;
			if(j!= exclude)
            SSE[m]	+= pow((centers[1][j]- data_attribute[i][j]),2.0);
		}
		}
		else if (data_attribute[i][attribute_count+1] == 2){
		for (int j = 1; j<= attribute_count; j++){
			if(j!= exclude)
            SSE[n]	+= pow((centers[2][j]- data_attribute[i][j]),2.0);
		  
		}
		}
	}
	//cout << endl;
	cout << "SSE[" << m << "]" << SSE[m] << "  SSE[" << n << "]" << SSE[n]<< endl;
     
	 
	 /* code to assign the final mean cluster centers */ 
	 
	for (int j = 1; j<=attribute_count; j++ )
	{
		Final_centers[m][j] = centers[1][j];
		Final_centers[n][j] = centers[2][j];
		// cout << "Final_centers  " << m << Final_centers[m][j] << "\t"  << "Final_centers  " << n << Final_centers[m][j]<< endl;
	}	
	 
	if (Normalize)
	strcpy(output_line1,"pg0018ClusterCenterNomalizedBasic");
    else
	strcpy(output_line1,"pg0018ClusterCenterBasic");	
	
	char append[2];
	sprintf(append,"%d",N); // put the int into a string
	strcat(output_line1,append);
	strcat(output_line1,iFile);
		
	
   }
	
	
	
	
	/* Function 1 "Read the file every time"  
   input: File_number.
   return : no return. 
   functionality: calculate mean and standard deviation. 
*/	
	
void ReadFile(int FileNumber)
{
	rows = 1; columns = 1;
	char line1[201];
	char Filename[30];
	strcpy(Filename,"output");
	char append[2];
	sprintf(append,"%d",FileNumber); // put the int into a string
	strcat(Filename,append );
	strcat(Filename,".txt");
	ifstream infile;
	infile.open(Filename, ofstream::in);
	
	while (infile.good())
    {
        infile.getline(line1, 200); 
		if(strlen(line1)>2){
		char * data;
        data = strtok (line1," ,");
		columns = 1;
        while (data != NULL)
        {
         data_attribute[rows][columns] = atof(data);
         data = strtok (NULL, " ,");
		 columns ++ ;
        }   
	    rows++;  }  
    }
	
	 if( Normalize ){
     if(FileNumber == 0){
	 for (int j=1; j <= attribute_count; j++)
	 mean_standard_deviation_calculation(j);
	 mean[exclude] = 0; st_deviation[exclude] =1;
		 
	 for (int i = 1; i<rows ; i++)
	 for (int j=1; j <= attribute_count; j++)
	 data_attribute[i][j] =  ( data_attribute[i][j] - mean[j] ) / st_deviation[j] ;
	 }
	}
	
     /* for(int i=1; i< rows;i++){
	cout << endl;
	for(int j=1; j<= attribute_count;j++)
	cout << data_attribute[i][j] << "\t";
    } */
	//cout << "rows" << rows << endl;
	//cout << "columns" << columns << endl; 
	
}

	/* main function */ 	
	
    int main (int argc, char *argv[])
    {
		
		FILE *inputfile; 			// input file pointer 
		char *line = NULL; 			// char pointer.
		size_t len = 0;  			// length of the string obtained each time.
		ssize_t read_length; 		// length of the line read. -1 if error. 
        char exclude_attribute[25]; // attribute name that has to be excluded.
		int N = 0;                  // number of K clusters
        ofstream outfile1;      // output file 1
		
		
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
		
		outfile1.open("output0.txt", ofstream::out);
		   
	    /* Reading the file and storing the input attributes names and data*/
		 
        while ((read_length = getline(&line, &len, inputfile)) != -1)  // returns -1 when there is error.
                 {
                    if(read_length > 2 && line[0] != '#')  // Skip any blank lines and comment line
                    {
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
						
				 else if (line[0] != '@')
				{

					outfile1 << line << endl;
							
                }							
        }
					
     }
		
			 
		/* close the file */
		fclose(inputfile); 
		outfile1.close();
		free(line);
		
		
		/* Calling the Bisecting Kmeans*/ 
		
		int all_Node_list[20];
		int parent_Node_list[20]; // assuming N < 20 clusters given
		int leaf_Node_list[20]; // choose only leaf node clusters given
		int count = 0;
		int select = 0;
		int max_value = 0;
		SSE[0] = 0;
		all_Node_list[0] =0;
		int count1=0;
		
		
		
		
		/* code that calls the k=2 K_means and recursively spilt the data by finding the highest SSE */
		
		for(int i=0; i<= (N-2); i++) { // runs Clustering for only floor of (N/2) times.
		
		    max_value = 0;
			for (int j = 0; j <=count;j++)
			{
				//cout << "SSE " << SSE[j] << "\t";
				if(SSE[j] > max_value){
					max_value = SSE[j];
					select = j;
				}		
			}
		    SSE[select] = 0;
		    parent_Node_list[i] = select;
			cout << "parent Node list[" << i << "] " <<  parent_Node_list[i] << endl;	
			
			count1++;
			all_Node_list[count1]= count +1;
			cout << "All Node list["<< count1 << "] " <<  all_Node_list[count1] << endl;
			
			count1++;
			all_Node_list[count1]= count +2;	
			cout << "All Node list["<< count1 << "] " << all_Node_list[count1] << endl;
			
		    ReadFile(select);
			K_means(2,count+1,count+2);
			printing_two_cluster(count+1,count+2);
			cleanData();
			count= count+2;
	
		}
		
		bool check;
		int list_count = 1;
		//cout << "count" << count << endl; 
		
		
		/* code that finds the leaf node*/
		/* parent_node-> data that is spilt
		   leaf_node -> data that is not split */
		
		for (int i = 1; i <= count1; i++)
		{
			// cout << all_Node_list[i] << endl;
			check = true;
			for (int j =0; j < N-1; j++)
			{
				 // cout << all_Node_list[i]<< "\t" <<  parent_Node_list[j] << endl;
				if(all_Node_list[i]== parent_Node_list[j])
					check = false;
			}
			
			if(check)
			{
				leaf_Node_list[list_count] = all_Node_list[i];
				list_count++;
			}
			
		}
		
		//for (int i=1; i <list_count; i++ )
	    //cout <<leaf_Node_list[i] << endl;
		
		 Reading_files(leaf_Node_list, N);
		 Print_cluster_centers(leaf_Node_list, N); // prints all the cluster centers
		 
		 if(Normalize)
		 Writing_UnNormalized_file(N);
	 
	 
	     Deleting_files(count1);
		 
		/* Free all the dynamically allocated data */
		 
			 
	    delete [] mean;  // When done, free memory pointed to by mean.
        mean = NULL;     // Clear a to prevent using invalid memory reference.
		
		delete [] st_deviation;  // When done, free memory pointed to by st_deviation.
        st_deviation = NULL;     // Clear a to prevent using invalid memory reference.
		 
	
	return 1;
 }
 
	   
	   
