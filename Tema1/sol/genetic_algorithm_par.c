#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include "genetic_algorithm.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

int read_input(sack_object **objects, int *object_count, int *sack_capacity, int *generations_count, int argc, char *argv[])
{
    FILE *fp;

    if (argc < 3) {
        fprintf(stderr, "Usage:\n\t./tema1 in_file generations_count\n");
        return 0;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        return 0;
    }

    if (fscanf(fp, "%d %d", object_count, sack_capacity) < 2) {
        fclose(fp);
        return 0;
    }

    if (*object_count % 10) {
        fclose(fp);
        return 0;
    }

    sack_object *tmp_objects = (sack_object *) calloc(*object_count, sizeof(sack_object));

    for (int i = 0; i < *object_count; ++i) {
        if (fscanf(fp, "%d %d", &tmp_objects[i].profit, &tmp_objects[i].weight) < 2) {
            free(objects);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);

    *generations_count = (int) strtol(argv[2], NULL, 10);
    
    if (*generations_count == 0) {
        free(tmp_objects);

        return 0;
    }

    *objects = tmp_objects;

    return 1;
}

void print_objects(const sack_object *objects, int object_count)
{
    for (int i = 0; i < object_count; ++i) {
        printf("%d %d\n", objects[i].weight, objects[i].profit);
    }
}

void print_generation(const individual *generation, int limit)
{
    for (int i = 0; i < limit; ++i) {
        for (int j = 0; j < generation[i].chromosome_length; ++j) {
            printf("%d ", generation[i].chromosomes[j]);
        }
        printf("\n%d - %d\n", i, generation[i].fitness);
    }
}

void print_best_fitness(const individual *generation)
{
    printf("%d\n", generation[0].fitness);
}

void compute_fitness_function_par(const sack_object *objects, individual *generation, int object_count_start, int object_count_end, int sack_capacity)
{
    int weight;
    int profit;

    for (int i = object_count_start; i < object_count_end; ++i) {
        weight = 0;
        profit = 0;

        for (int j = 0; j < generation[i].chromosome_length; ++j) {
            if (generation[i].chromosomes[j]) {
                weight += objects[j].weight;
                profit += objects[j].profit;
            }
        }

        generation[i].fitness = (weight <= sack_capacity) ? profit : 0;
    }
}

int cmpfunc(const void *a, const void *b)
{
    int i;
    individual *first = (individual *) a;
    individual *second = (individual *) b;

    int res = second->fitness - first->fitness; // decreasing by fitness
    if (res == 0) {
        int first_count = 0, second_count = 0;

        for (i = 0; i < first->chromosome_length && i < second->chromosome_length; ++i) {
            first_count += first->chromosomes[i];
            second_count += second->chromosomes[i];
        }

        res = first_count - second_count; // increasing by number of objects in the sack
        if (res == 0) {
            return second->index - first->index;
        }
    }

    return res;
}

void mutate_bit_string_1(const individual *ind, int generation_index)
{
    int i, mutation_size;
    int step = 1 + generation_index % (ind->chromosome_length - 2);

    if (ind->index % 2 == 0) {
        // for even-indexed individuals, mutate the first 40% chromosomes by a given step
        mutation_size = ind->chromosome_length * 4 / 10;
        for (i = 0; i < mutation_size; i += step) {
            ind->chromosomes[i] = 1 - ind->chromosomes[i];
        }
    } else {
        // for even-indexed individuals, mutate the last 80% chromosomes by a given step
        mutation_size = ind->chromosome_length * 8 / 10;
        for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length; i += step) {
            ind->chromosomes[i] = 1 - ind->chromosomes[i];
        }
    }
}

void mutate_bit_string_2(const individual *ind, int generation_index)
{   
    int step = 1 + generation_index % (ind->chromosome_length - 2);
    // mutate all chromosomes by a given step
    for (int i = 0; i < ind->chromosome_length; i += step) {
        ind->chromosomes[i] = 1 - ind->chromosomes[i];
    }
}

void crossover(individual *parent1, individual *child1, int generation_index)
{
    individual *parent2 = parent1 + 1;
    individual *child2 = child1 + 1;
    int count = 1 + generation_index % parent1->chromosome_length;
    

    memcpy(child1->chromosomes, parent1->chromosomes, count * sizeof(int));
    memcpy(child1->chromosomes + count, parent2->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));

    memcpy(child2->chromosomes, parent2->chromosomes, count * sizeof(int));
    memcpy(child2->chromosomes + count, parent1->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));
}

void copy_individual(const individual *from, const individual *to)
{
    memcpy(to->chromosomes, from->chromosomes, from->chromosome_length * sizeof(int));
}

void free_generation_par(individual *generation, int start, int end)
{
    int i;

    for (i = start; i < end; ++i) {
        free(generation[i].chromosomes);
        generation[i].chromosomes = NULL;
        generation[i].fitness = 0;
    }
}

