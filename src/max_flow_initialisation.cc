#include "max_flow_initialisation.hh"

//====================================================================================================================//

MaxFlowInitialisation::MaxFlowInitialisation(size_t n, VArc const& arcs, Export& e):
max_flow_(nullptr)
{
    // load graph
    Digraph* g = new Digraph;
    ArcMapDouble* map = new ArcMapDouble(*g);
    
    std::vector<Node> nodes;
    for(size_t i(0); i < n; i++)
    {
        nodes.push_back(g->addNode());
    }

    for (auto arc : arcs)
    {
        Digraph::Arc a(g->addArc(nodes[arc.source], nodes[arc.target]));
        (*map)[a] = arc.value;
    }
    
    max_flow_  = new MaxFlow(nodes[0], nodes[n-1], *g, *map, e);
}
