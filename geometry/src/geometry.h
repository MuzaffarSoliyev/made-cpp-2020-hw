#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

const double EPS = 1e-6;
const double PI = 3.141592653589793;

double degToRad(double angle) { return PI * angle / 180.0; }

double radToDeg(double angle) { return angle * 180.0 / PI; }

struct Point {
  double x;
  double y;

  Point() : x(0.0), y(0.0) {}
  Point(double x, double y) : x(x), y(y) {}

  bool operator==(const Point p) {
    if (fabs(p.x - x) < EPS && fabs(p.y - y) < EPS) {
      return true;
    } else {
      return false;
    }
  }

  bool operator!=(const Point& p) { return !(*this == p); }

  void rotate(Point center, double angle) {
    double x_tmp, y_tmp;
    double ccos = cos(degToRad(angle));
    double ssin = sin(degToRad(angle));
    x_tmp = (x - center.x) * ccos - (y - center.y) * ssin;
    y_tmp = (x - center.x) * ssin + (y - center.y) * ccos;
    x = x_tmp;
    y = y_tmp;
  }

  Point rotate(Point center, double angle) const {
    Point p(*this);
    p.rotate(center, angle);
    return p;
  }
};

class Line {
 public:
  Line(Point p1, Point p2) {
    a = fabs((p2.y - p1.y)) < EPS ? 0.0 : p2.y - p1.y;
    b = fabs((p2.x - p1.x)) < EPS ? 0.0 : -(p2.x - p1.x);
    c = fabs(p1.y * (p2.x - p1.x) - p1.x * (p2.y - p1.y)) < EPS
            ? 0.0
            : p1.y * (p2.x - p1.x) - p1.x * (p2.y - p1.y);
  }

  Line(double slope, double shift) {
    a = slope;
    b = -1.0;
    c = shift;
  }

  Line(Point p, double slope) : a(slope) {
    b = -1.0;
    c = fabs((p.y - slope * p.x)) < EPS ? 0.0 : p.y - slope * p.x;
  }

  ~Line() {}

  bool operator==(const Line& l) const {
    if (fabs(l.a - a) < EPS && fabs(l.b - b) < EPS && fabs(l.c - c) < EPS) {
      return true;
    } else {
      return false;
    }
  }

  void reflex(Point& p) {
    double x = p.x;
    double y = p.y;
    double t = -1 * (a * x + b * y + c) / (a * a + b * b);
    double x_c = a * t + x;
    double y_c = b * t + y;

    Point center(x_c, y_c);
    p.x = 2 * center.x - p.x;
    p.y = 2 * center.y - p.y;
  }

  bool operator!=(const Line& l) const { return !(*this == l); }

 private:
  double a;
  double b;
  double c;
};

class Shape {
 public:
  virtual double perimeter() = 0;
  virtual double area() = 0;
  virtual bool operator==(const Shape& another) = 0;
  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coefficient) = 0;
};

class Polygon : public Shape {
 public:
  Polygon(std::vector<Point> v) {
    for (size_t i = 0; i < v.size(); i++) {
      vertices.push_back(v[i]);
    }
  }

  size_t verticesCount() { return vertices.size(); }

  const std::vector<Point> getVertices() const { return vertices; }

  double perimeter() override {
    double p = 0;
    size_t n = vertices.size();
    for (size_t i = 0; i < n - 1; i++) {
      p += sqrt(pow(vertices[i].x - vertices[i + 1].x, 2) +
                pow(vertices[i].y - vertices[i + 1].y, 2));
    }
    p += sqrt(pow(vertices[n - 1].x - vertices[0].x, 2) +
              pow(vertices[n - 1].y - vertices[0].y, 2));

    return p;
  }

  double area() override {
    double s = 0;
    size_t n = vertices.size();

    for (size_t i = 0; i < n - 1; i++) {
      s += (vertices[i].x * vertices[i + 1].y);
      s -= (vertices[i + 1].x * vertices[i].y);
    }
    s += vertices[n - 1].x * vertices[0].y;
    s -= vertices[0].x * vertices[n - 1].y;
    s = 0.5 * fabs(s);
    return s;
  }

  bool operator==(const Shape& another) override {
    const Polygon* p = dynamic_cast<const Polygon*>(&another);

    std::vector<Point> points = p->getVertices();

    std::sort(points.begin(), points.end(),
              [](Point& p1, Point& p2) { return p1.x < p2.x; });

    std::sort(vertices.begin(), vertices.end(),
              [](Point& p1, Point& p2) { return p1.x < p2.x; });

    if (points.size() != vertices.size()) {
      return false;
    } else {
      for (size_t i = 0; i < points.size(); i++) {
        if (points[i] != vertices[i]) {
          return false;
        }
      }
    }
    return true;
  }

  bool operator!=(const Shape& another) { return !operator==(another); }

