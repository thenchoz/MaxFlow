#ifndef NAMESPACE_HH
#define NAMESPACE_HH

#include <lemon/adaptors.h>
#include <lemon/list_graph.h>
#include <lemon/maps.h>
#include <lemon/concepts/digraph.h>
#include <lemon/core.h>
#include <vector>

namespace lemon_spec
{
    typedef lemon::ListDigraph Digraph;
    typedef lemon::ListDigraph::ArcMap<double> ArcMapDouble;
    typedef lemon::ListDigraph::ArcMap<bool> ArcMapBool;
    typedef lemon::ListDigraph::Node Node;
    typedef lemon::SubMap<ArcMapDouble, ArcMapDouble> SubMap;
    typedef lemon::FilterArcs<Digraph, ArcMapBool> FilterArcs;
    typedef FilterArcs::NodeMap<bool> NodeMap;
    typedef lemon::FilterNodes<FilterArcs, NodeMap> FilterNodes;
} // namespace lemon_spec


namespace python_lib
{
    struct Arc
    {
        Arc(size_t s=0, size_t t=0, double v=0):
        source(s), target(t), value(v) {}

        size_t source;
        size_t target;
        double value;
    };

    typedef std::vector<Arc> VArc;
    typedef std::vector<Arc*>* VArcptr;
} // namespace python_lib


#endif //NAMESPACE_HH
