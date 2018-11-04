## Uoa Software Engineering Project_Part_1

### Introduction:<br><br>
* This repository implements an engine that performs join operations in relations.<br>
* Relations are read from binary file which has a format of: <br>
uint64_t numTuples|uint64_t numColumns|uint64_t T0C0|uint64_t T1C0|..|uint64_t TnC0|uint64_t T0C1|..|uint64_t TnC1|..|uint64_t TnCm<br>
* The engine uses the radix hash join algorithm to join the relations.<br>
* It supports only 2 relations and only " = " operator yet.<br>
* Join results are written in a special list that holds singly linked buffers of 1MB<br>
* Unit tests are developed with the help of the google test library
* Compile & Run Script included
* Compile & run Script for unit tests included
* 8 million & 8 million records Join in ~1.8 sec (i7-7700HQ)<br>
##### Future Releases: <br>
* Support for multiple relations <br>
* Support for multiple joins <br>
* Support for more operators <br>
* Multithreading support <br> 

<hr>

### Loading Relations: <br>
JoinEngine class implements int load_relations(); <br>
This function opens the relation files (which are given as command line arguments) and for each file reads the first 16 bytes to determine the number of columns and records<br>
Then allocates an array and loads data to memory <br>
<hr>

### Radix Hash Join <br>
##### Relation Segmentation <br>
JoinEngine class implements int segmentation(); <br>
This function is divided in 3 parts:<br>
int create_and_compute_hist_array(relations[i]); <br>
Computes and stores the histogram of the entries with the same hash value <br>
int create_and_compute_psum_array(relations[i]);<br>
Computes and stores the indexes of the buckets
int create_and_compute_new_column(relations[i]);<br>
Creates a new column which is a hash table and it is segmented by bucket

##### Relation Indexing <br>
JoinEngine class implements int indexing(); <br>
This function creates an index for the smallest relation <br>
The index it is actually a second hash map and consists of a bucket and a chain array for each bucket <br>

##### Relations Join <br>
JoinEngine class implements int join(); <br>
This function scans the entries of the non-indexed relation, hashes them properly to find the matching bucket and hashes them again to match them with the records inside the bucket <br>
The matching results are written in the output List <br>

### Join Results <br>
Join Engine class holds an output list with the results to be used later