  void rotate(Point center, double angle) override {
    double x_tmp, y_tmp;
    for (size_t i = 0; i < vertices.size(); i++) {
      vertices[i].rotate(center, angle);
    }
  }

  void reflex(Point center) override {
    for (size_t i = 0; i < vertices.size(); i++) {
      vertices[i].x = 2 * center.x - vertices[i].x;
      vertices[i].y = 2 * center.y - vertices[i].y;
    }
  }

  void reflex(Line axis) override {
    double x_tmp, y_tmp;
    for (size_t i = 0; i < vertices.size(); i++) {
      axis.reflex(vertices[i]);
    }
  }

  void scale(Point center, double coefficient) override {
    for (size_t i = 0; i < vertices.size(); i++) {
      vertices[i].x =
          vertices[i].x + (coefficient) * (vertices[i].x - center.x);
      vertices[i].y =
          vertices[i].y + (coefficient) * (vertices[i].y - center.y);
    }
  }

 public:
  std::vector<Point> vertices;
};

class Ellipse : public Shape {
 public:
  Ellipse(Point f1, Point f2, double r1r2) : f1(f1), f2(f2), r1r2(r1r2) {
    a = r1r2 / 2;
    c = sqrt(pow(f1.x - f2.x, 2) + pow(f1.y - f2.y, 2)) / 2;
    b = sqrt(pow(a, 2) - pow(c, 2));
  }
  std::pair<Point, Point> focuses() {
    std::pair<Point, Point> result = {f1, f2};
    return result;
  }

  bool operator==(const Shape& another) override {
    const Ellipse* p = dynamic_cast<const Ellipse*>(&another);
    if (((f1 == p->f1 && f2 == p->f2) || (f2 == p->f1 && f1 == p->f2)) &&
        fabs(r1r2 - p->r1r2) < EPS) {
      return true;
    }
    return false;
  }

  double eccentricity() { return c / a; }
  Point center() {
    Point center(0.0, 0.0);
    center.x = (f1.x + f2.x) / 2;
    center.y = (f1.y + f2.y) / 2;
    return center;
  }

  double perimeter() override {
    return 4 * a * std::comp_ellint_2(eccentricity());
  }

  double area() override { return PI * a * b; }

  void rotate(Point center, double angle) override {
    double x_tmp, y_tmp;

    double ccos = cos(degToRad(angle));
    double ssin = sin(degToRad(angle));

    x_tmp = center.x + (f1.x - center.x) * ccos - (f1.y - center.y) * ssin;
    y_tmp = center.y + (f1.x - center.x) * ssin - (f1.y - center.y) * ccos;
    f1.x = x_tmp;
    f1.y = y_tmp;

    x_tmp = center.x + (f2.x - center.x) * ccos - (f2.y - center.y) * ssin;
    y_tmp = center.y + (f2.x - center.x) * ssin - (f2.y - center.y) * ccos;
    f2.x = x_tmp;
    f2.y = y_tmp;
  }

  void reflex(Point center) override {
    f1.x = 2 * center.x - f1.x;
    f1.y = 2 * center.y - f1.y;

    f2.x = 2 * center.x - f2.x;
    f2.y = 2 * center.y - f2.y;
  }

  void reflex(Line axis) override {
    axis.reflex(f1);
    axis.reflex(f2);
  }

  void scale(Point center, double coefficient) override {
    f1.x = f1.x + (coefficient) * (f1.x - center.x);
    f1.y = f1.y + (coefficient) * (f1.y - center.y);
    f2.x = f2.x + (coefficient) * (f2.x - center.x);
    f2.y = f2.y + (coefficient) * (f2.y - center.y);
  }

 private:
  double a, b, c;

 protected:
  double r1r2;
  Point f1;
  Point f2;
};

class Circle : public Ellipse {
 public:
  Circle(Point center, double radius) : Ellipse(center, center, 2 * radius) {}
  double radius() { return r1r2 / 2; }

  double perimeter() override { return 2 * PI * r1r2 / 2; }

  double area() override { return PI * pow(r1r2 / 2, 2); }
};

class Rectangle : public Polygon {
 public:
  Rectangle(Point p1, Point p2, double relation)
      : Polygon({p1, ((const Point)p2).rotate(p1, atan(relation)), p2,
                 ((const Point)p1).rotate(p2, atan(1 / relation))}) {}

  Point center() {
    Point c((vertices[0].x + vertices[2].x) / 2,
            (vertices[0].y + vertices[2].y) / 2);
    return c;
  }

  std::pair<Line, Line> diagonals() {
    Line d1(vertices[0], vertices[2]);
    Line d2(vertices[1], vertices[3]);

    std::pair<Line, Line> diags = {d1, d2};
    return diags;
  }
};

class Square : public Rectangle {
 public:
  Square(Point p1, Point p2) : Rectangle(p1, p2, 1){};

