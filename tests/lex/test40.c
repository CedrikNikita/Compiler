/* Функция, создающая пустой контейнер. Возвращает назначенный ему дескриптор */
LSQ_HandleT LSQ_CreateSequence(void){
    ArrayT* a = malloc(sizeof(ArrayT));
    a->array = malloc(init * sizeof(LSQ_BaseTypeT));
    a->size = 0;
    a->real_size = init;
    return a;
}
 
/*Функция осуществляющая проверку*/
LSQ_HandleT LSQ_ResizeDec(LSQ_HandleT handle){
    ArrayT* a = (ArrayT*)handle;
    if (a->size + init < a->real_size/resize){
        a->real_size = a->real_size/resize;
        return realloc(a->array, a->real_size);
    }
    else return a->array;
}
 
/*Функция осуществляющая проверку*/
LSQ_HandleT LSQ_ResizeInc(LSQ_HandleT handle){
    ArrayT* a = (ArrayT*)handle;
    if(a->size == a->real_size){
        a->real_size = a->real_size * resize;
        return realloc(a->array, a->real_size);
    }
    else return a->array;
}
 
