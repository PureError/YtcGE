#include "YtcNode.hpp"

namespace YtcGE
{
    Node::~Node()
    {
        for (auto & child : children_)
        {
            child->parent_.reset();
        }

        if (!parent_.expired())
        {
            parent_.lock()->RemoveChild(shared_from_this());
        }
    }

    void Node::AddChild(const std::shared_ptr<Node> & child)
    {
        auto iter = std::find(children_.begin(), children_.end(), child);
        if (iter == children_.end())
        {
            children_.push_back(child);
            child->parent_ = shared_from_this();
        }
    }

    void Node::RemoveChild(const std::shared_ptr<Node> & child)
    {
        auto iter = std::find(children_.begin(), children_.end(), child);
        if (iter != children_.end())
        {
            (*iter)->parent_.reset();
            children_.erase(iter);
        }
    }

    void Node::ClearChildren()
    {
        for (auto & child : children_)
        {
            child->parent_.reset();
        }

        children_.clear();
    }

}