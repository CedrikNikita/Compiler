for(i = 0; i <= abs(max_val) + abs(min_val) ; i++) count[i] = 0;
	for(i = 0; i < n; i++) {
		scanf("%d", &z);
		if(z > max_val) max_val = z;
		if(z < min_val) min_val = z;
		count[z  + abs(min_val)]++;
		z = 0;
	}
	freopen("output0.txt", "w", stdout);
	for(i = 0; i <= abs(max_val) + abs(min_val); i++)
		for(k = 0; k < count[i]; k++){
			z++;
		
