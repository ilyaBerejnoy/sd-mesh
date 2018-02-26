#ifndef PEERNODE_H
#define PEERNODE_H

#include <stdlib.h>
#include <time.h>


typedef unsigned char[16] IPV6_TYPE;
typedef unsigned char[4] IPV4_TYPE;
typedef unsigned short PORT_TYPE;

typedef struct{
	IPV4_TYPE ip;
	PORT_TYPE port
} Address;

typedef struct{
	IPV6_TYPE uuid;
	Address address1;
	Address address2;
} Peer;

typedef struct{
	Peer node;
	Peer *pPrev
	Peer *pNext;
} PeerNode;

Peer* new_peer() {
	Peer* inst = malloc(sizeof(Peer));
	if(inst != NULL) {
		memset(inst, 0, sizeof(Peer));
	}
	return inst;
}

void free_peer(Peer* inst) {
	free(inst);
	inst = NULL;
}

void generate_peer_uuid(IPV6_TYPE uuid) {
	time_t t;
	srand ((unsigned)time(&t));
	int *pid = (int*) uuid;
	for (char i = 0;i < sizeof(IPV6_TYPE)/sizeof(int); i++){
		*pid++ = rand();
	}
	uuid[6] = 0x40 | (uuid[6] & 0xf);
	uuid[8] = 0x80 | (uuid[8] & 0x3f);
}

signed char pnt_compare(Peer* node1, Peer* node2) {
	if(node1 != NULL && node2 != NULL)
	{
		return memcmp(node1, node2, sizeof(Peer));
	} 
	else 
	{
		return (node1 >= node2) ? 1 : -1;
	}
}

PeerNode* pnt_insert(PeerNode* node, Peer* new_peer) 
{
	Peer* tmp = malloc(sizeof(Peer));
	if(tmp != NULL)
	{
		memcpy(&tmp.peer, new_peer, sizeof(Peer));	
		if(node == NULL)
		{
			tmp->pNext = NULL;
			tmp->pPrev = NULL;
		} 
		else
		{
			tmp->pNext = node->pNext;
			tmp->pPrev = node;
			if(node->pNext != NULL)
				node->pNext->pPrev = tmp;
			node->pNext = tmp;

		}
	}	
	return tmp;	
}

PeerNode* pnt_free(PeerNode* node)
{
	PeerNode* tmp = NULL;
	if(node != NULL)
	{
		PeerNode* nodePrev = node->pPrev;
		PeerNode* nodeNext = node->pNext;
		free(tmp);
		if(nodePrev != NULL)
			nodePrev->pNext = nodeNext;
		else 
			tmp = nodeNext;
		if(nodeNext != NULL)
			nodeNext->pPrev = nodePrev;	
		}				
	}
	return tmp;
}

void pnt_sort(PeerNode* root, signed char (*f_comparator)()){
	PeerNode* new_root = NULL;
	
	if(f_comp == NULL)
		f_comparator = &pnt_compare;

	while(root != NULL) 
	{
		PeerNode* node = root;
		root = root->pNext;
		if(new_root == NULL || f_comparator(node, new_root) > 0){
			node->next=new_root;
			new_root = node;
		} 
		else 
		{
			PeerNode* current = new_root;
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

void pnt_display(PeerNode *node)
{
	print("UUID\t\t|IP:Port\t\t|sherloks\t|metric\n");
    for ( ; node != NULL; node = node->pNext ) 
    {
    	//todo print uuid and ip port
    	printf("|%d\t|%d;\n", node->sherlock, node->metric);
    }    	
}

#endif PEERNODE_H