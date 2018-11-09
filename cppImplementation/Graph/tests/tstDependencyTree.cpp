#include "../DependencyTree.h"
#include "gtest/gtest.h"

class TestDependencyNode : public ::testing::Test
{
  protected:
    using node_ptr = std::shared_ptr<DependencyNode<std::string>>;

  public:
    std::string testFunction1() const { return "test function 1"; }
    std::string testFunction2() const { return "test function 2"; }
    std::string testFunction3() const { return "test function 3"; }
    std::string testFunction4() const { return "test function 4"; }
    std::string testFunction5() const { return "test function 5"; }
};

TEST_F(TestDependencyNode, walk_tree)
{
    node_ptr a = std::make_shared<DependencyNode<std::string>>("a", "a");
    node_ptr b = std::make_shared<DependencyNode<std::string>>("b", "b");
    node_ptr c = std::make_shared<DependencyNode<std::string>>("c", "c");
    node_ptr d = std::make_shared<DependencyNode<std::string>>("d", "d");
    node_ptr e = std::make_shared<DependencyNode<std::string>>("e", "e");

    a->dependsOn(b);
    a->dependsOn(d);
    b->dependsOn(c);
    b->dependsOn(e);
    c->dependsOn(d);
    c->dependsOn(e);

    // Graph (-. == ->)
    //        A
    //       / \
    //      .   .
    //     D     B
    //      .   / \
    //       \ .   .
    //        C -> E

    std::vector<node_ptr> resolved;
    DependencyNode<std::string>::resolveDependencies(a, resolved);

    std::vector<node_ptr> expected;
    expected.push_back(d);
    expected.push_back(e);
    expected.push_back(c);
    expected.push_back(b);
    expected.push_back(a);

    ASSERT_EQ(resolved.size(), expected.size());
    for (auto index = 0; index < expected.size(); index++)
        EXPECT_EQ(resolved[index].get(), expected[index].get());
}

TEST_F(TestDependencyNode, circular_dependency)
{
    node_ptr a = std::make_shared<DependencyNode<std::string>>("a", "a");
    node_ptr b = std::make_shared<DependencyNode<std::string>>("b", "b");
    node_ptr c = std::make_shared<DependencyNode<std::string>>("c", "c");
    node_ptr d = std::make_shared<DependencyNode<std::string>>("d", "d");
    node_ptr e = std::make_shared<DependencyNode<std::string>>("e", "e");

    a->dependsOn(b);
    a->dependsOn(d);
    b->dependsOn(c);
    b->dependsOn(e);
    c->dependsOn(d);
    c->dependsOn(e);
    d->dependsOn(b); // Creates circular dependence

    // Graph (-. == ->)
    //        A
    //       / \
    //      .   .
    //     D --> B
    //      .   / \
    //       \ .   .
    //        C -> E

    std::vector<node_ptr> resolved;
    EXPECT_THROW(DependencyNode<std::string>::resolveDependencies(a, resolved),
                 std::logic_error);
}

TEST_F(TestDependencyNode, find_root)
{
    node_ptr a = std::make_shared<DependencyNode<std::string>>("a", "a");
    node_ptr b = std::make_shared<DependencyNode<std::string>>("b", "b");
    node_ptr c = std::make_shared<DependencyNode<std::string>>("c", "c");
    node_ptr d = std::make_shared<DependencyNode<std::string>>("d", "d");
    node_ptr e = std::make_shared<DependencyNode<std::string>>("e", "e");

    a->dependsOn(b);
    a->dependsOn(d);
    b->dependsOn(c);
    b->dependsOn(e);
    c->dependsOn(d);
    c->dependsOn(e);

    // Graph (-. == ->)
    //        A
    //       / \
    //      .   .
    //     D     B
    //      .   / \
    //       \ .   .
    //        C -> E

    std::vector<node_ptr> roots;
    DependencyNode<std::string>::findRoot(e, roots);
    ASSERT_EQ(1, roots.size());
    EXPECT_EQ(a, roots[0]) << "Expected: " << a->getValue()
                           << "  Result: " << roots[0]->getValue();

    roots.clear();
    DependencyNode<std::string>::findRoot(d, roots);
    ASSERT_EQ(1, roots.size());
    EXPECT_EQ(a, roots[0]) << "Expected: " << a->getValue()
                           << "  Result: " << roots[0]->getValue();
}

