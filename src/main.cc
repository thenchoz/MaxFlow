#include <string>
#include "max_flow_initialisation.hh"
#include "export.hh"

/*
 * Expected arguments in order :
 * programm boolean int (int, int, int)*
 * first node is the source, last is the sink
 */
int main(int argc, char** argv)
{
  // get if all step
  std::string step(argv[1]);
  bool steps(step == "true");

  // create nodes
  size_t nb_nodes(std::stoi(argv[2]));

  std::vector<Arc> arcs;
  // add arcs
  for(int i(3); i < argc; i++)
  {
    // arc expected in the format (start node, end node, arc value)
    size_t pos(0);
    try
    {
      std::string values(argv[i]);
      size_t start(std::stoi(values, &pos));
      values = values.substr(pos+1);
      size_t end(std::stoi(values, &pos));
      values = values.substr(pos+1);
      double bandwidth(std::stod(values));

      arcs.push_back({start-1, end-1, bandwidth});
    }
    catch(std::invalid_argument const& ex)
    {
      std::cerr << "Wrong data format" << std::endl;
      return -1;
    }
  }

  Terminal t;

  // apply max flow algorythm
  MaxFlowInitialisation mfi(nb_nodes, arcs, t);
  std::cout << mfi.compute_max_flow(steps) << std::endl;
  
  return 0;
}
