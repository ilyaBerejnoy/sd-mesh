#include <stdlib.h>
#include <time.h>

#include "peerpeer.h"

static int pnt_compare(PeerNode* node1, PeerNode* node2);

Peer* new_peer(IPV6_TYPE uuid, IPV4_TYPE ip, PORT_TYPE port) {
	Peer* inst = malloc(sizeof(Peer));
	if(inst != NULL) {
		memset(inst, 0, sizeof(Peer));
		memcpy(inst->uuid, uuid, sizeof(IPV6_TYPE));
		memcpy(inst->address1.ip, ip, sizeof(IPV4_TYPE));
		memcpy(&inst->address1.port, port, sizeof(PORT_TYPE));
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

int id_cmp(const IPV6_TYPE id1, const IPV6_TYPE id2)
{
    return memcmp(id1, id2, sizeof(IPV6_TYPE));
}

int pnt_compare(PeerNode* node1, PeerNode* node2) {
	if(node1 != NULL && node2 != NULL)
	{
		return id_cmp(node1->peer.uuid,node2->peer.uuid);
	} 
	else 
	{
		return (node1 >= node2) ? 1 : -1;
	}
}

PeerNode* pnt_insert(PeerNode* root, Peer* new_peer) 
{
	PeerNode* tmp = malloc(sizeof(PeerNode));
	if(tmp != NULL)
	{
		memcpy(&tmp.peer, new_peer, sizeof(Peer));	
		if(root == NULL)
		{
			tmp->pNext = NULL;
			tmp->pPrev = NULL;
		} 
		else
		{
			int cmp_res = id_cmp(root->peer.uuid, tmp->peer.uuid);
			if(cmp_res > 0)
			{
				while(root->pNext != NULL && cmp_res > 0)
				{
					root = root->pNext;					
					cmp_res = id_cmp(root->peer.uuid, tmp->peer.uuid);
				}							
			} 
			else if(cmp_res < 0)
			{
				while(root->pPrev != NULL && cmp_res < 0)
				{
					root = root->pPrev;		
					cmp_res = id_cmp(root->peer.uuid, tmp->peer.uuid);
				}					
			}

			if(cmp_res > 0)
			{
				tmp->pNext = root->pNext;
				tmp->pPrev = root;
				if(root->pNext != NULL)
					node->pNext->pPrev = tmp;
				root->pNext = tmp;					
			}
			else if(cmp_res < 0)
			{
				tmp->pNext = root;
				tmp->pPrev = root->pPrev;
				if(root->pPrev != NULL)
					root->pPrev->pNext = tmp;					
				root->pPrev = tmp;					
			}
			else
			{
				memcpy(root->peer.address1, tmp->peer.address1, sizeof(Address));
				memcpy(root->peer.address2, tmp->peer.address2, sizeof(Address));
			}			
		}
	}	
	return tmp;	
}

PeerNode* pnt_delete_node(PeerNode* node)
{
	PeerNode* tmp = NULL;
	if(node != NULL)
	{
		PeerNode* nodePrev = node->pPrev;
		PeerNode* nodeNext = node->pNext;
		free(node);
		if(nodePrev != NULL)
		{
			nodePrev->pNext = nodeNext;
			tmp = nodePrev;
		}
		
		if(nodeNext != NULL)
			nodeNext->pPrev = nodePrev;
			if(tmp == NULL)
				tmp = nodeNext;	
		}				
	}
	return tmp;
}

PeerNode* pnt_free(PeerNode* root)
{
	if(root == NULL)	return NULL;
	
	while(root->pPrev != NULL)
		root = root->pPrev;

	PeerNode* tmp = NULL;
	while(root != NULL) {
		tmp = root;
		root = root->pNext;
		free(tmp);
	}
	return root;
}

void pnt_sort(PeerNode* root, int (*f_comparator)(PeerNode* node, Peer* new_peer)){
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

Peer* pnt_get_peer(PeerNode* root, const IPV6_TYPE uuid)
{
	Peer* peer = NULL;
	int cmp_res = 0;	
	//todo optimize searching
	while(root != NULL) 
	{
		cmp_res = id_cmp(root->uuid, uuid);
		if(cmp_res == 0)
		{
			peer = &root->peer;
			break;
		} 
		else if(cmp_res < 0)
		{
			break;
		}
		root = root->pNext;
	}
	return peer;
}

void pnt_dump(PeerNode *node, unsigned int deep)
{
	print("UUID\t\t|IP:Port1\t\t|IP:Port2\n");
    for ( ; node != NULL && deep > 0; node = node->pNext, deep-- ) 
    {
    	//todo print uuid and ip port
    	
    }    	
}