#ifndef MAX_FLOW_INIT_HH
#define MAX_FLOW_INIT_HH

#include "max-flow.hh"

using namespace python_lib;


class MaxFlowInitialisation
{
public:
    MaxFlowInitialisation(size_t, VArc const&, Export&);
    ~MaxFlowInitialisation() { delete max_flow_; }

    inline double compute_max_flow(bool b = false) const { return max_flow_->compute_max_flow(b); }

private:
    MaxFlow* max_flow_;
};

#endif //MAX_FLOW_LIB_HH
