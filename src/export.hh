#ifndef EXPORT_HH
#define EXPORT_HH

#include "namespace.hh"

using namespace lemon_spec;
using namespace python_lib;

class Export
{
public:
    Export() {}
    virtual ~Export() {}

    virtual void export_step(FilterNodes const&, SubMap const&, ArcMapDouble const&) = 0;
    virtual void export_final(Digraph const&, ArcMapDouble const&, ArcMapDouble const&) = 0;
};

class Python : public Export
{
public:
    struct Graph
    {
        size_t nb_nodes;
        VArcptr capacity;
        VArcptr flow;
    };
    
    Python():
    steps_(new std::vector<Graph*>), final_(new Graph)
    {
        final_->capacity = new std::vector<Arc*>;
        final_->flow = new std::vector<Arc*>;
    }
    virtual ~Python();

    virtual void export_step(FilterNodes const&, SubMap const&, ArcMapDouble const&) override;
    virtual void export_final(Digraph const&, ArcMapDouble const&, ArcMapDouble const&) override;

    std::vector<Graph*>* get_steps() const { return steps_; }
    Graph* get_final() const { return final_; }

private:
    std::vector<Graph*>* steps_;
    Graph* final_;
};

class Terminal : public Export
{
public:
    Terminal() {}
    virtual ~Terminal() {}

    virtual void export_step(FilterNodes const&, SubMap const&, ArcMapDouble const&) override;
    virtual void export_final(Digraph const&, ArcMapDouble const&, ArcMapDouble const&) override;
};

#endif //EXPORT_HH
