#include "max-flow.hh"
#include <limits>
#include <map>
#include <vector>
#include "export.hh"

//====================================================================================================================//

double MaxFlow::compute_max_flow(bool exportStep)
{
    double f(0);
    bool not_done(true);

    while (not_done)
    {
        // compute auxiliary network
        not_done = construct_auxiliary_network();

        if (not_done)
        {
            std::vector<Node> throughput_null;
            Node min_node;

            // compute min throughtput and node with throughtput == 0
            double min_throughtput(throughputs(throughput_null, min_node));

            while (!throughput_null.empty())
            {
                Node current(throughput_null.back());

                if (current == source_ || current == sink_)
                {
                    throughput_null.pop_back();
                }
                else
                {
                    // remove connected arc
                    for (auto arc : auxiliary_network_.outArcs(current))
                    {
                        aux_network_.disable(arc);
                    }
            
                    for (auto arc : auxiliary_network_.inArcs(current))
                    {
                        aux_network_.disable(arc);
                    }

                    // remove node
                    auxiliary_network_.disable(current);

                    throughput_null.clear();
                    min_throughtput = throughputs(throughput_null, min_node);
                }
            }

            if (min_throughtput != std::numeric_limits<double>::max())
            {
                push(min_node, min_throughtput);
                pull(min_node, min_throughtput);
                
                f += min_throughtput;
            }
            
            if (exportStep)
            {
                // export this step
                export_->export_step(auxiliary_network_, capacity_left_, flow_);
            }
        }
    }
    
    export_->export_final(graph_, arc_capacity_, flow_);

    return f;
}

//====================================================================================================================//

bool MaxFlow::construct_auxiliary_network()
{
    // reinit auxiliary networks
    for (auto arc : graph_.arcs())
    {
        if (capacity_left_[arc] == 0)
        {
            enabled_arcs_[arc] = false;
        }
        else
        {
            enabled_arcs_[arc] = true;
        }
    }
    for (auto node : graph_.nodes())
    {
        enabled_nodes_[node] = true;
    }
    
    // remove arc and node
    FilterNodes::NodeMap<size_t> label(auxiliary_network_, std::numeric_limits<size_t>::max());
    labeling(label);

    for (auto node : auxiliary_network_.nodes())
    {
        if (label[node] >= label[sink_] && node != sink_)
        {
            // remove connected arc
            for (auto a : auxiliary_network_.outArcs(node))
            {
                aux_network_.disable(a);
            }
            
            for (auto a : auxiliary_network_.inArcs(node))
            {
                aux_network_.disable(a);
            }

            // remove node
            auxiliary_network_.disable(node);
        }
        
    }

    for (auto arc : auxiliary_network_.arcs())
    {
        if (label[auxiliary_network_.target(arc)] - label[auxiliary_network_.source(arc)] != 1)
        {
            aux_network_.disable(arc);
        }
        
    }

    return label[sink_] != std::numeric_limits<size_t>::max();
}

//====================================================================================================================//

void MaxFlow::push(Node const& n, double value)
{
    FilterNodes::NodeMap<double> req(auxiliary_network_, 0);
    req[n] = value;

    std::vector<Node> queue({n});

    while (!queue.empty())
    {
        Node current(queue[0]);
        queue.erase(queue.begin());

        for (auto arc : auxiliary_network_.outArcs(current))
        {
            int m(std::min(req[current], capacity_left_[arc]));
            flow_[arc] += m;
            req[current] -= m;
            req[auxiliary_network_.target(arc)] += m;
            queue.push_back(auxiliary_network_.target(arc));
        }
    }
}

//====================================================================================================================//

void MaxFlow::pull(Node const& n, double value)
{
    FilterNodes::NodeMap<double> req(auxiliary_network_, 0);
    req[n] = value;

    std::vector<Node> queue({n});

    while (!queue.empty())
    {
        Node current(queue[0]);
        queue.erase(queue.begin());

        for (auto arc : auxiliary_network_.inArcs(current))
        {
            int m(std::min(req[current], capacity_left_[arc]));
            flow_[arc] += m;
            req[current] -= m;
            req[auxiliary_network_.source(arc)] += m;
            queue.push_back(auxiliary_network_.source(arc));
        }
    }
}

//====================================================================================================================//

void MaxFlow::labeling(FilterNodes::NodeMap<size_t>& label) const
{
    std::vector<Node> queue({source_});
    label[source_] = 0;

    while (!queue.empty())
    {
        Node current(queue[0]);
        queue.erase(queue.begin());

        size_t level = label[current] + 1;

        for (auto a : auxiliary_network_.outArcs(current))
        {
            if (label[auxiliary_network_.target(a)] == std::numeric_limits<size_t>::max())
            {
                label[auxiliary_network_.target(a)] = level;
                queue.push_back(auxiliary_network_.target(a));
            }
        }
    }
}

//====================================================================================================================//

double MaxFlow::throughputs(std::vector<Node>& v, Node& n) const
{
    double min_throughput(std::numeric_limits<double>::max());

    for (auto node : auxiliary_network_.nodes())
    {
        int t(throughput(node));
        
        if (t == 0.0)
        {
            v.push_back(node);
        }
        else if (t < min_throughput)
        {
            min_throughput = t;
            n = node;
        }
    }

    return min_throughput;
}

//====================================================================================================================//

double MaxFlow::throughput(Node const& n) const
{
    double t_out(0);
    double t_in(0);

    for (auto arc : auxiliary_network_.outArcs(n))
    {
        t_out += capacity_left_[arc];
    }
    
    for (auto arc : auxiliary_network_.inArcs(n))
    {
        t_in += capacity_left_[arc];
    }

    if (n == source_)
    {
        t_in = t_out;
    }
    else if (n == sink_)
    {
        t_out = t_in;
    }
    
    return std::min(t_out, t_in);
}
