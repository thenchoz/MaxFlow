'''
This file provides useful functions to use c++ library.
'''

from ctypes import c_bool, c_double, c_size_t, c_void_p, cdll
from typing import List

import networkx as nx
#from utils import EDGE_LABEL
EDGE_LABEL = 'capacity'

LIB_PATH = '../build/bin/libMax-Flow-lib.so'

class BaseArc:
    """Python equivalent to the c++ Arc class
    """
    lib = cdll.LoadLibrary(LIB_PATH)
    lib.arc_source.restype = None
    lib.arc_source.argtypes = [c_void_p, c_size_t]
    lib.arc_target.restype = None
    lib.arc_target.argtypes = [c_void_p, c_size_t]
    lib.arc_value.restype = None
    lib.arc_value.argtypes = [c_void_p, c_double]
    lib.get_arc_source.restype = c_size_t
    lib.get_arc_source.argtypes = [c_void_p]
    lib.get_arc_target.restype = c_size_t
    lib.get_arc_target.argtypes = [c_void_p]
    lib.get_arc_value.restype = c_double
    lib.get_arc_value.argtypes = [c_void_p]

    def __init__(self, ptr: c_void_p):
        self.arc = c_void_p(ptr)

    def __str__(self) -> str:
        msg = '('
        msg += str(self.get_source())
        msg += ', '
        msg += str(self.get_target())
        msg += ', '
        msg += str(self.get_value())
        msg += ')'
        return msg

    def source(self, value: int) -> None:
        """Adds source value in arc class.

        Args:
            value (int): id of the source node
        """
        BaseArc.lib.arc_source(self.arc, c_size_t(value))

    def target(self, value: int) -> None:
        """Adds target value in arc class.

        Args:
            value (int): id of the target node
        """
        BaseArc.lib.arc_target(self.arc, c_size_t(value))

    def value(self, value: float) -> None:
        """Adds value value in arc class.

        Args:
            value (float): weigth of an arc
        """
        BaseArc.lib.arc_value(self.arc, c_double(value))

    def get_source(self) -> int:
        """Gets the source value from the arc obj.

        Returns:
            int: Source value
        """
        return BaseArc.lib.get_arc_source(self.arc)

    def get_target(self) -> int:
        """Gets the target value from the arc obj.

        Returns:
            int: Target value
        """
        return BaseArc.lib.get_arc_target(self.arc)

    def get_value(self) -> float:
        """Gets the value value from the arc obj.

        Returns:
            float: value value
        """
        return BaseArc.lib.get_arc_value(self.arc)


class Arc(BaseArc):
    """Python equivalent to the c++ Arc class
    """
    lib = cdll.LoadLibrary(LIB_PATH)
    lib.new_arc.restype = c_void_p
    lib.new_arc.argtypes = []
    lib.delete_arc.restype = None
    lib.delete_arc.argtypes = [c_void_p]

    def __init__(self, source: int, target: int, value: int):
        super().__init__(Arc.lib.new_arc())
        self.source(source)
        self.target(target)
        self.value(value)

    #def __del__(self):
    #    Arc.lib.delete_arc(self.arc)


class VectorArc:
    """Python equivalent to the c++ vector class
    """
    lib = cdll.LoadLibrary(LIB_PATH)
    lib.new_vector.restype = c_void_p
    lib.new_vector.argtypes = []
    lib.delete_vector.restype = None
    lib.delete_vector.argtypes = [c_void_p]
    lib.vector_size.restype = c_size_t
    lib.vector_size.argtypes = [c_void_p]
    lib.vector_get.restype = c_void_p
    lib.vector_get.argtypes = [c_void_p, c_size_t]
    lib.vector_push.restype = None
    lib.vector_push.argtypes = [c_void_p, c_void_p]

    def __init__(self):
        self.vector = VectorArc.lib.new_vector()

    def __del__(self):
        for i in self:
            Arc.lib.delete_arc(i)
        VectorArc.lib.delete_vector(self.vector)

    def __len__(self) -> int:
        return VectorArc.lib.vector_size(self.vector)

    def __getitem__(self, i) -> c_void_p:
        if 0 <= i < len(self):
            return VectorArc.lib.vector_get(self.vector, c_size_t(i))
        raise IndexError('Vector index out of range')

    def push(self, arc: Arc) -> None:
        """Adds an arc in c++ vector.

        Args:
            arc (Arc): Arc to add in vector
        """
        VectorArc.lib.vector_push(self.vector, arc.arc)


