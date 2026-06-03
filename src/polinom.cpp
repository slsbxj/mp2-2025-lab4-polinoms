#include "polinom.h"

#include <cmath>
#include <sstream>
#include <algorithm>
#include <regex>

Monom::Monom(double coeff, int deg) : coefficient(coeff), degree(deg) {}

bool Monom::isValid() const {
	return getX() <= 9 && getY() <= 9 && getZ() <= 9;
}

bool Monom::operator<(const Monom& other) const {
	return degree > other.degree;
}

bool Monom::operator==(const Monom& other) const {
	return degree == other.degree;
}

Node::Node(const Monom& monom) : data(monom), next(nullptr) {}

Polinom::Polinom() :head(std::make_unique<Node>(Monom(0, 0))), size(0) {}

Polinom::Polinom(const std::vector<Monom>& monoms) : Polinom() {
	for (const auto& monom : monoms)
		if (std::abs(monom.coefficient) > 1e-10 && monom.isValid())
			InsertSorted(monom);
}

Polinom::Polinom(const Polinom& other) : Polinom() {
	Node* current = other.head->next.get();
	while (current) {
		InsertSorted(current->data);
		current = current->next.get();
	}
}

Polinom::Polinom(Polinom&& other) noexcept : head(std::move(other.head)), size(other.size) {
	other.head = std::make_unique<Node>(Monom(0, 0));
	other.size = 0;
}

Polinom& Polinom::operator= (const Polinom& other) {
	if (this != &other) {
		head = std::make_unique<Node>(Monom(0, 0));
		size = 0;

		Node* current = other.head->next.get();
		while (current) {
			InsertSorted(current->data);
			current = current->next.get();
		}
	}
	return *this;
}

Polinom& Polinom::operator=(Polinom&& other) noexcept {
	if (this != &other) {
		head = std::move(other.head);
		size = other.size;

		other.head = std::make_unique<Node>(Monom(0, 0));
		other.size = 0;
	}
	return *this;
}

void Polinom::InsertSorted(const Monom& monom) {
	if (std::abs(monom.coefficient) < 1e-10) return;

	Node* prev = head.get();
	Node* current = head->next.get();

	while (current && current->data < monom) {
		prev = current;
		current = current->next.get();
	}

	if (current && current->data == monom) {
		current->data.coefficient += monom.coefficient;
		if (std::abs(current->data.coefficient) < 1e-10) {
			prev->next = std::move(current->next);
			size--;
		}
	}
	else {
		auto newNode = std::make_unique<Node>(monom);
		newNode->next = std::move(prev->next);
		prev->next = std::move(newNode);
		size++;
	}
}

void Polinom::removeZeros() {
	Node* prev = head.get();
	Node* current = head->next.get();

	while (current) {
		if (std::abs(current->data.coefficient) < 1e-10) {
			prev->next = std::move(current->next);
			current = prev->next.get();
			size--;
		}
		else {
			prev = current;
			current = current->next.get();
		}
	}
}

Polinom Polinom::operator+(const Polinom& other) const {
	Polinom result;
	mergePolinom(*this, other, result, 1.0);
	return result;
}

Polinom Polinom::operator-(const Polinom& other) const {
	Polinom result;
	mergePolinom(*this, other, result, -1.0);
	return result;
}

Polinom Polinom::operator*(double constant) const {
	Polinom result;

	Node* current = head->next.get();
	while (current) {
		Monom newMonom = current->data;
		newMonom.coefficient *= constant;
		result.InsertSorted(newMonom);
		current = current->next.get();
	}

	return result;
}

Polinom Polinom::operator*(const Polinom& other) const {
	Polinom result;

	Node* current1 = head->next.get();
	while (current1) {
		Node* current2 = other.head->next.get();
		while (current2) {
			int newX = current1->data.getX() + current2->data.getX();
			int newY = current1->data.getY() + current2->data.getY();
			int newZ = current1->data.getZ() + current2->data.getZ();

			if (newX > 9 || newY > 9 || newZ > 9)
				throw std::runtime_error("ќщибка: степень переменной превышает 9.");

			Monom newMonom(current1->data.coefficient * current2->data.coefficient, newX * 100 + newY * 10 + newZ);
			result.InsertSorted(newMonom);
			current2 = current2->next.get();
		}
		current1 = current1->next.get();
	}
	return result;
}

Polinom& Polinom::operator+=(const Polinom& other) {
	*this = *this + other;
	return *this;
}

Polinom& Polinom::operator-=(const Polinom& other) {
	*this = *this - other;
	return *this;
}

