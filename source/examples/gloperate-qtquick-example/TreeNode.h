
#pragma once


#include <cppexpose/reflection/Object.h>
#include <cppexpose/reflection/Property.h>


class TreeNode : public cppexpose::Object
{
public:
    // Properties
    cppexpose::Property<int> id;


public:
    TreeNode(const std::string & name, cppexpose::PropertyGroup * parent = nullptr);
    virtual ~TreeNode();

    int getId() const;
    void setId(const int & id);

    void expand();
    void collapse();

    void print(const std::string & indent = "");


protected:
    int        m_id;
    TreeNode * m_left;
    TreeNode * m_right;
};
