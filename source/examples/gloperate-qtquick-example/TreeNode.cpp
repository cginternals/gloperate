
#include "TreeNode.h"

#include <cppassist/string/conversion.h>

#include <iostream>


using namespace cppexpose;


static int g_nextId = 1;


TreeNode::TreeNode(const std::string & name, PropertyGroup * parent)
: Object(name, parent)
, id("id", this, this, &TreeNode::getId, &TreeNode::setId)
, m_id(g_nextId++)
, m_left(nullptr)
, m_right(nullptr)
{
    // Register functions
    addFunction("expand",   this, &TreeNode::expand);
    addFunction("collapse", this, &TreeNode::collapse);
    addFunction("print",    this, &TreeNode::print);
}

TreeNode::~TreeNode()
{
    std::cout << "TreeNode destroyed." << std::endl;
}

int TreeNode::getId() const
{
    return m_id;
}

void TreeNode::setId(const int & id)
{
    m_id = id;
}

void TreeNode::expand()
{
    // Abort if already expanded
    if (m_left || m_right)
    {
        return;
    }

    // Create child nodes
    m_left = new TreeNode("left");
    addProperty(m_left, PropertyOwnership::Parent);

    m_right = new TreeNode("right");
    addProperty(m_right, PropertyOwnership::Parent);
}

void TreeNode::collapse()
{
    // Destroy child nodes
    if (m_left)
    {
        destroyProperty(m_left);
        m_left = nullptr;
    }

    if (m_right)
    {
        destroyProperty(m_right);
        m_right = nullptr;
    }
}

void TreeNode::print(const std::string & indent)
{
    std::cout << "node(" << cppassist::toString<int>(m_id) << ")" << std::endl;

    if (m_left)
    {
        std::cout << indent << "  left: ";
        m_left ->print(indent + "  ");
    }

    if (m_right)
    {
        std::cout << indent << "  right: ";
        m_right->print(indent + "  ");
    }
}