  Circle circumscribedCircle() {
    Point c = this->center();
    double r = sqrt(pow(vertices[0].x - vertices[3].x, 2) +
                    pow(vertices[0].y - vertices[3].y, 2)) /
               2;
    Circle cC(c, r);
    return cC;
  }

  Circle inscribedCircle() {
    Point c = this->center();
    double r = sqrt(pow(vertices[0].x - vertices[1].x, 2) +
                    pow(vertices[0].y - vertices[1].y, 2)) /
               2;
    Circle iC(c, r);

    return iC;
  }
};

class Triangle : public Polygon {
 public:
  explicit Triangle(Point p1, Point p2, Point p3) : Polygon({p1, p2, p3}) {}

  Circle circumscribedCircle() {
    double d = 2 * (vertices[0].x * (vertices[1].y - vertices[2].y) +
                    vertices[1].x * (vertices[2].y - vertices[0].y) +
                    vertices[2].x * (vertices[0].y - vertices[1].y));

    double x_center = ((pow(vertices[0].x, 2) + pow(vertices[0].y, 2)) *
                           (vertices[1].y - vertices[2].y) +
                       (pow(vertices[1].x, 2) + pow(vertices[1].y, 2)) *
                           (vertices[2].y - vertices[0].y) +
                       (pow(vertices[2].x, 2) + pow(vertices[2].y, 2)) *
                           (vertices[0].y - vertices[1].y)) /
                      d;

    double y_center = ((pow(vertices[0].x, 2) + pow(vertices[0].y, 2)) *
                           (vertices[2].x - vertices[1].x) +
                       (pow(vertices[1].x, 2) + pow(vertices[1].y, 2)) *
                           (vertices[0].x - vertices[2].x) +
                       (pow(vertices[2].x, 2) + pow(vertices[2].y, 2)) *
                           (vertices[1].x - vertices[0].x)) /
                      d;
    Point center(x_center, y_center);
    double p = (a + b + c) / 2;
    double r = (a * b * c) / (4 * sqrt(p * (p - a) * (p - b) * (p - c)));
    Circle circle(center, r);
    return circle;
  }

  Circle inscribedCircle() {
    double x_center =
        (b * vertices[0].x + c * vertices[1].x + a * vertices[2].x) /
        (a + b + c);

    double y_center =
        (b * vertices[0].y + c * vertices[1].y + a * vertices[2].y) /
        (a + b + c);
    Point center(x_center, y_center);
    double p = (a + b + c) / 2;
    double r = sqrt(p * (p - a) * (p - b) * (p - c)) / p;
    Circle circle(center, r);
    return circle;
  }

  Point centroid() {
    double x_sum = 0, y_sum = 0;
    for (size_t i = 0; i < 3; i++) {
      x_sum += vertices[i].x;
      y_sum += vertices[i].y;
    }
    Point center(x_sum / 3, y_sum / 3);
    return center;
  }

  Point orthocenter() {
    Point p1, p2, p3;
    if (vertices[0].y != vertices[1].y && vertices[0].y != vertices[2].y) {
      p1 = vertices[0];
      p2 = vertices[1];
      p3 = vertices[2];
    } else if (vertices[1].y != vertices[2].y &&
               vertices[1].y != vertices[0].y) {
      p1 = vertices[1];
      p2 = vertices[2];
      p3 = vertices[0];
    } else {
      p1 = vertices[0];
      p2 = vertices[2];
      p3 = vertices[1];
    }
    double p_slope_1 = -(p2.x - p1.x) / (p2.y - p1.y);
    double p_slope_2 = -(p3.x - p1.x) / (p3.y - p1.y);
    double x = (p3.y + p_slope_1 * p3.x - p2.y - p_slope_2 * p2.x) /
               (p_slope_1 + p_slope_2);
    double y = (p3.y + p_slope_1 * p3.x) - (p_slope_1 * x);

    Point oc(x, y);
    return oc;
  }

  Line EulerLine() {
    Point c1 = ninePointsCircle().center();
    Point c2 = centroid();
    Line eul(c1, c2);
    return eul;
  }

  Circle ninePointsCircle() {
    Circle circumscribedCircle_ = circumscribedCircle();
    Point c1 = circumscribedCircle_.center();
    Point c2 = orthocenter();
    double x = (c1.x + c2.x) / 2;
    double y = (c1.y + c2.y) / 2;
    double r = circumscribedCircle_.radius() / 2;

    Circle eul(Point(x, y), r);
    return eul;
  }

 private:
  double a = sqrt(pow(vertices[1].x - vertices[0].x, 2) +
                  pow(vertices[1].y - vertices[0].y, 2));
  double b = sqrt(pow(vertices[2].x - vertices[1].x, 2) +
                  pow(vertices[2].y - vertices[1].y, 2));
  double c = sqrt(pow(vertices[0].x - vertices[2].x, 2) +
                  pow(vertices[0].y - vertices[2].y, 2));
};