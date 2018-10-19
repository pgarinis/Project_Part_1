#include "JoinEngine.h"


JoinEngine::JoinEngine(char const *argv[]){
    relations[0].name = argv[1];
    relations[0].column_index = atoi(argv[2]);
    relations[0].column = NULL;

    relations[1].name = argv[3];
    relations[1].column_index = atoi(argv[4]);
    relations[1].column = NULL;

    h1_num_of_buckets = 16;
    h2_num_of_buckets = 16;
    main_engine();
}

int JoinEngine::main_engine(){
    load_info();
    segmentation();
    indexing(relations[1]);
    join(relations[0], relations[1]);
}

int JoinEngine::load_info(){
    for(int i = 0; i < 2; i++){
        //open relation's binary file
        //format of binary file ('|' do not exist in the file):
        //uint64_t numTuples|uint64_t numColumns|uint64_t T0C0|uint64_t T1C0|..|uint64_t TnC0|uint64_t T0C1|..|uint64_t TnC1|..|uint64_t TnCm
        ifstream infile;
        infile.open(relations[i].name, ios::binary | ios::in);

        //read number of records and number of columns in the relation
        infile.read((char*)&relations[i].num_of_records, sizeof(uint64_t));
        infile.read((char*)&relations[i].num_of_columns, sizeof(uint64_t));
        printf("There are %lu records and %lu columns in relation ", relations[i].num_of_records, relations[i].num_of_columns);
        cout << relations[i].name << endl;

        //calculate size of column
        size_t column_size = sizeof(uint64_t) * relations[i].num_of_records;

        //dynamically allocate space to store column
        relations[i].column = (uint64_t*)malloc(column_size);

        //move file pointer to the right place and read column_size bytes; then close the binary file
        infile.seekg(column_size * relations[i].column_index, ios::cur);
        infile.read((char*)relations[i].column, column_size);
        infile.close();
    }
    cout << "Relations loaded successfully!" << endl;
}

int JoinEngine::segmentation(){
    int hash_value; //to make code more readable
    for(int i = 0; i < 2; i++){
        //allocate relation's hist_array and initialize
        relations[i].hist_array = (int*)malloc(sizeof(int) * h1_num_of_buckets);
        for(int j = 0; j < h1_num_of_buckets; j++)
            relations[i].hist_array[j] = 0;

        //calculate hist_array
        for(int j = 0; j < relations[i].num_of_records; j++){
            hash_value = h1(relations[i].column[j]);
            relations[i].hist_array[hash_value]++;
        }
        // cout << "Histogram : " << endl;
        // for(int j = 0; j < h1_num_of_buckets; j++)
        //     cout << j << ". " << relations[i].hist_array[j] << endl;

        //allocate psum_array
        relations[i].psum_array = (int*)malloc(sizeof(int) * h1_num_of_buckets);

        //calculate psum_array
        relations[i].psum_array[0] = 0;
        for(int j = 1; j < h1_num_of_buckets; j++)
            relations[i].psum_array[j] = relations[i].psum_array[j-1] + relations[i].hist_array[j-1];
        // cout << "Psum : " << endl;
        // for(int j = 0; j < h1_num_of_buckets; j++)
        //     cout << j << ". " << relations[i].psum_array[j] << endl;

        //dynamically allocate new_column to store the column by bucket order
        relations[i].new_column = (Int_uint64_t*)malloc(sizeof(Int_uint64_t) * relations[i].num_of_records);

        //temporal array needed to calculate new_column. this array is copy of psum array initially
        int copy_of_psum_array[h1_num_of_buckets];
        for(int j = 0; j < h1_num_of_buckets; j++)
            copy_of_psum_array[j] = relations[i].psum_array[j];

        //calculate new_column
        for(int j = 0; j < relations[i].num_of_records; j++){
            hash_value = h1(relations[i].column[j]);
            relations[i].new_column[copy_of_psum_array[hash_value]] = Int_uint64_t(j, relations[i].column[j]);
            copy_of_psum_array[hash_value]++;
        }
    }
    cout << "Both relations segmentated successfully!" << endl;
}

int JoinEngine::indexing(Relation& relation){
    //create index for every bucket made at segmentation
    relation.index_array = (Index*)malloc(h1_num_of_buckets * sizeof(Index));

    //to make code more readable
    int hash_value;

    //for every bucket made at segmentation
    for(int j = 0; j < h1_num_of_buckets; j++){
        Index& cur_index = relation.index_array[j]; //to make code more readable

        //create bucket array and initialize it
        cur_index.bucket_array = (int*)malloc(sizeof(int) * h2_num_of_buckets);
        for(int i = 0; i < h2_num_of_buckets; i++)
            cur_index.bucket_array[i] = -1; //-1 means that there is not previous record in the bucket

        //create chain array and initialize it
        cur_index.chain_array = (int*)malloc(sizeof(int) * relation.hist_array[j]);
        for(int i = 0; i < relation.hist_array[j]; i++)
            cur_index.chain_array[i] = 0;

        //scan whole bucket in order to calculate chain and bucket array
        int upper_limit = relation.psum_array[j] + relation.hist_array[j];
        for(int i = relation.psum_array[j]; i < upper_limit; i++){
            hash_value = h2(relation.new_column[i].value);
            cur_index.chain_array[i - relation.psum_array[j]] = cur_index.bucket_array[hash_value];
            cur_index.bucket_array[hash_value] = i - relation.psum_array[j];
        }
    }
    cout << "Indexing completed successfully!" << endl;
    // int index = relation.index_array[2].bucket_array[0];
    // while(index != -1){
    //     cout << index << " --> ";
    //     int a = getchar();
    //     index = relation.index_array[2].chain_array[index];
    // }
    // cout << "END" << endl;
}

int JoinEngine::join(Relation r0, Relation r1){
    //r0 --> NOT Indexed relation
    //r1 --> Indexed relation

    //for every row in r0
    for(int i = 0; i < r0.num_of_records; i++){
        //for easier reading of code
        Int_uint64_t cur_row = r0.new_column[i];

        //take the bucket needed
        int bucket_num = h1(cur_row.value);

        //search index of r1 for this record

        int index = r1.index_array[bucket_num].bucket_array[h2(cur_row.value)];
        cout << " -----------------------------" << endl;
        cout << cur_row.value << "~~~~"<< endl;
        while(index != -1){
            //cout << r1.new_column[index].value << " vs "  << cur_row.value << endl;
            if(r1.new_column[index + r1.psum_array[bucket_num]].value == cur_row.value)
                cout << r1.new_column[index + r1.psum_array[bucket_num]].index << endl;
            index = r1.index_array[bucket_num].chain_array[index];
        }
    }
}
