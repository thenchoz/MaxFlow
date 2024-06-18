'''
This file provides useful functions to perform the max flow algorythm.
'''

from typing import List
import networkx as nx
from matplotlib import pyplot as plt

from max_flow_cc import MaxFlowInitialisation, Export

###############
#  CONSTANTS  #
###############

EDGE_LABEL = 'capacity'
NODES = ('s', 't')


###############
#   UTILS     #
###############

def load_graph(filename: str) -> nx.DiGraph:
    """
    Load the **file.graphml** as a **nx.DiGraph**.

    Args:
        filename (str): The path to the graph.

    Returns:
        nx.DiGraph: The loaded NetworkX graph.
    """
    assert filename.split('.')[-1] == 'graphml', f'Filename not valid: {filename}'

    graph = nx.read_graphml(filename, node_type=str)

    return graph


def draw_graph(
        graph: nx.DiGraph,
        to_save: bool = False,
        filename: str = None,
        flow: bool = False
) -> None:
    """
    This function draws a given networkx graph object.
    If specified, saves the drawing to a specified file.

    Args:
        graph (nx.Graph): A networkx graph object.
        to_save (bool, optional): If the image will be saved. Defaults to False.
        filename (str, optional): A string representing the path and filename
                  where the graph will be saved as an image. Defaults to None.
        flow (bool, optional): Add the flow value. Defaults to False.
    """

    pos = nx.bfs_layout(graph, start='s', scale=10)

    s_t_node = [n for n in graph.nodes() if n in NODES]
    other_nodes = [n for n in graph.nodes() if n not in NODES]

    if flow:
        edge_colors = [(0,0,v['flow']/v['capacity']) for _,_,v in graph.edges(data=True)]
        edge_labels = {
            (s,t): 'capacity=' + str(v['capacity']) + '\nflow=' + str(v['flow'])
            for s,t,v in graph.edges(data=True)
        }
    else:
        edge_colors = (0,0,0)
        edge_labels = nx.get_edge_attributes(graph, EDGE_LABEL)

    # nodes
    nx.draw_networkx_nodes(graph, pos=pos, nodelist=s_t_node, node_color='Red', node_size=250)
    nx.draw_networkx_nodes(graph, pos=pos, nodelist=other_nodes, node_size=250)
    # edges
    nx.draw_networkx_edges(
        graph,
        pos=pos,
        edge_color=edge_colors,
        width=2,
        node_size=250
    )
    # nodes label
    nx.draw_networkx_labels(graph, pos=pos, font_size=5, font_family="sans-serif")
    # edges weight label
    nx.draw_networkx_edge_labels(graph, pos=pos, edge_labels=edge_labels, font_size=5)

    ax = plt.gca()
    ax.margins(0.08)
    plt.axis("off")
    plt.tight_layout()

    if to_save:
        ax.savefig(filename)
    else :
        plt.show()


def compute_max_flow(graph: nx.Graph, steps: bool = False) -> int | nx.DiGraph | List[nx.DiGraph]:
    """_summary_

    Args:
        graph (nx.Graph): _description_
        steps (bool, optional): _description_. Defaults to False.

    Returns:
        int | nx.DiGraph | List[nx.DiGraph]: _description_
    """
    export = Export()
    mfi = MaxFlowInitialisation(graph=graph, steps=steps, export=export)

    max_flow = mfi.compute_max_flow()

    return max_flow, export.get_final(), export.get_steps()
