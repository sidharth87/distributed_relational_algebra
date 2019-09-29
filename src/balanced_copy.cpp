#include "balanced_hash_relation.h"
#include "comm.h"
#include "parallel_join.h"
#include "RA_tasks.h"


int main(int argc, char **argv)
{
    mpi_comm mcomm;
    mcomm.create(argc, argv);
    mcomm.set_number_of_buckets(1);

    int arity = 2;
    u32 sub_buckets_per_bucket_G = 1;
    u32 sub_buckets_per_bucket_T = 1;

    u32 join_column_count = 1;

    relation* G = new relation();
    relation* T = new relation();
    G->initialize(arity, join_column_count, STATIC, sub_buckets_per_bucket_G, argv[1], FULL, mcomm);

    int rename_and_project_copy[2] = {1,0};
    T->initialize_empty(arity, join_column_count, DYNAMIC, sub_buckets_per_bucket_T, mcomm);

    parallel_RA copy_1(COPY, mcomm);
    copy_1.copy_input(G, FULL);
    copy_1.copy_output(T);
    copy_1.set_projection_index(rename_and_project_copy);

    RAM scc0(mcomm);
    scc0.push_back(copy_1);
    scc0.execute();



    mcomm.destroy();

    return 0;
}
