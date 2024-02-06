#include <stdio.h>
#include <stdlib.h>
#include <omp-tools.h>

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

int ompt_initialize(ompt_function_lookup_t lookup, ompt_data_t* data) {
        ompt_set_callback_t ompt_set_callback = (ompt_set_callback_t)(lookup("ompt_set_callback"));
        assert(ompt_set_callback != NULL);

        ompt_set_result_t result = ompt_set_error;

        result = ompt_set_callback(ompt_callback_thread_begin, (ompt_callback_t)(my_thread_start_callback));
        assert(result == ompt_set_always);

        result = ompt_set_callback(ompt_callback_thread_end, (ompt_callback_t)(my_thread_end_callback));
        assert(result == ompt_set_always);

        return 0;
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

