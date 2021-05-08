#include "common.h"

void logMakingPizza(int pizza_no){
    printf("%d [%s] Przygotowuje pizze: %d\n", (int) getpid(), getTimeAsString(), pizza_no);
}

void logAddPizza(int pizza_no, int pizzas_in_cooks){
    printf("%d [%s] Dodałem pizze: %d Liczba pizz w piecu: %d\n", (int) getpid(), getTimeAsString(), pizza_no, pizzas_in_cooks);
}

void logRemovePizzaFromOven(int pizza_no, int pizzas_in_cooks, int pizzas_in_table){
    printf("%d [%s] Wyjmuję pizze: %d Liczba pizz w piecu: %d Liczba pizz na stole: %d\n", (int) getpid(), getTimeAsString(), pizza_no, pizzas_in_cooks, pizzas_in_table);
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
    int *ovens = attach_memory_block(OVENS_SHARED_MEMORY_KEY, N_OF_OVENS);
    int oven_index = findEmptyPlaceAndReplaceWithValue(ovens, N_OF_OVENS, pizza_no);
    
    logAddPizza(pizza_no, calculatePizzas(ovens, N_OF_OVENS));
    detach_memory_block(ovens);

    return oven_index;
}

void getPizzaFromOvenAndPutItInTable(int oven_number){
    int *ovens = attach_memory_block(OVENS_SHARED_MEMORY_KEY, N_OF_OVENS);
    int pizza_no = ovens[oven_number];

    ovens[oven_number] = EMPTY;
    
    int *tables = attach_memory_block(TABLES_SHARED_MEMORY_KEY, N_OF_TABLES);
    int table_number = findEmptyPlaceAndReplaceWithValue(tables, N_OF_TABLES, pizza_no);

    logRemovePizzaFromOven(pizza_no, calculatePizzas(ovens, N_OF_OVENS), calculatePizzas(tables, N_OF_TABLES));

    detach_memory_block(tables);
    detach_memory_block(ovens);
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