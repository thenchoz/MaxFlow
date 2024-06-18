#ifndef MAX_FLOW_HH
#define MAX_FLOW_HH

#include "namespace.hh"

using namespace lemon_spec;

class Export;


class MaxFlow
{
public :
    MaxFlow(
        Node s,
        Node t,
        Digraph& g,
        ArcMapDouble& am,
        Export& e
    ):
    source_(s),
    sink_(t),
    graph_(g),
    arc_capacity_(am),
    flow_(ArcMapDouble(g, 0.0)),
    capacity_left_(arc_capacity_, flow_),
    enabled_arcs_(graph_, true),
    aux_network_(graph_, enabled_arcs_),
    enabled_nodes_(aux_network_, true),
    auxiliary_network_(aux_network_, enabled_nodes_),
    export_(&e)
    {}
    ~MaxFlow() {}

    double compute_max_flow(bool exportStep = false);

private:
    bool construct_auxiliary_network();
    void push(Node const&, double);
    void pull(Node const&, double);

    void labeling(FilterNodes::NodeMap<size_t>&) const;
    double throughputs(std::vector<Node>&, Node&) const;
    double throughput(Node const&) const;

private:
    const Node source_;
    const Node sink_;
    Digraph& graph_;
    const ArcMapDouble& arc_capacity_;

    ArcMapDouble flow_;
    SubMap capacity_left_;

    ArcMapBool enabled_arcs_;
    FilterArcs aux_network_;

    NodeMap enabled_nodes_;
    FilterNodes auxiliary_network_;

    Export* export_;
};

#endif //MAX_FLOW_HH
