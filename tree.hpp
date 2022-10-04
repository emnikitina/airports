#ifndef TREE_HPP
#define TREE_HPP

#include <ostream>
#include <vector>
#include <unordered_map>
#include <memory>

class Node {
    public:
        std::string     		_name;
        double          		_lat;
        double              	_lon;
        Node*                 _parent;
        std::vector<Node*>	_children;

        Node();
        Node(std::string name, double lat, double lon);
        Node(Node const& copy);
        Node& operator=(Node const& source);
        ~Node();

        friend std::ostream& operator<<(std::ostream& out, Node const& node);
};

class Tree {
    public:
        typedef std::allocator<Node>     Allocator;
        typedef typename Allocator::pointer pointer;

    // private:
        Allocator   _allocator;
        pointer     _root;
        size_t      _size;
        pointer     _currentNode;

    // public:
        Tree();
        ~Tree();

        pointer	getRoot() const;
        size_t	getSize() const;

        void setRoot(std::string name, double lat, double lon);

        pointer	createNode(std::string name, double lat, double lon);
        void	destoyNode(pointer node);

        pointer	find(int cost, pointer node) const;
        void	clear(pointer node);
};

#endif // TREE_HPP