TEST_F(TestDependencyNode, function_ptr)
{
    using Func             = std::string (TestDependencyNode::*)() const;
    using String_Func_Pair = std::pair<std::string, Func>;
    using node_ptr         = std::shared_ptr<DependencyNode<String_Func_Pair>>;
    node_ptr a = std::make_shared<DependencyNode<String_Func_Pair>>(
        "a", std::make_pair("a", &TestDependencyNode::testFunction1));
    node_ptr b = std::make_shared<DependencyNode<String_Func_Pair>>(
        "b", std::make_pair("b", &TestDependencyNode::testFunction2));
    node_ptr c = std::make_shared<DependencyNode<String_Func_Pair>>(
        "c", std::make_pair("c", &TestDependencyNode::testFunction3));
    node_ptr d = std::make_shared<DependencyNode<String_Func_Pair>>(
        "d", std::make_pair("d", &TestDependencyNode::testFunction4));
    node_ptr e = std::make_shared<DependencyNode<String_Func_Pair>>(
        "e", std::make_pair("e", &TestDependencyNode::testFunction5));

    a->dependsOn(b);
    a->dependsOn(d);
    b->dependsOn(c);
    b->dependsOn(e);
    c->dependsOn(d);
    c->dependsOn(e);

    // Graph (-. == ->)
    //        A
    //       / \
    //      .   .
    //     D     B
    //      .   / \
    //       \ .   .
    //        C -> E

    std::vector<node_ptr> resolved;
    DependencyNode<String_Func_Pair>::resolveDependencies(a, resolved);

    std::vector<std::string> expected = {this->testFunction4(),
                                         this->testFunction5(),
                                         this->testFunction3(),
                                         this->testFunction2(),
                                         this->testFunction1()};
    ASSERT_EQ(expected.size(), resolved.size());

    for (auto index = 0; index < expected.size(); index++)
    {
        Func        ptr   = resolved[index]->getValue().second;
        std::string value = (this->*ptr)();
        EXPECT_EQ(expected[index], value);
    }
}

TEST_F(TestDependencyNode, multiple_root)
{
    node_ptr a = std::make_shared<DependencyNode<std::string>>("a", "a");
    node_ptr b = std::make_shared<DependencyNode<std::string>>("b", "b");
    node_ptr c = std::make_shared<DependencyNode<std::string>>("c", "c");
    node_ptr d = std::make_shared<DependencyNode<std::string>>("d", "d");
    node_ptr e = std::make_shared<DependencyNode<std::string>>("e", "e");
    node_ptr f = std::make_shared<DependencyNode<std::string>>("f", "f");
    node_ptr g = std::make_shared<DependencyNode<std::string>>("g", "g");
    node_ptr h = std::make_shared<DependencyNode<std::string>>("h", "h");

    DependencyTree<std::string> tree = {a, b, c, d, e, f, g};
    tree.addNode(h);

    a->dependsOn(b);
    a->dependsOn(d);
    b->dependsOn(c);
    b->dependsOn(e);
    c->dependsOn(d);
    c->dependsOn(e);

    f->dependsOn(g);
    f->dependsOn(h);

    // Graph (-. == ->)
    //        A                     F
    //       / \                   / \
    //      .   .                 .    .
    //     D     B               G      H
    //      .   / \
    //       \ .   .
    //        C -> E

    std::vector<node_ptr> resolved;
    tree.resolveDependencies(resolved);

    std::vector<node_ptr> expected;
    expected.push_back(d);
    expected.push_back(e);
    expected.push_back(c);
    expected.push_back(b);
    expected.push_back(a);
    expected.push_back(g);
    expected.push_back(h);
    expected.push_back(f);

    ASSERT_EQ(resolved.size(), expected.size());
    for (auto index = 0; index < expected.size(); index++)
        EXPECT_EQ(resolved[index].get(), expected[index].get());
}
