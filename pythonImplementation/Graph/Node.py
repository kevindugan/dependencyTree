# Dependency graph structure

class Node:
    def __init__(self, name):
        self.name = name
        self.dependencies = []
        self.parents = []

    def dependsOn(self, node):
        self.dependencies.append(node)
        node.parents.append(self)



    def resolve_dependencies(self,node, resolved, seen=[]):
        print(node.name)
        seen.append(node)
        for edge in node.dependencies:
            if edge not in resolved:
                if edge in seen:
                    raise RuntimeError('Circular reference detected: %s => %s' % (node.name, edge.name))
                self.resolve_dependencies(edge, resolved, seen)
        resolved.append(node)

    def find_root(self, node, root):
        if not node.parents:
            if node not in root:
                root.append( node)
            return
        for parent in node.parents:
            self.find_root(parent, root)
