#include "../lib/geometry.hpp"

Vector::Vector() : coordinates_(std::make_pair(0, 0)) {}

Vector::Vector(int64_t xcoord, int64_t ycoord)
    : coordinates_(std::make_pair(xcoord, ycoord)) {}

Vector::Vector(std::pair<int64_t, int64_t> coordinate1,
               std::pair<int64_t, int64_t> coordinate2) {
  coordinates_.first = coordinate2.first - coordinate1.first;
  coordinates_.second = coordinate2.second - coordinate1.second;
}

int64_t Vector::operator*(const Vector& other) const {
  return coordinates_.first * other.coordinates_.first +
         coordinates_.second * other.coordinates_.second;
}

int64_t Vector::operator^(const Vector& other) const {
  return coordinates_.first * other.coordinates_.second -
         coordinates_.second * other.coordinates_.first;
}

Vector Vector::operator+=(const Vector& other) {
  coordinates_.first += other.coordinates_.first;
  coordinates_.second += other.coordinates_.second;
  return *this;
}

Vector Vector::operator-=(const Vector& other) {
  coordinates_.first -= other.coordinates_.first;
  coordinates_.second -= other.coordinates_.second;
  return *this;
}

Vector Vector::operator+(const Vector& other) const {
  Vector result = *this;
  result += other;
  return result;
}

Vector Vector::operator-(const Vector& other) const {
  Vector result = *this;
  result -= other;
  return result;
}

Vector Vector::operator*=(int scalar) {
  coordinates_.first *= scalar;
  coordinates_.second *= scalar;
  return *this;
}

Vector Vector::operator-() const {
  Vector result = *this;
  result *= -1;
  return result;
}

int64_t Vector::GetX() const { return coordinates_.first; }

int64_t Vector::GetY() const { return coordinates_.second; }

Vector::~Vector() = default;

int64_t Vector::Length() const {
  return std::sqrt(coordinates_.first * coordinates_.first +
                   coordinates_.second * coordinates_.second);
}

Point::Point() : xcoord_(0), ycoord_(0) {}

Point::Point(int64_t xcoord, int64_t ycoord)
    : xcoord_(xcoord), ycoord_(ycoord) {}

IShape& Point::Move(const Vector& vector) {
  xcoord_ += vector.GetX();
  ycoord_ += vector.GetY();
  return *this;
}

bool Point::ContainsPoint(const Point& point) const {
  return (xcoord_ == point.xcoord_ && ycoord_ == point.ycoord_);
}

bool Point::CrossSegment(const Segment& segment) const {
  return segment.ContainsPoint(*this);
}

IShape* Point::Clone() const { return new Point(xcoord_, ycoord_); }

int64_t Point::GetX() const { return xcoord_; }

int64_t Point::GetY() const { return ycoord_; }

Vector Point::operator-(const Point& other) const {
  std::pair<int64_t, int64_t> coordinate1 = std::make_pair(xcoord_, ycoord_);
  std::pair<int64_t, int64_t> coordinate2 =
      std::make_pair(other.xcoord_, other.ycoord_);
  return Vector(coordinate2, coordinate1);
}

Segment::Segment() : start_({0, 0}), finish_({0, 0}) {}

Segment::Segment(const std::pair<int64_t, int64_t>& first,
                 const std::pair<int64_t, int64_t>& second)
    : start_(first), finish_(second) {}

Segment::Segment(const Point& point1, const Point& point2) {
  start_ = std::make_pair(point1.GetX(), point1.GetY());
  finish_ = std::make_pair(point2.GetX(), point2.GetY());
}

IShape& Segment::Move(const Vector& vector) {
  start_.first += vector.GetX();
  start_.second += vector.GetY();
  finish_.first += vector.GetX();
  finish_.second += vector.GetY();
  return *this;
}

bool Segment::ContainsPoint(const Point& point) const {
  bool check =
      (Vector({point.GetX() - start_.first, point.GetY() - start_.second}) ^
       Vector(start_, finish_)) == 0;
  check &= point.GetX() >= std::min(start_.first, finish_.first) &&
           point.GetX() <= std::max(start_.first, finish_.first);
  check &= point.GetY() >= std::min(start_.second, finish_.second) &&
           point.GetY() <= std::max(start_.second, finish_.second);
  return check;
}

