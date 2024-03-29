#include <stdio.h>
#include <stdlib.h>
#include <omp-tools.h>
#include <assert.h>

void my_thread_start_callback(ompt_thread_t thread_type, ompt_data_t* data) {
    switch(thread_type) {
        case ompt_thread_initial:
            printf("Initial thread begins\n");
            break;
        case ompt_thread_worker:
            printf("Worker thread begins\n");
            break;
        case ompt_thread_other:
            printf("Other thread begins\n");
            break;
        default:
            printf("Unknown thread type begins\n");
    }
}

void my_thread_end_callback(ompt_data_t* data) {
    printf("Thread ends\n");
}

void my_task_create_callback(
    ompt_data_t *encountering_task_data,
    const ompt_frame_t *encountering_task_frame,
    ompt_data_t *new_task_data,
    int flags,
    int has_dependences,
    const void *codeptr_ra)
{
        switch (flags) {
                case ompt_task_initial:
                        printf("initial task\n");
                        break;
                case ompt_task_implicit:
                        printf("implicit task\n");
                        break;
                case ompt_task_target:
                        printf("target task\n");
                        break;
                case ompt_task_taskwait:
                        printf("taskwait task\n");
                        break;
                case ompt_task_explicit:
                        printf("explicit task\n");
                        break;
                case ompt_task_undeferred:
                        printf("undeferred task\n");
                        break;
                case ompt_task_final:
                        printf("task is final\n");
                        break;
                case ompt_task_untied:
                        printf("task is untied\n");
                        break;
                case ompt_task_mergeable:
                        printf("task is mergeable\n");
                        break;
                case ompt_task_merged:
                        printf("task is merged\n");
                        break;
                default:
                        printf("unknown task type\n");
                //      printf("new_task_data: %d\n", new_task_data);
                        printf("flags: %u\n", (unsigned int)flags);
                        printf("Size of flags: %zu bytes\n", sizeof(flags));
                        break;
    }
}

int ompt_initialize(ompt_function_lookup_t lookup, int initial_device_num, ompt_data_t* data) {
    ompt_set_callback_t ompt_set_callback = (ompt_set_callback_t)(lookup("ompt_set_callback"));
    assert(ompt_set_callback != NULL);

    ompt_set_result_t result = ompt_set_error;

    result = ompt_set_callback(ompt_callback_thread_begin, (ompt_callback_t)(my_thread_start_callback));
    assert(result == ompt_set_always);

    result = ompt_set_callback(ompt_callback_thread_end, (ompt_callback_t)(my_thread_end_callback));
    assert(result == ompt_set_always);

    result = ompt_set_callback(ompt_callback_task_create, (ompt_callback_t)(my_task_create_callback));
    assert(result == ompt_set_always);

    return 1;
}

void ompt_finalize(ompt_data_t* data) {
    assert(data->ptr != NULL);
    ompt_start_tool_result_t* result = (ompt_start_tool_result_t*)(data->ptr);
    free(result);
}

ompt_start_tool_result_t* ompt_start_tool(unsigned int omp_version, const char* runtime_version) {
    ompt_start_tool_result_t* result = (ompt_start_tool_result_t*)malloc(sizeof(ompt_start_tool_result_t));
    result->initialize = ompt_initialize;
    result->finalize = ompt_finalize;
    result->tool_data.ptr = result;

    return result;
}
