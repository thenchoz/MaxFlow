#ifndef MAX_FLOW_LIB_HH
#define MAX_FLOW_LIB_HH

#include "export.hh"
#include "namespace.hh"
using namespace python_lib;

typedef Python::Graph* Graph;

extern "C"
{
    // compute
    Python* new_export() { return new Python(); }

    void delete_export(Python* e) { delete e; }

    double compute_max_flow(size_t, VArcptr, bool, Export*);


    // export result
    std::vector<Graph>* get_steps(Python* p) { return p->get_steps(); }

    size_t steps_size(std::vector<Graph>* v) { return v->size(); }

    Graph get_step(std::vector<Graph>* v, size_t i) { return v->at(i); }

    Graph get_final(Python* p) { return p->get_final(); }

    size_t get_nodes(Graph g) { return g->nb_nodes; }

    VArcptr get_capacity(Graph g) { return g->capacity; }

    VArcptr get_flow(Graph g) { return g->flow; }


    // vector related
    VArcptr new_vector() { return new std::vector<Arc*>; }

    void delete_vector(VArcptr v) { delete v; }

    size_t vector_size(VArcptr v) { return v->size(); }

    Arc* vector_get(VArcptr v, size_t i) { return v->at(i); }

    void vector_push(VArcptr v, Arc* a) { v->push_back(a); }


    // arcs related
    Arc* new_arc() { return new Arc; }

    void delete_arc(Arc* a) { delete a; }

    void arc_source(Arc* a, size_t i) { a->source = i; }

    void arc_target(Arc* a, size_t i) { a->target = i; }

    void arc_value(Arc* a, double i) { a->value = i; }

    size_t get_arc_source(Arc* a) { return a->source; }

    size_t get_arc_target(Arc* a) { return a->target; }

    double get_arc_value(Arc* a) { return a->value; }
}

#endif //MAX_FLOW_LIB_HH
