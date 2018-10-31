class Index{
    int* bucket_array;
    int* chain_array;
public:
    //getters
    int* get_bucket_array(){ return this->bucket_array; };
    int* get_chain_array(){ return this->chain_array; };

    //setters
    void set_bucket_array(int size){ this->bucket_array = (int*) malloc(size * sizeof(int)); }
    void set_chain_array(int size){ this->chain_array = (int*)malloc(size * sizeof(int)); }
};
