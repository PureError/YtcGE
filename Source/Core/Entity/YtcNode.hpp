#ifndef YTC_NODE_HPP
#define YTC_NODE_HPP

#include "../Fundation.hpp"
#include "../StringUtils/YtcString.hpp"
namespace YtcGE
{
    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        Node() = default;
        virtual ~Node();

        virtual String ToString() const noexcept
        {
            return String(_T("Empty Node"));
        }

        std::weak_ptr<Node> Parent() const noexcept
        {
            return parent_;
        }

        const std::vector<std::shared_ptr<Node>> & Children() const noexcept
        {
            return children_;
        }

        std::shared_ptr<Node> Clone()
        {
            return nullptr;
        }

        void AddChild(const std::shared_ptr<Node> & child);
        void RemoveChild(const std::shared_ptr<Node> & child);
        void ClearChildren();
    protected:
        std::weak_ptr<Node> parent_;
        std::vector<std::shared_ptr<Node>> children_;
    };
}

#endif