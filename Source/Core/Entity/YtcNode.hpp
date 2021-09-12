#ifndef YTC_NODE_HPP
#define YTC_NODE_HPP

#include "../Fundation.hpp"
#include "../StringUtils/YtcString.hpp"
#include "../Math/YtcMatrix.hpp"
#include "../Render/YtcModel.hpp"
namespace YtcGE
{
    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        Node() = default;
        Node(const Node&) = delete;
        Node& operator=(const Node)=delete;
        virtual ~Node();

        const String& Name() const noexcept
        {
            return name_;
        }

        String& Name() noexcept
        {
            return name_;
        }

        virtual String ToString() const noexcept
        {
            return String(_T("Empty Node"));
        }

        void Translate(const Vec3f& v) noexcept
        {
            position_ += v;
            auto& row_3 = mat_m2w[3];
            row_3 = position_;
        }

        void Rotate(const Vec3f& r, float theta)
        {
            if (model_)
            {
                model_->Rotate(r, theta);
            }
        }

        void Scale(const Vec3f& s)
        {
            if (model_)
            {
                model_->Scale(s);
            }
        }

        std::weak_ptr<Node> Parent() const noexcept
        {
            return parent_;
        }

        bool Visible() const noexcept
        {
            return visible_;
        }

        bool& Visible() noexcept
        {
            return visible_;
        }

        const std::vector<std::shared_ptr<Node>> & Children() const noexcept
        {
            return children_;
        }

        virtual std::shared_ptr<Node> Clone()
        {
            return nullptr;
        }

        const Vec3f & Position() const noexcept
        {
            return position_;
        }

        Model::Ptr & ModelInUse() noexcept
        {
            return model_;
        }

        const Model::Ptr& ModelInUse() const noexcept
        {
            return model_;
        }

        const Mat44f& Model2Word() const noexcept
        {
            return mat_m2w;
        }

        bool Texture(const Texture2D::Ptr & texture) noexcept
        {
            if (this->model_)
            {
                this->model_->texture = texture;
                return true;
            }
            return false;
        }

        void AddChild(const std::shared_ptr<Node> & child);
        void RemoveChild(const std::shared_ptr<Node> & child);
        void ClearChildren();
    protected:
        String name_;
        std::weak_ptr<Node> parent_;
        bool visible_;
        std::vector<std::shared_ptr<Node>> children_;
        Vec3f position_ = { 0.0f, 0.0f, 0.0f };
        Mat44f mat_m2w = SquareMatrixHelper44f::Identity();
        Model::Ptr model_;
    };
}

#endif