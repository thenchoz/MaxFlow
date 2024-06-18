#include "max_flow_lib.hh"
#include "export.hh"
#include "max_flow_initialisation.hh"

//====================================================================================================================//

extern "C" double compute_max_flow(
    size_t nb_nodes,
    VArcptr arcsptr,
    bool steps,
    Export* e
)
{
    std::vector<Arc> arcs;
    for (auto a : *arcsptr)
    {
        arcs.push_back(*a);
    }
    MaxFlowInitialisation mfi(nb_nodes, arcs, *e);
    return mfi.compute_max_flow(steps);
}