void free_generation(individual *generation)
{
    int i;

    for (i = 0; i < generation->chromosome_length; ++i) {
        free(generation[i].chromosomes);
        generation[i].chromosomes = NULL;
        generation[i].fitness = 0;
    }
}

/* function that finds the maximum value on a given portion of an array from
start to end (start and end are calculated based on the thread id) */
void find_max(individual * arr, int start, int end,
 int thread_id, int *max_values_from_threds) {
   int cur_max_value = -1;
   for (int i = start; i < end; ++i) {
      if (arr[i].fitness > cur_max_value) {
         cur_max_value = arr[i].fitness;
      }
   }
   max_values_from_threds[thread_id] = cur_max_value;
}

/* function that will renew indexes of the current generation */
void swap_index(thread_arg data){
    int start = data.thread_id * data.object_count / data.nr_threads;
    int end = min((data.thread_id + 1) * data.object_count / data.nr_threads,
     data.object_count); 
    for (int i = start; i < end; ++i) {
        (*data.current_generation)[i].index = i;
    }
}

/* translating from the old generation to the enhanced one */
void swap(thread_arg data){
    individual * tmp = *data.current_generation;
    *data.current_generation = *data.next_generation;
    *data.next_generation = tmp;
}

/* updating the current genration to the sorted one */
void merge_swap(thread_arg data){
    for (int i = 0; i < data.object_count; i++) {
        (*data.current_generation)[i] = data.output[i];
    }
}

/* comparator used to compare witch of the two individuals is better */
bool compare(individual *arr1, individual *arr2, int i, int j){
    if (arr1[i].fitness == arr2[j].fitness) {
        
        int first_count = 0, second_count = 0;

        for (int z = 0; z < arr1[i].chromosome_length &&
         z < arr2[j].chromosome_length; ++z) {
            first_count += arr1[i].chromosomes[z];
            second_count += arr2[j].chromosomes[z];
        }

        if (first_count == second_count){
            if (arr1[i].index > arr2[j].index){
                return true;
            } else {
                return false;
            }
        } else if (first_count < second_count){
            return true;
        } else {
            return false;
        }

    } else if (arr1[i].fitness > arr2[j].fitness) {
        return true;
    } else {
        return false;
    }
}

/* function that will allow us to merge any given number of sorted arrays
in a sorted output that consists of all the given arrays */
void merge_sorted_arrays(thread_arg data, individual *output, int i, 
int j, int nr_elem_per_thread) {  
    int x = 0;
    int y = 0;
    int z = 0;
    /* recalucalting size of the first half */
    int first_half = nr_elem_per_thread * ((( i + j ) / 2 ) - i + 1);
    /* recalucalting size of the second half */
    int second_half  = nr_elem_per_thread * ( j - (( i + j) / 2));
    
    individual first[first_half];
    individual second[second_half];

    /* if one array is in range */
    if(i == j)
    {
        for(int k = 0; k < nr_elem_per_thread; k++)
            output[k] = data.arr[i][k];
        return;
    }
    
    /* if only two arrays are left the merge them */
    if(j - i == 1)
    {
        /* combine the two remaining arrays in one */
        while (1) {
            if (x < nr_elem_per_thread && y < nr_elem_per_thread){
                if (compare(data.arr[i], data.arr[j], x, y)) {
                    output[z++] = data.arr[i][x++];
                } else {
                    output[z++] = data.arr[j][y++];
                }
            } else {
                break;
            }
        }
        /* in case there are any elements left in data.arr[i] store them to 
        output */ 
        for (; x < nr_elem_per_thread; ) {
            output[z++] = data.arr[i][x++];
        }
        /* in case there are any elements left in data.arr[j] store them to 
        output */ 
        for (; y < nr_elem_per_thread; ) {
            output[z++] = data.arr[j][y++];
        }

        return;
    }
     
    /* the array should be divided into two parts */
    /* integrate the parts in the first half of the array in a recursive manner */
    merge_sorted_arrays(data, first, i, ((i + j) / 2 ), nr_elem_per_thread);
    /* integrate the parts in the second half of the array in a recursive manner */
    merge_sorted_arrays(data, second, ((i + j) / 2) + 1, j, nr_elem_per_thread);
    /* combine the first and second halves */
    while (1) {
        if (x < first_half && y < second_half){
            if (compare(first, second, x, y)){
                output[z++] = first[x++];
            } else {
                output[z++] = second[y++];
            }
        } else {
            break;
        }
    }
    /* in case there are any elements left in the first array store them to 
    output */ 
    for (; x < first_half; ) {
        output[z++] = first[x++];
    }
    /* in case there are any elements left in the second array store them to 
    output */ 
    for (; y < second_half; ) {
        output[z++] = second[y++];
    }

    return;
}

