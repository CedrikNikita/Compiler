/* Все последующие элементы смещаются на одну позицию в сторонsу начала.                    */
/* Заданный итератор продолжает указывать на элемент последовательности с тем же индексом. */
void LSQ_DeleteGivenElement(LSQ_IteratorT iterator){
    IteratorT* a = (IteratorT*)iterator;
    a->Array_T->array = LSQ_ResizeDec(a->Array_T);
    memmove(a->Array_T->array + a->index, a->Array_T->array + a->index + 1, (a->Array_T->size -1 - a->index)*sizeof(LSQ_BaseTypeT));
    a->Array_T->size--;
}
 
