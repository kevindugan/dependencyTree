from Graph import Node
import pytest

def test_walk_tree():
    a = Node.Node('a')
    b = Node.Node('b')
    c = Node.Node('c')
    d = Node.Node('d')
    e = Node.Node('e')

    a.dependsOn(b)
    a.dependsOn(d)
    a.dependsOn(c)
    b.dependsOn(c)
    b.dependsOn(e)
    c.dependsOn(d)
    c.dependsOn(e)

    print("\nDependency Tree:")
    resolved = []
    a.resolve_dependencies(a, resolved)
    print("Dependencies: ", end='')
    for item in resolved:
        print(item.name, end=' => ')
    print("|\n")

    expected_result = ['d', 'e', 'c', 'b', 'a']
    for result, expected in zip(resolved, expected_result):
        assert result.name == expected

def test_circular_dep():
    a = Node.Node('a')
    b = Node.Node('b')
    c = Node.Node('c')
    d = Node.Node('d')
    e = Node.Node('e')

    a.dependsOn(b)
    a.dependsOn(d)
    b.dependsOn(c)
    b.dependsOn(e)
    c.dependsOn(d)
    c.dependsOn(e)
    d.dependsOn(b)

    resolved = []
    with pytest.raises(RuntimeError):
        a.resolve_dependencies(a, resolved)

def test_find_root():
    a = Node.Node('a')
    b = Node.Node('b')
    c = Node.Node('c')
    d = Node.Node('d')
    e = Node.Node('e')

    a.dependsOn(b)
    a.dependsOn(d)
    b.dependsOn(c)
    b.dependsOn(e)
    c.dependsOn(d)
    c.dependsOn(e)
    for node in [a, b, c, d, e]:
        print(node.name+" parents: "+str( [item.name for item in node.parents] ))

    print("\n\n")

    root = []
    a.find_root(e, root)
    assert len(root) == 1
    assert root[0] == a
