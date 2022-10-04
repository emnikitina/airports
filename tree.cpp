#include "tree.hpp"

Node::Node(): _name(""), _lat(0), _lon(0), _parent(nullptr), _children(std::vector<Node*>()) {}

Node::Node(std::string name, double lat, double lon): _name(name), _lat(lat), _lon(lon), _parent(nullptr), _children(std::vector<Node*>()) {}

Node::Node(Node const& copy): _name(copy._name), _lat(copy._lat), _lon(copy._lon), _parent(copy._parent) {
    _children.clear();
    for (size_t i = 0; i < copy._children.size(); i++)
        _children.push_back(copy._children[i]);
}

Node& Node::operator=(Node const& source) {
    if (this != &source) {
        _name = source._name;
        _lat = source._lat;
        _lon = source._lon;
        _parent = source._parent;
        _children.clear();
        for (size_t i = 0; i < source._children.size(); i++)
            _children.push_back(source._children[i]);
    }
    return *this;
}

Node::~Node() {
    _children.clear();
}

std::ostream& operator<<(std::ostream& out, Node const& node) {
    out << "airport name: " << node._name << " coordinates: (" << node._lat << "; " << node._lon << ")\n";
    return out;
}

Tree::Tree(): _allocator(Allocator()), _root(NULL), _size(0) {}

Tree::~Tree() {
    clear(_root);
}

Tree::pointer Tree::createNode(std::string name, double lat, double lon) {
    Node* node = _allocator.allocate(1);
    _allocator.construct(node, name, lat, lon);
    return node;
}

void Tree::destoyNode(pointer node) {
    if (node) {
        _allocator.destroy(node);
        _allocator.deallocate(node, 1);
    }
}


Tree::pointer Tree::getRoot() const {
    return _root;
}

size_t Tree::getSize() const {
    return _size;
}

void Tree::setRoot(std::string name, double lat, double lon) {
    _root = createNode(name, lat, lon);
}

/* Tree::pointer Tree::find(int cost, pointer node) const {
    pointer finded;

    if (node && node->_cost == cost)
        return node;
    for (size_t i = 0; i < node->_children.size(); i++) {
        if ((finded = find(cost, node->_children[i])))
            return finded;
    }
    return NULL;
} */

void Tree::clear(pointer node) {
    if (node) {
        for (size_t i = 0; i < node->_children.size(); i++)
            clear(node->_children[i]);
        destoyNode(node);
        node = NULL;
    }
}