bool Segment::CrossSegment(const Segment& segment) const {
  Vector vector1(start_, finish_);
  Vector vector2(start_, segment.finish_);
  Vector vector3(start_, segment.start_);
  Vector vector4(segment.start_, segment.finish_);
  Vector vector5(segment.start_, start_);
  Vector vector6(segment.start_, finish_);
  Vector vector7(segment.finish_, start_);
  Vector vector8(segment.finish_, finish_);
  Vector vector9(finish_, segment.finish_);
  Vector vector10(finish_, segment.start_);
  bool case1 = (((vector1 ^ vector3) * (vector1 ^ vector2)) < 0 &&
                ((vector4 ^ vector5) * (vector4 ^ vector6)) < 0);

  bool case2 = ((vector1 ^ vector3) == 0 && (vector5) * (vector6) <= 0);
  bool case3 = ((vector4 ^ vector5) == 0 && (vector3) * (vector2) <= 0);
  bool case4 = ((vector4 ^ vector6) == 0 && (vector10) * (vector9) <= 0);
  bool case5 = ((vector1 ^ vector2) == 0 && (vector7) * (vector8) <= 0);
  bool result = case1 || case2 || case3 || case4 || case5;
  return result;
}

IShape* Segment::Clone() const {
  return new Segment(std::make_pair(start_.first, start_.second),
                     std::make_pair(finish_.first, finish_.second));
}

Point Segment::GetA() const { return Point(start_.first, start_.second); }

Point Segment::GetB() const { return Point(finish_.first, finish_.second); }
double CountDistance(const std::pair<int64_t, int64_t>& point, double xxx,
                     double yyy) {
  return std::sqrt((point.first - xxx) * (point.first - xxx) +
                   (point.second - yyy) * (point.second - yyy));
}
double Segment::DistanceTo(std::pair<int64_t, int64_t> point) const {
  double side_c = finish_.first - start_.first;
  double side_d = finish_.second - start_.second;
  double dot = (point.first - start_.first) * side_c +
               (point.second - start_.second) * side_d;
  double len_sq = side_c * side_c + side_d * side_d;
  double param = (len_sq != 0) ? dot / len_sq : -1;
  double xxx;
  double yyy;
  if (param < 0) {
    xxx = start_.first;
    yyy = start_.second;
  } else if (param > 1) {
    xxx = finish_.first;
    yyy = finish_.second;
  } else {
    xxx = start_.first + param * side_c;
    yyy = start_.second + param * side_d;
  }
  return CountDistance(point, xxx, yyy);
}

Line::Line() : Acoef_(0), Bcoef_(0), Ccoef_(0) {}

Line::Line(int64_t acoef, int64_t bcoef, int64_t ccoef)
    : Acoef_(acoef), Bcoef_(bcoef), Ccoef_(ccoef) {}

Line::Line(const Point& point1, const Point& point2) {
  Acoef_ = point2.GetY() - point1.GetY();
  Bcoef_ = point1.GetX() - point2.GetX();
  Ccoef_ = point1.GetY() * (point2.GetX() - point1.GetX()) -
           point1.GetX() * (point2.GetY() - point1.GetY());
  start_ = point1;
}

IShape& Line::Move(const Vector& vector) {
  Ccoef_ = Ccoef_ - Acoef_ * vector.GetX() - Bcoef_ * vector.GetY();
  return *this;
}

bool Line::ContainsPoint(const Point& point) const {
  return (Acoef_ * point.GetX() + Bcoef_ * point.GetY() + Ccoef_) == 0;
}

bool Line::CrossSegment(const Segment& segment) const {
  Vector direct = DirVector();
  std::pair<int64_t, int64_t> point1 = {segment.GetA().GetX(),
                                        segment.GetA().GetY()};
  std::pair<int64_t, int64_t> point2 = {segment.GetB().GetX(),
                                        segment.GetB().GetY()};
  Vector segment_vector(point1, point2);
  std::pair<int64_t, int64_t> point3 = {start_.GetX(), start_.GetY()};
  Vector vector1(point3, point1);
  Vector vector2(point3, point2);
  bool result = (direct ^ vector1) * (direct ^ vector2) <= 0;
  return result;
}

