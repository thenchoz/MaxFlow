#include "export.hh"
#include "max-flow.hh"
#include "max_flow_initialisation.hh"

//====================================================================================================================//

Python::~Python()
{
    for (auto s : *steps_)
    {
        for (auto a : *(s->capacity))
        {
            delete a;
        }
        delete s->capacity;
        for (auto a : *(s->flow))
        {
            delete a;
        }
        delete s->flow;
        
        delete s;
    }

    delete steps_;

    if (final_ != nullptr)
    {
        for (auto a : *(final_->capacity))
        {
            delete a;
        }
        delete final_->capacity;
        for (auto a : *(final_->flow))
        {
            delete a;
        }
        delete final_->flow;
        delete final_;
    }
}

//====================================================================================================================//

void Python::export_step(FilterNodes const& an, SubMap const& c, ArcMapDouble const& f)
{
    Graph* g = new Graph;
    g->nb_nodes = lemon::countNodes(an);

    g->capacity = new std::vector<Arc*>;
    g->flow = new std::vector<Arc*>;

    for (auto arc : an.arcs())
    {
        g->capacity->push_back(new Arc(an.id(an.source(arc)), an.id(an.target(arc)), c[arc]+f[arc]));
        g->flow->push_back(new Arc(an.id(an.source(arc)), an.id(an.target(arc)), f[arc]));
    }
    
    steps_->push_back(g);
}

//====================================================================================================================//

void Python::export_final(Digraph const& g, ArcMapDouble const& c, ArcMapDouble const& f)
{
    final_->nb_nodes = lemon::countNodes(g);
    
    for (auto arc : g.arcs())
    {
        final_->capacity->push_back(new Arc(g.id(g.source(arc)), g.id(g.target(arc)), c[arc]));
        final_->flow->push_back(new Arc(g.id(g.source(arc)), g.id(g.target(arc)), f[arc]));
    }
}

//====================================================================================================================//

void Terminal::export_step(FilterNodes const& an, SubMap const& c, ArcMapDouble const& f)
{
    std::cout << std::endl
              << "-------------------------------------------------------------"
              << std::endl << std::endl
              << "Auxiliary graph compose of :" << std::endl
              << " Nodes :" << std::endl;
    for (auto node : an.nodes())
    {
        std::cout << "  - " << an.id(node) << std::endl;
    }
    std::cout << " Arcs :" << std::endl;
    for (auto arc : an.arcs())
    {
        std::cout << "  - (" << an.id(an.source(arc)) << ","
                  << an.id(an.target(arc)) << "), c=" << c[arc]
                  << ", f=" << f[arc] << std::endl;
    }
}

//====================================================================================================================//

void Terminal::export_final(Digraph const& g, ArcMapDouble const& c, ArcMapDouble const& f)
{
    std::cout << std::endl
              << "-------------------------------------------------------------"
              << std::endl << std::endl
              << "Final graph compose of :" << std::endl
              << " Nodes :" << std::endl;
    for (auto node : g.nodes())
    {
        std::cout << "  - " << g.id(node) << std::endl;
    }
    std::cout << " Arcs :" << std::endl;
    for (auto arc : g.arcs())
    {
        std::cout << "  - (" << g.id(g.source(arc)) << ","
                  << g.id(g.target(arc)) << "), c=" << c[arc]
                  << ", f=" << f[arc] << std::endl;
    }
}
