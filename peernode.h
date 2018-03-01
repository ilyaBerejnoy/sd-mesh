#ifndef PEERNODE_H
#define PEERNODE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char IPV6_TYPE[16];
typedef unsigned char IPV4_TYPE[4];
typedef unsigned short PORT_TYPE;

typedef struct{
	IPV4_TYPE ip;
	PORT_TYPE port;
} Address;

typedef struct{
	IPV6_TYPE uuid;
	Address address1;
	Address address2;		
} Peer;

typedef struct __peer_node{
	Peer peer;
	PeerNode *pPrev;
	PeerNode *pNext;
} PeerNode;

Peer* new_peer(IPV6_TYPE uuid, IPV4_TYPE ip, PORT_TYPE port);
void free_peer(Peer* inst);
void generate_peer_uuid(IPV6_TYPE uuid);
int id_cmp(const IPV6_TYPE id1, const IPV6_TYPE id2);
PeerNode* pnt_insert(PeerNode* node, Peer* new_peer);
PeerNode* pnt_delete_node(PeerNode* node);
PeerNode* pnt_free(PeerNode* root);
void pnt_sort(PeerNode* root, signed char (*f_comparator)(PeerNode*, PeerNode*));
Peer* pnt_get_peer(PeerNode* root, IPV6_TYPE uuid);
void pnt_dump(PeerNode *node, unsigned int deep);

#ifdef __cplusplus
}
#endif

#endif PEERNODE_H