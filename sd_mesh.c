#include "sd_mesh.h"

FAILED_NODE_REQUEST_FUNC_TYPE failedNodeRequestClbck = NULL;


void regFailedNodeRequestClbck(FAILED_NODE_REQUEST_FUNC_TYPE customClbck)
{
	failedNodeRequestClbck = customClbck;
}

//first initiating node
int node_bootstrap()
{
	
}
