#include "common.h"

void logMakingPizza(int pizza_no){
    printf("%d CH [%s] Przygotowuje pizze: %d\n", (int) getpid(), getTimeAsString(), pizza_no);
}

void logAddPizza(int pizza_no, int pizzas_in_cooks){
    printf("%d CH [%s] Dodałem pizze: %d Liczba pizz w piecu: %d\n", (int) getpid(), getTimeAsString(), pizza_no, pizzas_in_cooks);
}

void logRemovePizzaFromOven(int pizza_no, int pizzas_in_cooks, int pizzas_in_table){
    printf("%d CH [%s] Wyjmuję pizze: %d Liczba pizz w piecu: %d Liczba pizz na stole: %d\n", (int) getpid(), getTimeAsString(), pizza_no, pizzas_in_cooks, pizzas_in_table);
}

int findEmptyPlaceAndReplaceWithValue(int *array, int n, int value){
    for(int i = 0; i < n; i++){
        if(array[i] == EMPTY){
            array[i] = value;
            return i;
        }
    }
}

int putPizzaToOven(int pizza_no){
    make_semaphore_operation(OVENS_COUTING_SEMAPHORE, WAIT);
    make_semaphore_operation(OVENS_FILE_OPERATION_SEMAPHORE, WAIT);

    int *ovens = attach_memory_block(OVENS_KEY, N_OF_OVENS);
    int oven_index = findEmptyPlaceAndReplaceWithValue(ovens, N_OF_OVENS, pizza_no);

    detach_memory_block(ovens);
    
    logAddPizza(pizza_no, calculatePizzas(OVENS_COUTING_SEMAPHORE, N_OF_OVENS));

    make_semaphore_operation(OVENS_FILE_OPERATION_SEMAPHORE ,POST);

    return oven_index;
}

void getPizzaFromOvenAndPutItInTable(int oven_number){
    make_semaphore_operation(OVENS_FILE_OPERATION_SEMAPHORE ,WAIT);

    int *ovens = attach_memory_block(OVENS_KEY, N_OF_OVENS);
    int pizza_no = ovens[oven_number];

    ovens[oven_number] = EMPTY;

    detach_memory_block(ovens);
    
    int pizzasInOven = calculatePizzas(OVENS_COUTING_SEMAPHORE, N_OF_OVENS);

    make_semaphore_operation(OVENS_COUTING_SEMAPHORE, POST);
    make_semaphore_operation(OVENS_FILE_OPERATION_SEMAPHORE, POST);

    make_semaphore_operation(TABLES_COUTING_SEMAPHORE, WAIT);
    make_semaphore_operation(TABLES_FILE_OPERATION_SEMAPHORE, WAIT);

    int *tables = attach_memory_block(TABLES_KEY, N_OF_TABLES);
    int table_number = findEmptyPlaceAndReplaceWithValue(tables, N_OF_TABLES, pizza_no);

    logRemovePizzaFromOven(pizza_no, pizzasInOven, calculatePizzas(TABLES_COUTING_SEMAPHORE, N_OF_TABLES));

    detach_memory_block(tables);

    make_semaphore_operation(TABLES_TABLES_EXTRA_COUNTING, POST);
    make_semaphore_operation(TABLES_FILE_OPERATION_SEMAPHORE, POST);
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    int pizza_type, oven_number;

    while(1){
        pizza_type = rand()%10;
        logMakingPizza(pizza_type);
        sleep(rand()%2 +1);
        oven_number = putPizzaToOven(pizza_type);
        sleep(rand()%2 +4);
        getPizzaFromOvenAndPutItInTable(oven_number);
    }

    return 0;
}