#include "Common.h"

#include <stdexcept>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Shape : public IShape {
public:
    Shape() = default;

    Shape(const Point& position, const Size& size, const std::shared_ptr<ITexture>& texture)
        : position_(position), size_(size), texture_(texture) {}

    void SetPosition(Point position) override {
        position_ = move(position);
    }

    Point GetPosition() const override {
        return position_;
    }

    void SetSize(Size size) override {
        size_ = move(size);
    }

    Size GetSize() const override {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> texture) override {
        texture_ = move(texture);
    }

    ITexture* GetTexture() const override {
        return texture_.get();
    }

    void Draw(Image& image) const override {
        Size texture_size = (texture_ ? texture_->GetSize() : Size());
        for (int y = position_.y; y < position_.y + size_.height && y < image.size(); ++y) {
            for (int x = position_.x; x < position_.x + size_.width && x < image[0].size(); ++x) {
                Point local_p = {x - position_.x, y - position_.y};
                if (CheckPoint(local_p, size_)) {
                    if (local_p.y < texture_size.height && local_p.x < texture_size.width) {
                        image[y][x] = texture_->GetImage()[local_p.y][local_p.x]; 
                    } else {
                        image[y][x] = '.';
                    }
                }
            }
        }
    }

protected:
    virtual bool CheckPoint(Point p, Size size) const = 0;

    Point position_;
    Size size_;
    std::shared_ptr<ITexture> texture_;
};

class Ellipse : public Shape {
public:
    
    using Shape::Shape;

    std::unique_ptr<IShape> Clone() const override {
        return make_unique<Ellipse>(position_, size_, texture_);
    }

    bool CheckPoint(Point p, Size size) const override {
        return IsPointInEllipse(move(p), move(size));
    }

};

class Rectangle : public Shape {
public:
    using Shape::Shape;

    std::unique_ptr<IShape> Clone() const override {
        return make_unique<Rectangle>(position_, size_, texture_);
    }

    bool CheckPoint(Point p, Size size) const override {
        return true;
    }
};


// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    switch(shape_type) {
    case ShapeType::Ellipse:
        return make_unique<Ellipse>();
    case ShapeType::Rectangle:
        return make_unique<Rectangle>();
    default:
        throw exception();
    }
}