#include <iostream>
#include <string>
#include <fstream>
# include <cmath>
using namespace std;

// after this point normal matrix operation will take over
int crossover = 128;
// int dimension = 2048;
int dimension = 4;
int current_dim = dimension;

// NOTES
// Change to be able to handle 32 bit numbers - perhaps use long int

class Matrix
{
	public:
		int dims;
		int array_len = 0;
		int *data;

		Matrix(int array_size){
			array_len = array_size;
			dims = (int) sqrt(array_size);
			data = new int[array_len];
		}
		

		void init_rand(){
			for (int i=0; i < array_len; i++){
				this->data[i] = rand() %2;
			}
		}

		void init_randadjacency(int probability){
			for (int i=0; i < array_len; i++){
				int column = i % dims;
				int row = ((i - (i%dims))/dims); 
				printf("( %d,  %d ):", column, row);
				if ((i % dims) > (i - i% dims)/dims){
					//if the entry is in the upper triangle 
					int raw = rand() % 100;
					printf("random number: %d", raw);
					if (raw <= probability){
						this->data[i] = 1;
					}
					else {
						this->data[i] = 0;
					}
					//Don't add anything to the matrix in the lower triangle or diagonal
				}
			}   
			//Copy from upper to lower	
			for (int i=0; i < array_len; i++){
				int column = i % dims;
				int row = ((i - (i%dims))/dims); 
				// if this isn't divisible add or subtract 1, it should be
				if (column < row){
					//if the entry is in the lower triangle or diagonal
					int mirror = column*dims + row;
					//Compute mirror image
					this->data[i] = data[mirror];
					//This might not access data[mirror] correctly
				}
			}	
		}


		void print_matrix(){
			for (int i = 0; i < array_len; i++){
				cout << this->data[i] << " ";
			}
			cout << "\n";
		}

		void print_diags(){
			for (int i = 0; i < dims; i++){
				cout << this->data[i*dims + i] << "\n";
			}
		}

		void read(int* array_in, string option){
			if (option == "first"){
				for (int i=0; i < array_len; i++){
					this->data[i] = array_in[i+1];
				}
				
			}
			else if (option == "second"){
				for (int i = 0; i < array_len; i++){
					this->data[i] = array_in[i+array_len+1];
				}
			}
			else if (option == "all"){
				for (int i=0; i < array_len; i++){
					this->data[i] = array_in[i+1];
				}
			}
			else{
				cout << "incorrect read parameters given \n";
			}
		}

};
int* read_in(string filename){
	fstream in_file(filename);
	if (in_file.is_open())
	{	string line;

		//Count how many rows in input file
		int rows = 0;
		while (getline(in_file,line)){
			rows++;
		}
		// set the first value of the array to be the array length
		// assumes that all arrays are square
		int *data_all = new int[rows+1];
		data_all[0] = (int) (rows/2);

		// move seeker to start
		in_file.clear();
		in_file.seekg(0);

		for (int i =1; i < rows+1; i++){
			getline (in_file, line);
			data_all[i] = stoi(line);
		}


		// Close file because we have already stored the array
		in_file.close();
		return data_all;
	}
	else {
		cout << "Unable to open file";
		return NULL;
	}
}

Matrix conventional(Matrix a, Matrix b){
	// perform conventional matrix multi
	assert(a.dims == b.dims);
	Matrix output_matrix(a.array_len);
	cout << "Output matrix: " << a.dims <<"\n";

	// iterate through the output matrix
	for (int i = 0; i < output_matrix.dims; i++){
		for (int j = 0; j < output_matrix.dims; j++){
			int sum =0;
			for (int k = 0; k < output_matrix.dims; k++){
				// I val
				int a_val = a.data[(i*output_matrix.dims + k)];
				int b_val = b.data[((i+k)*output_matrix.dims + j)];
				sum += a_val*b_val;
			}
			cout << "for i = " << i << " for j = " << j << " Sum: " << sum << "\n";
			output_matrix.data[(i*output_matrix.dims) +j] = sum;
		}
	}
	return output_matrix;
}

int strass(int val){
	while (current_dim > crossover){
		//perform strassen
		strass(val);
	}
	return 0;
}

bool matrix_equal(Matrix a, Matrix b){
	assert(a.dims == b.dims);
	for (int i=0; i < a.array_len; i++){
		if (a.data[i] != b.data[i]){
			return false;
		}
	}
	return true;
}

// int triangleCount(int probability){
//     //initialize a random 1024x1024 adjacency matrix
//     Matrix mat(1024); //we hardcode this 
//     int array_len = pow(1024,2);
//     mat.init_randadjacency(probability);
//     Matrix twomat = strass(mat, mat);
//     Matrix threemat = strass(twomat, mat);
//     //count number of triangles
//     int diagonaledges = 0;
//     for (int i = 0; i < array_len; i = i + array_len + 1){
//         //We only hit the diagonal entries by adding dim + 1 to i each time
//         if (mat.data[i] == 1){
//             diagonaledges = diagonaledges + 1;
//         }
//     }
//     int triangles = diagonaledges / 6;
//     printf("Triangles: %d", triangles);
//     return (triangles);
// }

int main(){
	cout << "conventional test: \n\n";
	// Matrix mat_a(dimension^2);
	// Matrix mat_b();
	// mat_a.init_rand();
	// mat_b.init_rand();
	// mat_a.print_matrix();
	// mat_b.print_matrix();
	

	int* data_ptr = read_in("ascii_file.txt");

	Matrix mat_c(data_ptr[0]);
	mat_c.read(data_ptr, "first");
	cout << "Matrix C: ";
	mat_c.print_matrix();

	Matrix mat_d(data_ptr[0]);
	mat_d.read(data_ptr, "second");
	cout << "Matrix D: ";
	mat_d.print_matrix();
	delete data_ptr;

	cout << "\nconventional dot product: ";
	conventional(mat_c, mat_d).print_matrix();

	int* sol_ptr = read_in("solution.txt");
	Matrix mat_sol(sol_ptr[0]*2);
	mat_sol.read(sol_ptr, "all");
	cout << "\nMatrix Solution: ";
	mat_sol.print_matrix();
	delete sol_ptr;

	// check correctnes against python version

	return 0;
}
// generates the matrices based on a given size, 


// run strassen until a cutoff point n after that normal matrix


// write a diagonaliser output