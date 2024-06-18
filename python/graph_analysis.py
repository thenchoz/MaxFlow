'''
'''
import os
from glob import glob
import argparse

from utils import load_graph, draw_graph, compute_max_flow


if __name__ == '__main__':
    parser = argparse.ArgumentParser(add_help=True)
    parser.add_argument('--path', type=str, required=False, help="path to the graph folder", default='../graph/')
    parser.add_argument('-step', help="output every steps", action='store_true')
    parser.add_argument('-init', help="show initial graph", action='store_true')
    parser.add_argument('-save', help="save all graph plot", action='store_true')
    parser.add_argument('--output', type=str, required=False, help="output path for the image", default='../drawings')
    args = parser.parse_args()

    graph_paths = glob(os.path.join(args.path, '*.graphml'))

    for graph_path in graph_paths:
        name = os.path.splitext(os.path.split(graph_path)[1])[0]

        # 1. Load the graphs
        graph = load_graph(graph_path)

        # 1.b Show initial
        if args.init:
            draw_graph(
                graph=graph,
                to_save=args.save,
                filename=os.path.join(args.output,'init_'+name+'.jpg')
            )

        # 2. Call c++
        max_flow, final_graph, steps_graph = compute_max_flow(graph=graph, steps=args.step)
        print('The max flow is ', max_flow, '.', sep='')

        # 3. Output
        draw_graph(
            graph=final_graph,
            to_save=args.save,
            filename=os.path.join(args.output,'out_'+name+'.jpg')
        )

        if args.step:
            for i,g in enumerate(steps_graph):
                draw_graph(
                    graph=g,
                    to_save=args.save,
                    filename=os.path.join(args.output,'step'+str(i)+'_'+name+'.jpg')
                )
