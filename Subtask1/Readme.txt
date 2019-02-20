COP290 Design Practices Task1 Subtask2
Shayan Aslam Saifi 2017CS10375
Shivam Sheshrao Jadhav 2017CS10378

All functions take input in column major form
But print output in row major form.

We have implemented matrix MULTIPLICATION using different libraries i.e mkl open blas and pthread.

Before executing makefile go to intel/mkl/bin folder and give command: source mklvars.sh intel64

****  In place of (library) specify which library to be used: mkl / openblas / pthread   *****
Convolution(co):

	Input format- ./t1s2_(library) co padsize matrix matrix_size kernel kernel_size
		e.g.- ./t1s2_mkl co 1 matrix1.txt 3 kernel.txt 2

	Kernel size should be smaller than matrix size.
	For NO padding give padsize =0.
	Only square matrices as inputs.

Conv-Mult(mu):

	Input format- ./t1s2_(library) mu padsize matrix matrix_size kernel kernel_size
		e.g.- ./t1s2_openblas mu 1 matrix1.txt 3 kernel.txt 2

	Kernel size should be smaller than matrix size.
	For NO padding give padsize =0.
	Only square matrices as inputs.

Non-linear activations(ac):

	Input format- ./t1s2_(library) ac activation_function(rel/tan) matrix numRows numCols
		e.g.- ./t1s2_pthread ac rel matrix.txt 3 2

	Any matrix can be given as input.
	Available activation functions are RELU and tanh.

Pooling(po):

	Input Format- ./t1s2_(library) po Pooling_function(max/avg) matrix Input_size Output_size
		e.g.- ./t1s2_mkl po max matrix1.txt 4 2

	Sqaure matrix only.
	Available pooling functions are Max and Avg.
	Size of output matrix should be specified.
	Works only if in_size is divisible by out_size.

float-probability(pr):

	Input Format- ./t1s2_(library) pr prob_function(sof/sig) matrix input_size
		e.g.- ./t1s2_pthread pr sof matrix1.txt 3

	Square matrix only.
	Available functions are Softmax(sof) and Sigmoid(sig).
	
