#include "JoinEngine.h"

JoinEngine::JoinEngine(char const *argv[]){
    //initialise the 2 relations
    relations[0] = new Relation(argv[1], (uint64_t)atoi(argv[2]));
    relations[1] = new Relation(argv[3], (uint64_t)atoi(argv[4]));

    h1_num_of_buckets = 16;
    h1_num_of_bits = (int)log2( h1_num_of_buckets);
    h2_num_of_buckets = 16;
    h2_num_of_bits = (int)log2(h2_num_of_buckets);
}


int JoinEngine::load_relations(){
    //for every relation : open relation's binary file and load needed column
    for(int i = 0; i < 2; i++){
        //open relation's binary file
        ifstream infile;
        infile.open(relations[i]->get_name(), ios::binary | ios::in);
        //format of binary file ('|' do not exist in the file):
        //uint64_t numTuples|uint64_t numColumns|uint64_t T0C0|uint64_t T1C0|..|uint64_t TnC0|uint64_t T0C1|..|uint64_t TnC1|..|uint64_t TnCm
        read_header_info(relations[i], infile);

        //calculate size of the column & dynamically allocate space to store the column
        size_t column_size = sizeof(uint64_t) * relations[i]->get_num_of_records();
        relations[i]->set_column(column_size);

        //move file cursor to the right place and read column_size bytes; then close the binary file
        infile.seekg(column_size * relations[i]->get_column_index(), ios::cur);
        infile.read((char*)relations[i]->get_column(), column_size);
        infile.close();
    }
    cout << "Relations loaded successfully!" << endl;
}

int JoinEngine::read_header_info(Relation* relation, ifstream& infile){
    //use these variables to store what is read
    uint64_t num_of_records, num_of_columns;

    //read & set respective fields of relation
    infile.read((char*)&num_of_records, sizeof(uint64_t));
    relation->set_num_of_records(num_of_records);
    infile.read((char*)&num_of_columns, sizeof(uint64_t));
    relation->set_num_of_columns(num_of_columns);

    //printing(optional)
    printf("There are %lu records and %lu columns in relation ", relation->get_num_of_records(), relation->get_num_of_columns());
    cout << relation->get_name() << endl;
}

int JoinEngine::segmentation(){
    for(int i = 0; i < 2; i++){
        create_and_compute_hist_array(relations[i]);
        create_and_compute_psum_array(relations[i]);
        create_and_compute_new_column(relations[i]);
    }
    cout << "Both relations segmentated successfully!" << endl;
}

int JoinEngine::create_and_compute_hist_array(Relation* relation){
    //to make code more readable
    int hash_value;

    //allocate relation's hist_array and initialise
    relation->set_hist_array(h1_num_of_buckets);
    for(int j = 0; j < h1_num_of_buckets; j++)//TODO: memset
        relation->get_hist_array()[j] = 0;

    //compute hist_array
    for(int j = 0, limit = relation->get_num_of_records(); j < limit; j++){
        hash_value = h1(relation->get_column()[j]);
        relation->get_hist_array()[hash_value]++;
    }
}

int JoinEngine::create_and_compute_psum_array(Relation* relation){
    //allocate psum_array
    relation->set_psum_array(h1_num_of_buckets);

    //calculate psum_array
    relation->get_psum_array()[0] = 0;
    for(int j = 1; j < h1_num_of_buckets; j++)
        relation->get_psum_array()[j] = relation->get_psum_array()[j-1] + relation->get_hist_array()[j-1];
}

