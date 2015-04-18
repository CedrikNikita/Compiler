void LSQ_DestroySequence(LSQ_HandleT handle){
	if(handle == LSQ_HandleInvalid) return;
	ListT* a = (ListT*)handle;                                     
	while(a->size) LSQ_DeleteFrontElement(a);
	free(a);
}

/* Функция, удаляющая первый элемент контейнера */ 
void LSQ_DeleteFrontElement(LSQ_HandleT handle){
	if(((ListT*)handle)->size == 0) return;
	ListT* a = (ListT*)handle;
	DeleteNode(handle, a->main_root);
}

int test0(){
	LSQ_HandleT a = LSQ_CreateSequence();
	LSQ_DestroySequence(a);
	return 1;
}

int test1(){
	LSQ_HandleT a = LSQ_CreateSequence();
	LSQ_InsertElement(a, 5, 111);
	LSQ_InsertElement(a, 8, 111);
	/*LSQ_InsertElement(a, 0, 111);
	LSQ_InsertElement(a, 2, 111);*/
	LSQ_DestroySequence(a);
	return 1;
