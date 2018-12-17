from Graph import Tree, Node
import pytest
    
def test_addNode():
    tree = Tree.Tree()

    tree.addNode(Node.Node('a'))
    tree.addNode(Node.Node('b'))

    with pytest.raises(RuntimeError):
        tree.addNode(Node.Node('a'))