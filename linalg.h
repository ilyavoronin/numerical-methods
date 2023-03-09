#ifndef LINALG_LINALG_HH
#define LINALG_LINALG_HH

#include <ostream>
#include <vector>

class Expression {};

template <class T>
class Array;

template <class E>
Array<typename E::value_type> evaluate(const E& expr) {
    using value_type = typename E::value_type;
    const auto n = expr.size();
    Array<value_type> result(n);
    for (int i=0; i<n; ++i) {
        result(i) = expr.evaluate(i);
    }
    return result;
}

template <class T>
class Array: public Expression {

public:
    using value_type = T;

private:
    std::vector<T> _data;

public:

    Array(std::initializer_list<T> rhs): _data(rhs) {}
    explicit Array(int n): _data(n) {}
    template <class E>
    Array(const E& expr,
          typename std::enable_if<std::is_base_of<Expression,E>::value,E>::type* dummy=nullptr):
            Array(::evaluate(expr)) {}

    Array() = default;
    ~Array() = default;
    Array(Array&& rhs) = default;
    Array(const Array& rhs) = default;
    Array& operator=(Array&& rhs) = default;
    Array& operator=(const Array& rhs) = default;

    T& operator()(int i) { return this->_data[i]; }
    const T& operator()(int i) const { return this->_data[i]; }
    T evaluate(int i) { return this->_data[i]; }
    T evaluate(int i) const { return this->_data[i]; }
    int size() const { return this->_data.size(); }
    void display(std::ostream& out) const {
        out << "Array(";
        const auto n = size();
        if (n != 0) { out << this->_data.front(); }
        for (int i=1; i<n; ++i) { out << ',' << this->_data[i]; }
        out << ')';
    }

};

template <class E>
typename std::enable_if<std::is_base_of<Expression,E>::value,std::ostream&>::type
operator<<(std::ostream& out, const E& expr) {
    expr.display(out); return out;
}

template <class E1, class E2>
class Plus: public Expression {

public:
    using value_type =
            typename std::common_type<typename E1::value_type,typename E2::value_type>::type;

private:
    const E1& _a;
    const E2& _b;

public:
    explicit Plus(const E1& a, const E2& b): _a(a), _b(b) {}
    value_type evaluate(int i) { return this->_a.evaluate(i) + this->_b.evaluate(i); }
    value_type evaluate(int i) const { return this->_a.evaluate(i) + this->_b.evaluate(i); }
    int size() const { return this->_a.size(); }
    void display(std::ostream& out) const {
        out << "Plus(" << this->_a << ", " << this->_b << ')';
    }

};

struct Bool {
public:
    bool value;

    explicit Bool(bool value = false): value(value) {}

    bool t() const {
        return value;
    }

    bool f() const {
        return !value;
    }
};

std::ostream& operator<<(std::ostream& out, const Bool& b) {
    if (b.value) {
        out << "true";
    } else {
        out << "false";
    }
    return out;
}

template <class E1, class E2>
class LT: public Expression {
public:
    using value_type = Bool;

private:
    const E1& _a;
    const E2& _b;

public:
    explicit LT(const E1& a, const E2& b): _a(a), _b(b) {}
    Bool evaluate(int i) { return Bool(this->_a.evaluate(i) < this->_b.evaluate(i)); }
    Bool evaluate(int i) const { return Bool(this->_a.evaluate(i) < this->_b.evaluate(i)); }
    int size() const { return this->_a.size(); }
    void display(std::ostream& out) const {
        out << "Less(" << this->_a << ", " << this->_b << ')';
    }
};

template <class E>
class All: public Expression {
private:
    const E& _a;
public:
    explicit All(const E& a): _a(a) {}

    Bool evaluate(int i) { return Bool(this->_a.evaluate(i)); }
    Bool evaluate(int i) const { return Bool(this->_a.evaluate(i)); }
    int size() const { return this->_a.size(); }
    void display(std::ostream& out) const {
        out << "All(" << this->_a << ')';
    }
};

template <class E>
class Any: public Expression {
private:
    const E& _a;
public:
    explicit Any(const E& a): _a(a) {}

    Bool evaluate(int i) { return Bool(this->_a.evaluate(i)); }
    Bool evaluate(int i) const { return Bool(this->_a.evaluate(i)); }
    int size() const { return this->_a.size(); }
    void display(std::ostream& out) const {
        out << "Any(" << this->_a << ')';
    }
};

template <class E1, class E2, class E3>
class Where: public Expression {

public:
    using value_type =
            typename std::common_type<typename E2::value_type,typename E3::value_type>::type;

private:
    const E1& _a;
    const E2& _b;
    const E3& _c;

public:
    explicit Where(const E1& a, const E2& b, const E3& c): _a(a), _b(b), _c(c) {}
    value_type evaluate(int i) {
        if (_a.evaluate(i).t()) {
            return _b.evaluate(i);
        } else {
            return _c.evaluate(i);
        }
    }
    value_type evaluate(int i) const {
        if (_a.evaluate(i).t()) {
            return _b.evaluate(i);
        } else {
            return _c.evaluate(i);
        }
    }
    int size() const { return this->_a.size(); }
    void display(std::ostream& out) const {
        out << "Where(" << this->_a << ", " << this->_b << ", " << this->_c << ')';
    }

};



template <class E>
Bool evaluate(const All<E> expr) {
    const auto n = expr.size();
    for (int i = 0; i < n; i++) {
        if (expr.evaluate(i).f()) {
            return Bool(false);
        }
    }
    return Bool(true);
}

template <class E>
Bool evaluate(const Any<E> expr) {
    const auto n = expr.size();
    for (int i = 0; i < n; i++) {
        if (expr.evaluate(i).t()) {
            return Bool(true);
        }
    }
    return Bool(false);
}



template <class E1, class E2>
typename std::enable_if<std::is_base_of<Expression,E1>::value &&
                        std::is_base_of<Expression,E2>::value,Plus<E1,E2>>::type
operator+(const E1& a, const E2& b) {
    return Plus<E1,E2>(a,b);
}

template <class E1, class E2>
typename std::enable_if<std::is_base_of<Expression,E1>::value &&
                        std::is_base_of<Expression,E2>::value,LT<E1,E2>>::type
operator<(const E1& a, const E2& b) {
    return LT<E1,E2>(a, b);
}

template <class E>
typename std::enable_if<std::is_base_of<Expression,E>::value && std::is_same<typename E::value_type, Bool>::value,All<E>>::type
all(const E& a) {
    return All<E>(a);
}

template <class E>
typename std::enable_if<std::is_base_of<Expression,E>::value && std::is_same<typename E::value_type, Bool>::value,Any<E>>::type
any(const E& a) {
    return Any<E>(a);
}

template <class E1, class E2, class E3>
typename std::enable_if<std::is_base_of<Expression,E2>::value &&
                        std::is_base_of<Expression,E3>::value &&
                        std::is_same<typename E1::value_type, Bool>::value, Where<E1,E2,E3>>::type
where(const E1& a, const E2& b, const E3& c) {
    return Where<E1,E2,E3>(a, b, c);
}



#endif