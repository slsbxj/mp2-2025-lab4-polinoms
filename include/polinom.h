#ifndef POLINOM_H
#define POLINOM_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Monom {
  double coefficient;
  int degree;

  Monom(double coeff = 0.0, int deg = 0);

  int getX() const { return degree / 100; }
  int getY() const { return (degree / 10) % 10; }
  int getZ() const { return degree % 10; }

  bool isValid() const;

  bool operator<(const Monom &other) const;
  bool operator==(const Monom &other) const;
};

struct Node {
  Monom data;
  std::unique_ptr<Node> next;

  Node(const Monom &monom);
};

class Polinom {
private:
  std::unique_ptr<Node> head;
  size_t size;

  void InsertSorted(const Monom &monom);
  void removeZeros();

public:
  Polinom();
  Polinom(const std::vector<Monom> &monoms);
  Polinom(const Polinom &other);
  Polinom(Polinom &&other) noexcept;
  ~Polinom() = default;

  Polinom &operator=(const Polinom &other);
  Polinom &operator=(Polinom &&other) noexcept;

  Polinom operator+(const Polinom &other) const;
  Polinom operator-(const Polinom &other) const;
  Polinom operator*(double constant) const;
  Polinom operator*(const Polinom &other) const;

  Polinom &operator+=(const Polinom &other);
  Polinom &operator-=(const Polinom &other);
  Polinom &operator*=(double constant);
  Polinom &operator*=(const Polinom &other);

  Polinom operator-() const;

  size_t getSize() const { return size; }
  bool isEmpty() const { return size == 0; }

  friend std::ostream &operator<<(std::ostream &os, const Polinom &p);
  friend std::istream &operator>>(std::istream &is, Polinom &p);

  static Polinom parseFromString(const std::string &str);

private:
  static void mergePolinom(const Polinom &a, const Polinom &b, Polinom &result,
                           double sign);
};

#endif