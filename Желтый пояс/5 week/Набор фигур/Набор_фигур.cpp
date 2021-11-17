#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <memory>
#include <cmath>

class Figure {
public:
    virtual std::string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Triangle : public Figure {
public:
    Triangle(size_t side_1, size_t side_2, size_t side_3) : side_1(side_1), side_2(side_2), side_3(side_3) {}
    std::string Name() const override {
        return std::string("TRIANGLE");
    }
    double Perimeter() const override {
        return side_1 + side_2 + side_3;
    }
    double Area() const override {
        const double half_perimeter = Perimeter() / 2.;
        return std::sqrt(half_perimeter * (half_perimeter - side_1) * (half_perimeter - side_2) * (half_perimeter - side_3));
    }
private:
    size_t side_1;
    size_t side_2;
    size_t side_3;
};

class Rect : public Figure {
public:
    Rect(size_t width, size_t height) : width(width), height(height) {}
    std::string Name() const override {
        return std::string("RECT");
    }
    double Perimeter() const override {
        return 2. * (width + height);
    }
    double Area() const override {
        return width * height;
    }
private:
    size_t width;
    size_t height;
};

class Circle : public Figure {
public:
    Circle(size_t radius) : radius(radius) {}
    std::string Name() const override {
        return std::string("CIRCLE");
    }
    double Perimeter() const override {
        return 2. * pi * radius;
    }
    double Area() const override {
        return pi * radius * radius;
    }
private:
    const double pi = 3.14;
    size_t radius;
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& is) {
    std::string figure;
    is >> figure;

    if (figure == "TRIANGLE") {
        size_t side_1, side_2, side_3;
        is >> side_1 >> side_2 >> side_3;
        return std::make_shared<Triangle>(side_1, side_2, side_3);
    } else if (figure == "RECT") {
        size_t width, height;
        is >> width >> height;
        return std::make_shared<Rect>(width, height);
    } else {
        size_t radius;
        is >> radius;
        return std::make_shared<Circle>(radius);
    }
}


int main() {
    std::vector<std::shared_ptr<Figure>> figures;
    for (std::string line; getline(std::cin, line); ) {
        std::istringstream is(line);

        std::string command;
        is >> command;
        if (command == "ADD") {
            is >> std::ws;
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                std::cout << std::fixed << std::setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << std::endl;
            }
        }
    }
    return 0;
}