Polinom& Polinom::operator*=(double constant) {
	*this = *this * constant;
	return *this;
}

Polinom& Polinom::operator*=(const Polinom& other) {
	*this = *this * other;
	return *this;
}

Polinom Polinom::operator-() const {
	return *this * (-1.0);
}

void Polinom::mergePolinom(const Polinom& a, const Polinom& b, Polinom& result, double sign) {
	Node* nodeA = a.head->next.get();
	Node* nodeB = b.head->next.get();

	while (nodeA && nodeB) {
		if (nodeA->data.degree == nodeB->data.degree) {
			double coeff = nodeA->data.coefficient + sign * nodeB->data.coefficient;
			if (std::abs(coeff) > 1e-10)
				result.InsertSorted(Monom(coeff, nodeA->data.degree));
			nodeA = nodeA->next.get();
			nodeB = nodeB->next.get();
		}
		else if (nodeA->data < nodeB->data) {
			result.InsertSorted(nodeA->data);
			nodeA = nodeA->next.get();
		}
		else {
			Monom monomB = nodeB->data;
			monomB.coefficient *= sign;
			result.InsertSorted(monomB);
			nodeB = nodeB->next.get();
		}
	}

	while (nodeA) {
		result.InsertSorted(nodeA->data);
		nodeA = nodeA->next.get();
	}

	while (nodeB) {
		Monom monomB = nodeB->data;
		monomB.coefficient *= sign;
		result.InsertSorted(monomB);
		nodeB = nodeB->next.get();
	}
}

std::ostream& operator<<(std::ostream& os, const Polinom& p) {
	Node* current = p.head->next.get();

	if (!current) {
		os << "0";
		return os;
	}

	bool first = true;
	while (current) {
		double coeff = current->data.coefficient;
		int x = current->data.getX();
		int y = current->data.getY();
		int z = current->data.getZ();

		if (first) {
			if (coeff < 0) os << "-";
			first = false;
		}
		else {
			if (coeff > 0) os << " + ";
			else os << " - ";
		}

		double absCoeff = std::abs(coeff);
		if (std::abs(absCoeff - 1.0) > 1e-10 || (x == 0 && y == 0 && z == 0))
			os << absCoeff;

		if (x > 0) os << "x" << (x > 1 ? "^" + std::to_string(x) : "");
		if (y > 0) os << "y" << (y > 1 ? "^" + std::to_string(y) : "");
		if (z > 0) os << "z" << (z > 1 ? "^" + std::to_string(z) : "");

		current = current->next.get();
	}
	return os;
}

std::istream& operator>>(std::istream& is, Polinom& p) {
	std::string input;
	std::getline(is, input);
	p = Polinom::parseFromString(input);
	return is;
}

Polinom Polinom::parseFromString(const std::string& str) {
	Polinom result;
	std::string s = str;

	s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

	if (s.empty()) return result;

	std::vector<std::string> terms;
	std::string current;

	for (size_t i = 0; i < s.length(); i++) {
		char c = s[i];
		if ((c == '+' || c == '-') && i > 0) {
			if (!current.empty()) {
				terms.push_back(current);
				current.clear();
			}
		}
		current += c;
	}
	if (!current.empty()) {
		terms.push_back(current);
	}

	for (const auto& term : terms) {
		if (term.empty()) continue;

		size_t pos = 0;
		double coeff = 1.0;

		int sign = 1;
		if (term[0] == '-') {
			sign = -1;
			pos = 1;
		}
		else if (term[0] == '+') {
			pos = 1;
		}

		std::string numStr;
		while (pos < term.length() && (isdigit(term[pos]) || term[pos] == '.')) {
			numStr += term[pos];
			pos++;
		}

		if (!numStr.empty()) {
			coeff = std::stod(numStr);
		}

		coeff *= sign;

		int x = 0, y = 0, z = 0;

		while (pos < term.length()) {
			char var = term[pos];
			pos++;

			int power = 1;
			if (pos < term.length() && term[pos] == '^') {
				pos++;
				std::string powStr;
				while (pos < term.length() && isdigit(term[pos])) {
					powStr += term[pos];
					pos++;
				}
				if (!powStr.empty()) {
					power = std::stoi(powStr);
				}
			}

			switch (var) {
			case 'x': x = power; break;
			case 'y': y = power; break;
			case 'z': z = power; break;
			}
		}

		int degree = x * 100 + y * 10 + z;
		Monom m(coeff, degree);

		if (m.isValid() && std::abs(coeff) > 1e-10) {
			result.InsertSorted(m);
		}
	}

	return result;
}