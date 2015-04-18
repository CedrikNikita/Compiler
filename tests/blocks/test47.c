typedef struct NodeT {
    LSQ_KeyT key;
    LSQ_BaseTypeT value;
    struct NodeT *next;
} NodeT;
 
typedef struct {
    LSQ_SizeT size;
    NodeT **array;
    LSQ_Callback_CloneFuncT *keyClone;
    LSQ_Callback_SizeFuncT *keySize;
    LSQ_Callback_CompareFuncT *keyComp;
    LSQ_Callback_CloneFuncT *valClone;
} HashT;
 
typedef struct IteratorT{
    int flag;
    HashT *hash;
    int arrayIndex;
    NodeT *node;
} IteratorT;

LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_KeyT key){
    if (handle == LSQ_HandleInvalid) return NULL;
    HashT* hash = (HashT*)handle;
    int ind = hashf(hash, key);
    NodeT *node;
    for (node = hash->array[ind]; node != NULL && hash->keyComp(node->key, key); node = node->next);
    if (node == NULL) return LSQ_GetPastRearElement(handle);
    else return CreateIterator(hash, ind, node, 0);
 
}
