#ifndef ROOTING_H
#define ROOTING_H


typedef struct RootEntity {
	Peer peer;
	short sherlocks;
	unsigned short metric;
	RootEntity* pNext;
};

signed char rt_compare(RootEntity* node1, RootEntity* node2) {
	if(node1 != NULL && node2 != NULL)
	{
		return ( (node1->sherlocks - node2->sherlocks) / (node1->sherlocks + node2->sherlocks) + 
			+ (node2->metric - node1->metric) / (node2->metric + node1->metric) ) > 0.f ? 1 : -1;
	} 
	else 
	{
		return (node1 >= node2) ? 1 : -1;
	}
}

RootEntity* rt_insert(RootEntity* node, Peer* src_peer, short sherlocks, short metric) 
{
	RootEntity* tmp = malloc(sizeof(RootEntity));
	if(tmp != NULL)
	{
		memcpy(&tmp.peer, src_peer, sizeof(Peer));	
		tmp.sherlocks = sherlocks;
		tmp.metric = metric;
		if(node == NULL)
		{
			tmp->pNext = NULL;
		} 
		else
		{
			tmp->pNext = node-pNext;
			node->pNext = tmp;						
		}
	}	
	return tmp;	
}

RootEntity* rt_free(RootEntity* node)
{
	RootEntity* tmp = NULL;
	while(node != NULL)
	{
		tmp = node;
		node = node->pNext;		
		free(tmp);
	}
	return node;
}

void rt_sort(RootEntity* root, signed char (*f_comparator)()){
	RootEntity* new_root = NULL;
	
	if(f_comp == NULL)
		f_comparator = &rt_compare;

	while(root != NULL) 
	{
		RootEntity* node = root;
		root = root->pNext;
		if(new_root == NULL || f_comparator(node, new_root) > 0){
			node->next=new_root;
			new_root = node;
		} 
		else 
		{
			RootEntity* current = new_root;
			while(current->pNext != NULL && f_comparator(current->pNext, node) > 0)
			{
				current = current->pNext;
			}			
			node->pNext = current->pNext;
			current->pNext = node;
		}
	}
	return new_root;
}

void rt_display(RootEntity *node)
{
	print("UUID\t\t|IP:Port\t\t|sherloks\t|metric\n");
    for ( ; node != NULL; node = node->pNext ) 
    {
    	//todo print uuid and ip port
    	printf("|%d\t|%d;\n", node->sherlock, node->metric);
    }    	
}


#endif