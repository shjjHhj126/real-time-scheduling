#include "schedule.h"

int main()
{
    stream *success = malloc(table_number * sizeof(success));
    stream *fail = malloc(sizeof(success));
    build_table(success, fail);
    free(success);
    free(fail);
    return 0;
}