class VectorGraph:
    """Python equivalent to the c++ vector class
    """
    lib = cdll.LoadLibrary(LIB_PATH)
    lib.vector_size.restype = c_size_t
    lib.vector_size.argtypes = [c_void_p]
    lib.vector_get.restype = c_void_p
    lib.vector_get.argtypes = [c_void_p, c_size_t]

    def __init__(self, ptr_c: c_void_p, ptr_f: c_void_p):
        self.vector_capacity = c_void_p(ptr_c)
        self.vector_flow = c_void_p(ptr_f)

    def __len_capacity__(self) -> int:
        return VectorGraph.lib.vector_size(self.vector_capacity)

    def __len_flow__(self) -> int:
        return VectorGraph.lib.vector_size(self.vector_flow)

    def __getitem_capacity__(self, i) -> c_void_p:
        if 0 <= i < self.__len_capacity__():
            return VectorGraph.lib.vector_get(self.vector_capacity, c_size_t(i))
        raise IndexError('Vector index out of range')

    def __getitem_flow__(self, i) -> c_void_p:
        if 0 <= i < self.__len_flow__():
            return VectorGraph.lib.vector_get(self.vector_flow, c_size_t(i))
        raise IndexError('Vector index out of range')

    def get_arc(self) -> dict:
        """Converts all arc into dict

        Returns:
            dict: all arc with key (source, target)
        """
        arcs = {}
        for i in range(self.__len_capacity__()):
            arc = BaseArc(self.__getitem_capacity__(i))
            arcs[(arc.get_source(), arc.get_target())] = {EDGE_LABEL: arc.get_value()}
        for i in range(self.__len_flow__()):
            arc = BaseArc(self.__getitem_flow__(i))
            arcs[(arc.get_source(), arc.get_target())]['flow'] = arc.get_value()
        return arcs


class Export:
    """Python equivalent to the c++ Export class
    """
    lib = cdll.LoadLibrary(LIB_PATH)
    lib.new_export.restype = c_void_p # Python*
    lib.new_export.argtypes = []
    lib.delete_export.restype = None
    lib.delete_export.argtypes = [c_void_p] # Python*

    lib.get_steps.restype = c_void_p # Python*
    lib.get_steps.argtypes = [c_void_p] # Python*
    lib.get_step.restype = c_void_p # Graph*
    lib.get_step.argtypes = [c_void_p, c_size_t] # vector<Graph*>*, size_t
    lib.steps_size.restype = c_size_t # size_t
    lib.steps_size.argtypes = [c_void_p] # vector<Graph*>*
    lib.get_final.restype = c_void_p # Graph*
    lib.get_final.argtypes = [c_void_p] # Python*

    lib.get_nodes.restype = c_size_t # size_t
    lib.get_nodes.argtypes = [c_void_p] # Graph*
    lib.get_capacity.restype = c_void_p # VArcptr
    lib.get_capacity.argtypes = [c_void_p] # Graph*
    lib.get_flow.restype = c_void_p # VArcptr
    lib.get_flow.argtypes = [c_void_p] # Graph*

    def __init__(self):
        self.export = Export.lib.new_export()
        self.steps = Export.lib.get_steps(self.export)
        self.final = Export.lib.get_final(self.export)
        self.nb_node = 0

    def __del__(self):
        Export.lib.delete_export(self.export)

    def __len__(self) -> c_size_t:
        return Export.lib.steps_size(self.steps)

    def __getitem__(self, i: c_size_t) -> c_void_p:
        if 0 <= i < len(self):
            return Export.lib.get_step(self.steps, c_size_t(i))
        raise IndexError('Vector index out of range')

    def get_final(self) -> nx.Graph:
        """Gets the final state as a nx.Graph

        Returns:
            nx.Graph: Final state after the algorythm
        """
        self.nb_node = Export.lib.get_nodes(c_void_p(self.final))
        return self.__get_graph__(self.final)

    def get_steps(self) -> List[nx.Graph]:
        """Gets all intermediate step to solve to max-flow

        Returns:
            List[nx.Graph]: List of all step as nx.Graph
        """
        return [self.__get_graph__(ptr) for ptr in self]

    def __get_graph__(self, ptr: c_void_p) -> nx.Graph:
        arcs = VectorGraph(
            Export.lib.get_capacity(c_void_p(ptr)),
            Export.lib.get_flow(c_void_p(ptr))
        )

        graph = nx.DiGraph()

        for ((source, target), value) in arcs.get_arc().items():
            s,t = source, target
            if source == 0:
                s = 's'
            else:
                s = 'v' + str(source)
            if target == self.nb_node - 1:
                t = 't'
            else:
                t = 'v' + str(target)
            graph.add_edge(s, t, capacity=value.get(EDGE_LABEL), flow=value.get('flow'))
        return graph


class MaxFlowInitialisation:
    """Python equivalent to the c++ MaxFlowInitialisation class
    """
    lib = cdll.LoadLibrary(LIB_PATH)
    lib.compute_max_flow.restype = c_double
    lib.compute_max_flow.argtypes = [c_size_t, c_void_p, c_bool, c_void_p]

    def __init__(self, graph: nx.Graph, steps: bool, export: Export):
        self.export = export
        self.vector = VectorArc()
        self.nb_nodes = 0
        self.steps = steps

        self.__add_nodes__(graph)
        self.__add_arcs__(graph)

    def __add_nodes__(self, graph: nx.Graph):
        self.nb_nodes = len(graph.nodes())

    def __add_arcs__(self, graph: nx.Graph):
        pos = {
            name: position+1 for (position, name) in
            enumerate(filter(lambda n: (n!='s') &( n!='t'), graph.nodes()))
        }
        pos['s'] = 0
        pos['t'] = len(graph.nodes())-1
        for (source, target, weight) in graph.edges(data=True):
            a = Arc(pos[source], pos[target], weight['capacity'])
            self.vector.push(a)

    def compute_max_flow(self) -> int:
        """Computes the max flow on a given graph.

        Returns:
            int: max_flow calculation value
        """
        return MaxFlowInitialisation.lib.compute_max_flow(
            c_size_t(self.nb_nodes),
            c_void_p(self.vector.vector),
            c_bool(self.steps),
            c_void_p(self.export.export)
        )
