int test1(){
	LSQ_HandleT a = LSQ_CreateSequence();
	LSQ_DeleteFrontElement(a);
	LSQ_DeleteRearElement(a);
	LSQ_DestroySequence(a);
	return 1;
}

int test2(){
	LSQ_HandleT a = LSQ_CreateSequence();
	for (int i = 0; i <= 199; i++) {
		LSQ_InsertElement(a, i-200, rand()%50);
		LSQ_InsertElement(a, i+1, rand()%50);
	}
	LSQ_DestroyIterator(iter);
	LSQ_DestroySequence(a);
	return r;
}