int JoinEngine::create_and_compute_new_column(Relation* relation){
    //to make code more readable
    int hash_value;

    //dynamically allocate new_column to store the column by hash value order
    relation->set_new_column(relation->get_num_of_records());

    //temporal array needed to calculate new_column. this array is copy of psum array initially
    int copy_of_psum_array[h1_num_of_buckets];
    for(int j = 0; j < h1_num_of_buckets; j++)
        copy_of_psum_array[j] = relation->get_psum_array()[j];

    //calculate new_column. new column is an array of tuples (uint64_t index, uint64_t value)
    for(int j = 0; j < relation->get_num_of_records(); j++){
        hash_value = h1(relation->get_column()[j]);
        relation->get_new_column()[copy_of_psum_array[hash_value]].set(j, relation->get_column()[j]);
        copy_of_psum_array[hash_value]++;
    }
}

int JoinEngine::indexing(){
    //choose the smallest relation to index
    Relation* relation = NULL;
    if(relations[0]->get_num_of_records() < relations[1]->get_num_of_records())
        relation = relations[0];
    else
        relation = relations[1];

    //create index for every bucket made at segmentation
    relation->set_index_array(h1_num_of_buckets);

    //to make code more readable
    int hash_value;

    //for every bucket made at segmentation
    for(int j = 0; j < h1_num_of_buckets; j++){
        Index& cur_index = relation->get_index_array()[j]; //to make code more readable

        //create bucket array and initialize it
        cur_index.set_bucket_array(h2_num_of_buckets);
        for(int i = 0; i < h2_num_of_buckets; i++)
            cur_index.get_bucket_array()[i] = -1; //-1 means that there is not previous record in the bucket

        //create chain array and initialize it
        cur_index.set_chain_array(relation->get_hist_array()[j]);
        for(int i = 0; i < relation->get_hist_array()[j]; i++)
            cur_index.get_chain_array()[i] = 0;

        //scan whole bucket in order to calculate chain and bucket array
        int upper_limit = relation->get_psum_array()[j] + relation->get_hist_array()[j];
        for(int i = relation->get_psum_array()[j]; i < upper_limit; i++){
            hash_value = h2(relation->get_new_column()[i].get_value());
            cur_index.get_chain_array()[i - relation->get_psum_array()[j]] = cur_index.get_bucket_array()[hash_value];
            cur_index.get_bucket_array()[hash_value] = i - relation->get_psum_array()[j];
        }
    }
    cout << "Indexing completed successfully!" << endl;
}

int JoinEngine::join(){
    //TODO: DYNAMICALLY
    //r0 --> NOT Indexed relation
    Relation* r0 = relations[1];
    //r1 --> Indexed relation
    Relation* r1 = relations[0];
    OutputList *outList = new OutputList();

    int counter = 0;
    //for every row in r0
    for(int i = 0; i < r0->get_num_of_records(); i++){
        //for easier reading of code
        Int_uint64_t cur_row = r0->get_new_column()[i];

        //take the bucket needed
        int bucket_num = h1(cur_row.get_value());

        //search index of r1 for this record

        int index = r1->get_index_array()[bucket_num].get_bucket_array()[h2(cur_row.get_value())];

        //cout << "Value : " << cur_row.get_value() << endl;
        while(index != -1){
            //cout << r1.new_column[index].value << " vs "  << cur_row.value << endl;
            if(r1->get_new_column()[index + r1->get_psum_array()[bucket_num]].get_value() == cur_row.get_value()){
                //tuple [cur_row.get_index() + 1, r1->get_new_column()[index + r1->get_psum_array()[bucket_num]].get_index() + 1]
                //cout <<"[" << cur_row.get_index() + 1 << " : "<<r1->get_new_column()[index + r1->get_psum_array()[bucket_num]].get_index() + 1 <<"]"<< endl;
                uint64_t row1 = cur_row.get_index() + 1;
                uint64_t row2 = r1->get_new_column()[index + r1->get_psum_array()[bucket_num]].get_index() + 1;
                //printf("%lu == %lu\n",row1,row2);
                outList->InsertData(row1, row2);
            }
            index = r1->get_index_array()[bucket_num].get_chain_array()[index];
        }
        cout << " -----------------------------" << endl;
    }
    outList->printList();
}
