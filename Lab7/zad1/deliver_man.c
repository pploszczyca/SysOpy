#include "common.h"

void logPizzaTaken(int pizza_no, int pizzas_in_table){
    printf("%d DM [%s] Pobieram pizze: %d Liczba pizz na stole: %d\n", (int) getpid(), getTimeAsString(), pizza_no, pizzas_in_table);
}

void logDeliveredPizza(int pizza_no){
    printf("%d DM [%s] Dostarczam pizze: %d\n", (int) getpid(), getTimeAsString(), pizza_no);
}

int getPizzaFromTable(){
    make_semaphore_operation(TABLES_KEY, TABLES_EXTRA_COUNTING ,-1);
    make_semaphore_operation(TABLES_KEY, FILE_OPERATION_SEMAPHORE ,-1);

    int *table = attach_memory_block(TABLES_KEY, N_OF_TABLES);
    int pizza_type = -1;

    for(int i = 0; i < N_OF_TABLES; i++){
        if(table[i] != EMPTY){
            pizza_type = table[i];
            table[i] = EMPTY;
            break;
        }
    }

    logPizzaTaken(pizza_type, calculatePizzas(TABLES_KEY, N_OF_TABLES));

    detach_memory_block(table);

    make_semaphore_operation(TABLES_KEY, FILE_OPERATION_SEMAPHORE ,1);
    make_semaphore_operation(TABLES_KEY, COUTING_SEMAPHORE ,1);

    return pizza_type;
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    int pizza_type;

    while(1){
        pizza_type = getPizzaFromTable();
        sleep(rand()%2+4);
        logDeliveredPizza(pizza_type);
        sleep(rand()%2+4);
    }
    
    return 0;
}