IShape* Line::Clone() const { return new Line(Acoef_, Bcoef_, Ccoef_); }

Vector Line::DirVector() const { return Vector({0, 0}, {Bcoef_, -Acoef_}); }

int64_t Line::GetA() const { return Acoef_; }

int64_t Line::GetB() const { return Bcoef_; }

int64_t Line::GetC() const { return Ccoef_; }

Ray::Ray() = default;

Ray::Ray(std::pair<int64_t, int64_t> start, std::pair<int64_t, int64_t> point) {
  start_ = Point(start.first, start.second);
  some_point_ = Point(point.first, point.second);
}

Ray::Ray(const Point& point1, const Point& point2)
    : start_(point1), some_point_(point2) {}

IShape& Ray::Move(const Vector& vector) {
  start_.Move(vector);
  some_point_.Move(vector);
  return *this;
}
bool Ray::ContainsPoint(const Point& point) const {
  Vector vector1({start_.GetX(), start_.GetY()}, {point.GetX(), point.GetY()});
  Vector vector2({start_.GetX(), start_.GetY()},
                 {some_point_.GetX(), some_point_.GetY()});
  return (vector1 ^ vector2) == 0 && (vector1 * vector2) >= 0;
}

bool Ray::CrossSegment(const Segment& segment) const {
  Vector vector1({start_.GetX(), start_.GetY()},
                 {segment.GetA().GetX(), segment.GetA().GetY()});
  Vector vector2({start_.GetX(), start_.GetY()},
                 {segment.GetB().GetX(), segment.GetB().GetY()});
  Vector vector3({start_.GetX(), start_.GetY()},
                 {some_point_.GetX(), some_point_.GetY()});
  bool case1 = (vector1 ^ vector3) * (vector2 ^ vector3) < 0;
  bool case2 = (vector1 ^ vector3) == 0 && (vector1 * vector3) >= 0;
  bool case3 = (vector2 ^ vector3) == 0 && (vector2 * vector3) >= 0;
  return case1 || case2 || case3;
}

IShape* Ray::Clone() const {
  return new Ray(std::make_pair(start_.GetX(), start_.GetY()),
                 std::make_pair(some_point_.GetX(), some_point_.GetY()));
}
Point Ray::GetA() const { return start_; }

Vector Ray::GetVector() const {
  return Vector({start_.GetX(), start_.GetY()},
                {some_point_.GetX(), some_point_.GetY()});
}

Circle::Circle() : radius_(0) {}

Circle::Circle(const Point& point, int64_t radius)
    : center_(point), radius_(radius) {}

IShape& Circle::Move(const Vector& vector) {
  center_.Move(vector);
  return *this;
}

bool Circle::ContainsPoint(const Point& point) const {
  return (center_.GetX() - point.GetX()) * (center_.GetX() - point.GetX()) +
             (center_.GetY() - point.GetY()) *
                 (center_.GetY() - point.GetY()) <=
         radius_ * radius_;
}

bool Circle::CrossSegment(const Segment& segment) const {
  Point point1(segment.GetA().GetX(), segment.GetA().GetY());
  Point point2(segment.GetB().GetX(), segment.GetB().GetY());
  Vector vector1({segment.GetA().GetX(), segment.GetA().GetY()},
                 {segment.GetB().GetX(), segment.GetB().GetY()});
  std::pair<int64_t, int64_t> point3 = {center_.GetX(), center_.GetY()};
  int64_t distance = segment.DistanceTo(point3);
  Vector v_c1(point3, {segment.GetA().GetX(), segment.GetA().GetY()});
  Vector v_c2(point3, {segment.GetB().GetX(), segment.GetB().GetY()});
  return distance <= radius_ &&
         (v_c1.Length() >= radius_ || v_c2.Length() >= radius_);
}
IShape* Circle::Clone() const {
  return new Circle(Point(center_.GetX(), center_.GetY()), radius_);
}

Point Circle::GetCentre() const { return center_; }

int64_t Circle::GetRadius() const { return radius_; }
