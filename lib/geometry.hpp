#pragma once

#include <complex>
#include <cstdint>
#include <string>
#include <vector>

class Vector {
 public:
  Vector();
  Vector(int64_t xcoord, int64_t ycoord);
  Vector(std::pair<int64_t, int64_t> coordinate1,
         std::pair<int64_t, int64_t> coordinate2);
  int64_t operator*(const Vector& other) const;
  int64_t operator^(const Vector& other) const;
  Vector operator+=(const Vector& other);
  Vector operator-=(const Vector& other);
  Vector operator+(const Vector& other) const;
  Vector operator-(const Vector& other) const;
  Vector operator*=(int scalar);
  Vector operator-() const;
  [[nodiscard]] int64_t GetX() const;
  [[nodiscard]] int64_t GetY() const;
  [[nodiscard]] int64_t Length() const;
  ~Vector();

 private:
  std::pair<int64_t, int64_t> coordinates_;
};

class Point;
class Segment;
class IShape {
 public:
  virtual IShape& Move(const Vector& vector) = 0;
  [[nodiscard]] virtual bool ContainsPoint(const Point& point) const = 0;
  [[nodiscard]] virtual bool CrossSegment(const Segment&) const = 0;
  [[nodiscard]] virtual IShape* Clone() const = 0;
  virtual ~IShape() = default;
};

class Point : public IShape {
 public:
  Point();
  Point(int64_t xcoord, int64_t ycoord);
  IShape& Move(const Vector& vector) override;
  [[nodiscard]] bool ContainsPoint(const Point& point) const override;
  [[nodiscard]] bool CrossSegment(const Segment& segment) const override;
  [[nodiscard]] IShape* Clone() const override;
  [[nodiscard]] int64_t GetX() const;
  [[nodiscard]] int64_t GetY() const;
  Vector operator-(const Point& other) const;
  ~Point() override = default;

 private:
  int64_t xcoord_;
  int64_t ycoord_;
};

class Segment : public IShape {
 public:
  Segment();
  Segment(const Point& point1, const Point& point2);
  Segment(const std::pair<int64_t, int64_t>& first,
          const std::pair<int64_t, int64_t>& second);
  IShape& Move(const Vector& vector) override;
  [[nodiscard]] bool ContainsPoint(const Point& point) const override;
  [[nodiscard]] bool CrossSegment(const Segment& segment) const override;
  [[nodiscard]] IShape* Clone() const override;
  ~Segment() override = default;
  [[nodiscard]] Point GetA() const;
  [[nodiscard]] Point GetB() const;
  [[nodiscard]] double DistanceTo(std::pair<int64_t, int64_t> point1) const;

 private:
  std::pair<int64_t, int64_t> start_;
  std::pair<int64_t, int64_t> finish_;
};

class Line : public IShape {
 public:
  Line();
  Line(int64_t acoef, int64_t bcoef, int64_t ccoef);
  Line(const Point& point1, const Point& point2);
  IShape& Move(const Vector& vector) override;
  [[nodiscard]] bool ContainsPoint(const Point& point) const override;
  [[nodiscard]] bool CrossSegment(const Segment& segment) const override;
  [[nodiscard]] IShape* Clone() const override;
  ~Line() override = default;
  [[nodiscard]] Vector DirVector() const;
  [[nodiscard]] int64_t GetA() const;
  [[nodiscard]] int64_t GetB() const;
  [[nodiscard]] int64_t GetC() const;

 private:
  int64_t Acoef_;
  int64_t Bcoef_;
  int64_t Ccoef_;
  Point start_;
  Point finish_;
};

class Ray : public IShape {
 public:
  Ray();
  Ray(std::pair<int64_t, int64_t> start, std::pair<int64_t, int64_t> point);
  Ray(const Point& point1, const Point& point2);
  IShape& Move(const Vector& vector) override;
  [[nodiscard]] bool ContainsPoint(const Point& point) const override;
  [[nodiscard]] bool CrossSegment(const Segment& segment) const override;
  [[nodiscard]] IShape* Clone() const override;
  ~Ray() override = default;
  [[nodiscard]] Point GetA() const;
  [[nodiscard]] Vector GetVector() const;

 private:
  Point start_;
  Point some_point_;
};

class Circle : public IShape {
 public:
  Circle();
  Circle(const Point& point, int64_t radius);
  IShape& Move(const Vector& vector) override;
  [[nodiscard]] bool ContainsPoint(const Point& point) const override;
  [[nodiscard]] bool CrossSegment(const Segment& segment) const override;
  [[nodiscard]] IShape* Clone() const override;
  ~Circle() override = default;
  [[nodiscard]] Point GetCentre() const;
  [[nodiscard]] int64_t GetRadius() const;

 private:
  Point center_;
  int64_t radius_ = 0;
};