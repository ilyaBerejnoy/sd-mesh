#ifndef SD_MESH_H
#define SD_MESH_H

typedef enum {WEAK_NODE_MODE=0, SUPER_NODE_MODE} NodeOperatingMode;  
typedef struct {
	NodeOperatingMode node_mode;
	unsigned int capacity_rate;
	time_t ts_registry;
} NodeConfig;

typedef void (*FAILED_NODE_REQUEST_FUNC_TYPE)(IPV6_TYPE* );
void regFailedNodeRequestClbck(FAILED_NODE_REQUEST_FUNC_TYPE customClbck);

//first initiating node procedure
int node_bootstrap() {

}

#endif
