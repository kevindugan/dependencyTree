# Structure which holds a list of unique nodes for dependency graph
from Graph import Node

class Tree:
    def __init__(self):
        self.nodes = []

    def addNode(self, node):
        # Check that node has not been added
        for item in self.nodes:
            if item.name == node.name:
                raise RuntimeError("Node with name \""+node.name+"\" already exists in tree")
            
        self.nodes.append(node)

    