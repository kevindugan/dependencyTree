#ifndef DEPENDENCY_NODE_H_12SL
#define DEPENDENCY_NODE_H_12SL

#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>

/**
 * @brief Directed Acyclic Graph for finding dependency ordering
 * @details Constructs a dependency graph based on specified dependencies on
 *          other nodes in the tree. Can find the root of the tree given any
 *          node in the tree and will resolve the dependency ordering of the
 *          tree.
 *
 *          Nodes should be constructed using shared pointers.
 */
template<class T>
class DependencyNode : public std::enable_shared_from_this<DependencyNode<T>>
{
    using node_ptr = std::shared_ptr<DependencyNode<T>>;

  public:
    /**
     * @brief Constructor which sets the data stored for this node
     */
    DependencyNode(const std::string& name, const T& value)
    {
        this->nodeName  = name;
        this->nodeValue = value;
    }
    /**
     * @brief Default destructor
     */
    ~DependencyNode() {}

    /**
     * @brief Returns the data for this node
     */
    T getValue() const { return this->nodeValue; }

    /**
     * @brief Specifies another node which this node depends on.
     * @details Updates a list of this nodes dependencies and updates the
     *          incoming node to add this node as a parent.
     * @param[std::shared_prt<DependencyNode<T>>] node  The node which this
     * node depends on.
     */
    void dependsOn(node_ptr node)
    {
        this->dependencies.push_back(node);
        node->parents.push_back(this->shared_from_this());
    }

    /**
     * @brief List out the dependencies of the incoming node.
     * @details Static method which will accept a shared ptr to a node and
     * print its dependencies including address values.
     * @param[std::shared_prt<DependencyNode<T>>] node  The node whose
     *                                                  dependencies are
     * wanted.
     */
    static void printDependencies(const std::shared_ptr<DependencyNode<T>> node)
    {
        std::cout << "Node (" << node.get() << "): " << node->nodeValue
                  << "  Deps: [ ";
        for (const auto& item : node->dependencies)
        {
            std::cout << item.get() << "(" << item->nodeValue << "), ";
        }
        std::cout << "]" << std::endl;
    }

    /**
     * @brief Static public method which sets up dependency resolution given a
     *        specific node in the tree
     * @details This method sets up a vector which is used to keep track of
     *          circular dependencies and then calls a private method which
     *          recursively resolves the dependencies.
     * @param[node_ptr] node  Shared pointer to a node in the tree
     * @param[std::vector<node_ptr>] resolved  List of pointers to nodes in the
     *                                         tree in dependency resolved
     * order
     */
    static void
    resolveDependencies(const node_ptr node, std::vector<node_ptr>& resolved)
    {
        std::vector<node_ptr> seen;
        resolveDependencies(node, resolved, seen);
    }

    /**
     * @brief Given a node in the tree, find the root of the tree
     * @details Allows for the possibility of multiple roots in a single tree.
     * @param[node_ptr] node  A node in the tree
     * @param[std::vector<node_ptr>] root  Empty list provided by the user.
     * This list is populated with roots of the tree
     */
    static void findRoot(const node_ptr node, std::vector<node_ptr>& root)
    {
        if (node->parents.size() == 0)
        {
            if (std::find(root.begin(), root.end(), node) == root.end())
            {
                // std::cout << "Found Root: " << node->nodeValue << std::endl;
                root.push_back(node);
            }
            return;
        }
        for (const auto& edge : node->parents)
        {
            findRoot(edge, root);
        }
    }

  private:
    std::vector<node_ptr> dependencies, parents;
    std::string           nodeName;
    T                     nodeValue;

    /**
     * @brief Recursively resolves dependencies in the tree
     * @details Keeps track of circular dependencies. If a circular dependency
     *          is detected, this method will throw an exception and list the
     *          nodes which contribute to the error.
     */
    static void resolveDependencies(const node_ptr         node,
                                    std::vector<node_ptr>& resolved,
                                    std::vector<node_ptr>& seen)
    {
        // std::cout << "Node: " << node->nodeValue << std::endl;
        seen.push_back(node);
        for (const auto& edge : node->dependencies)
        {
            if (std::find(resolved.begin(), resolved.end(), edge)
                == resolved.end())
            {
                if (std::find(seen.begin(), seen.end(), edge) != seen.end())
                {
                    throw std::logic_error("Circular reference dectected: "
                                             + node->nodeName + " => "
                                             + edge->nodeName);
                }
                resolveDependencies(edge, resolved, seen);
            }
        }
        resolved.push_back(node);
    }
};

/**
 * @brief Collection of nodes which allows for multiple roots in the same tree
 * @details Keeps a list of all nodes in the tree and uses this list to resolve
 *          all dependencies between all nodes, even if the nodes don't share
 *          a common root. The list of resolved nodes produced from this class
 *          is guaranteed to be in an order which accounts for all dependencies
 */
template<class T>
class DependencyTree
{
    using node_ptr = std::shared_ptr<DependencyNode<T>>;

  public:
    DependencyTree(std::initializer_list<node_ptr> list)
    {
        for (const auto& item : list)
        {
            all_nodes.push_back(item);
        }
    }
    /**
     * @brief Add a node to the tree
     */
    void addNode(node_ptr node) { all_nodes.push_back(node); }

    /**
     * @brief Traverse all nodes and if a node is not found in the list of
     *        resolved nodes, determine its depenencies.
     */
    void resolveDependencies(std::vector<node_ptr>& resolved)
    {
        for (const auto& node : all_nodes)
        {
            if (std::find(resolved.begin(), resolved.end(), node)
                == resolved.end())
            {
                DependencyNode<T>::resolveDependencies(node, resolved);
            }
        }
    }

  private:
    std::vector<node_ptr> all_nodes;
};

#endif // DEPENDENCY_NODE_H_12SL
