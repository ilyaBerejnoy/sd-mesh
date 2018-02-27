#include <stdlib.h>
#include <time.h>

#include "rooting.h"

int rt_compare(RootEntity* node1, RootEntity* node2);

int rt_compare(RootEntity* node1, RootEntity* node2) {
	int score = 0;
	if(node1 != NULL && node2 != NULL)
	{
		do {
			score = node2->overloaded - node1->overloaded;
			if(score != 0)	break;
			if(node1->hits_per100 == 0 && node2->hits_per100 == 0)
			{
				score = ((node1->shared > node2->shared) ? 1 : ((node1->shared < node2->shared) ? -1 : 0));
				break;
			}

			score = (node1->shared*node1->hits_per100 - node2->shared*node2->hits_per100) >= 0 ? 1 : -1
						
		} while(false);
		return score;
		// return ( (node1->path_get - node2->path_get) / (node1->path_get + node2->path_get) + 
		// 	+ (node2->metric - node1->metric) / (node2->metric + node1->metric) ) > 0.f ? 1 : -1;
	} 
	else 
	{
		return (node1 >= node2) ? 1 : -1;
	}
}

RootEntity* rt_insert(RootEntity* node, Peer* src_peer) 
{
	RootEntity* tmp = malloc(sizeof(RootEntity));
	if(tmp != NULL)
	{
		memcpy(&tmp.peer, src_peer, sizeof(Peer));	
		tmp->hits_per100 = 100;
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

void rt_sort(RootEntity* root, int (*f_comparator)(RootingEntity*, RootingEntity*)){
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

RootingEntity* rt_get_node(RootingEntity* root, IPV6_TYPE uuid)
{
	RootingEntity* rt_ent = NULL;
	int cmp_res = 0;	
	//todo optimize searching
	while(root != NULL) 
	{
		cmp_res = id_cmp(root->peer.uuid, uuid);
		if(cmp_res == 0)
		{
			rt_ent = root;
			break;
		} 
		else if(cmp_res < 0)
		{
			break;
		}
		root = root->pNext;
	}
	return rt_ent;	
}

void rt_dump(RootEntity *node, unsigned int deep)
{
	print("UUID\t\t|IP:Port\t\t|path_get\t|path_req\n");
    for ( ; node != NULL && deep > 0; node = node->pNext, deep-- ) 
    {
    	//todo print uuid and ip port
    	printf("|%d\t|%d;\n", node->path_get, node->path_req);
    }    	
}