void *run_genetic_algorithm_par(void *arg) {
    thread_arg data = *(thread_arg *) arg;
    int cursor, count;
    int start;
    int end;
    int start_obj_count = data.thread_id * data.object_count / data.nr_threads;
    int end_obj_count = min((data.thread_id  + 1) * 
    data.object_count / data.nr_threads, data.object_count);
    
    /* iterate for each generation */
    for (int k = 0; k < data.generations_count; ++k) {
        cursor = 0;
        
        pthread_barrier_wait(data.pthread_barrier);
        /* computing the fitness of the current genration */
        compute_fitness_function_par(data.objects, *data.current_generation, 
        start_obj_count, end_obj_count, data.sack_capacity);
        /* waiting for all threads to finish computing fitness on
        their part */
        pthread_barrier_wait(data.pthread_barrier);

        /* each thread will sort a part of the current generation */
        qsort(*data.current_generation + start_obj_count, 
        (end_obj_count - start_obj_count), sizeof(individual), cmpfunc);
        /* waiting for all threads to finish sorting their part */
        pthread_barrier_wait(data.pthread_barrier);

        /* each of the parts that were sorted previously are stored in an array
        in order to be merged later */
        memcpy(&data.arr[data.thread_id][0], 
        *data.current_generation + start_obj_count, 
        (end_obj_count - start_obj_count) * sizeof(individual));
        pthread_barrier_wait(data.pthread_barrier);

        /* on a single thread we are merging all the sorted arrays in one 
        sorted array and we are updating the current generation to it */
        if (data.thread_id == 0) {
            merge_sorted_arrays(data, data.output, 0, 
            data.nr_threads - 1, data.nr_per_thread);
            merge_swap(data);
        }
        /* waiting for the thread with the id 0 to finish merging in 
        order to apply the next operation on the full sorted array */
        pthread_barrier_wait(data.pthread_barrier);

        /* keep first 30% children (elite children selection) */
        count = data.object_count * 3 / 10;
        /* spreading the array in parts for each thread */
        start = data.thread_id * count / data.nr_threads;
        end = min((data.thread_id + 1) * count / data.nr_threads, count); 
        
        for (int i = start; i < end; ++i) {
            copy_individual(*data.current_generation + i, 
            *data.next_generation + i);
        }
        cursor = count;

        /* mutate first 20% children with the first version of bit string 
        mutation */
        count = data.object_count * 2 / 10;
        /* updating start and stop for each thread base on the new count */
        start = data.thread_id * count / data.nr_threads;
        end = min((data.thread_id + 1) * count / data.nr_threads, count); 

        for (int i = start; i < end; ++i) {
            copy_individual(*data.current_generation + i, 
            *data.next_generation + cursor + i);
            mutate_bit_string_1(*data.next_generation + cursor + i, k);

            copy_individual(*data.current_generation + i + count, 
            *data.next_generation + (cursor + count) + i);
            mutate_bit_string_2(*data.next_generation + (cursor + count) + i, k);
        }

        cursor += 2 * count;
        // crossover first 30% parents with one-point crossover
        // (if there is an odd number of parents, the last one is kept as such)
        count = data.object_count * 3 / 10;
        if (count % 2 == 1) {
            if(data.thread_id == 0) {
            	copy_individual(*data.current_generation + data.object_count - 1, 
                *data.next_generation + cursor + count - 1);
            }
            count--;
        }

        /* updating start and stop for each thread base on the new count */
        start = data.thread_id * count / data.nr_threads;
        end = min((data.thread_id + 1) * count / data.nr_threads, count - 1);

        for (int i = start; i < end; i += 2) {
            crossover(*data.current_generation + i, *data.next_generation + cursor + i, k);
        }
        pthread_barrier_wait(data.pthread_barrier);

        /* switch to new generation */
        if (data.thread_id == 0) {
            swap(data);
        }
        pthread_barrier_wait(data.pthread_barrier);

        swap_index(data);

        if (data.thread_id == 0) {
            if (k % 5 == 0) {
                print_best_fitness(*data.current_generation);
            }
        }
    }
    
    /* computing the fitness of the current genration */
    compute_fitness_function_par(data.objects, *data.current_generation, 
    start_obj_count, end_obj_count, data.sack_capacity);
    /* waiting for all threads to finish computing fitness on
    their part */
    pthread_barrier_wait(data.pthread_barrier);

    /* splitting the array in order that each one will search for the maximum
    value and store it inside max_values_from_thread */
    int start_find = data.thread_id * data.object_count / data.nr_threads;
    int end_find = min((data.thread_id  + 1) * data.object_count / data.nr_threads, data.object_count);
    
    find_max(*data.current_generation, start_find, end_find,
    data.thread_id, data.max_values_from_thread);
    pthread_barrier_wait(data.pthread_barrier);

    /* joining all the parts in order to find the maximum in the hole array */
    if (data.thread_id == 0) {
        int final_max_val = data.max_values_from_thread[0];
        for (int i = 0; i < data.nr_threads; ++i) {
            if (data.max_values_from_thread[i] > final_max_val) {
                final_max_val = data.max_values_from_thread[i];
            }
        }
        printf("%d\n", final_max_val);
    }
    
    pthread_exit(NULL);
}