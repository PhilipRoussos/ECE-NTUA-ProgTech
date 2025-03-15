#include <iostream>
#include <cmath>

using namespace std;

class Polynomial {
protected:
    class Term {
    protected:
        int exponent;
        int coefficient;
        Term *next;

        Term(int exp, int coeff, Term *n) {
            exponent = exp;
            coefficient = coeff;
            next = n;
        }

        friend class Polynomial;
        friend Polynomial operator + (const Polynomial &p, const Polynomial &q);
        friend Polynomial operator * (const Polynomial &p, const Polynomial &q);
        friend ostream & operator << (ostream &out, const Polynomial &p);
    };

    Term *head;

    void push_back(const int &exp, const int &coeff) {
        Term *p = new Term(exp, coeff, nullptr);
        Term *q;

        if(head == nullptr) {
            head = p;
            return;
        }
        else {
            q = head;
            while (q->next != nullptr) {
                q = q->next;
            }
            q->next = p;
        }
    }

    void copy (const Polynomial &t) {
        for(Term *p = t.head; p != nullptr; p = p->next) {
            push_back(p->exponent, p->coefficient);
        }
    }

    void purge() {
        Term *p = head;
        while(p != nullptr) {
            Term *q = p;
            p = p->next;
            delete q;
        }
    }

public:
    Polynomial(){
        head = nullptr;
    }
    Polynomial(const Polynomial &p){
        head = nullptr;
        copy(p);
    }
    ~Polynomial(){
        purge();
    }

    Polynomial & operator = (const Polynomial &p) {
        purge();
        head = nullptr;
        copy(p);
        return *this;
    }

    void addTerm(int expon, int coeff) {
        Term *n = new Term(expon, coeff, nullptr);
        Term *p, *q;

        if(coeff == 0) {
            delete n;
            return;
        }

        if(head == nullptr) {
            head = n;
            head->next = nullptr;
            return;
        }

        if(head->exponent == expon) {
            head->coefficient += coeff;
            if(head->coefficient == 0) {
                p = head;
                head = head->next;
                delete p;
            }
            delete n;
            return;
        }

        p = head;
        q = p->next;
        while(q != nullptr && q->exponent > expon) {
            p = q;
            q = q->next;
        }
        if(q != nullptr && q->exponent == expon) {
            q->coefficient += coeff;
            if(q->coefficient == 0) {
                p->next = q->next;
                delete q;
            }
            delete n;
        } else {
            if(n->exponent < p->exponent) {
                n->next = p->next;
                p->next = n;
            } else {
                n->next = p;
                head = n;
            }
        }
    }


    double evaluate(double x){
        Term *p = head;
        double res = 0;
        while (p != nullptr) {
            res += p->coefficient * pow(x, p->exponent);
            p = p->next;
        }
        return res;
    }

    friend Polynomial operator+ (const Polynomial &p, const Polynomial &q){
        Polynomial res;
        Term *a, *b, *c = nullptr; //3os deikths gia thn periptwsh pou to ena pointer ginei nullptr
        a = p.head;
        b = q.head;
        while(a != nullptr && b!= nullptr) {
            if (a->exponent > b->exponent) {
                res.addTerm(a->exponent, a->coefficient);
                a = a->next;
            }
            else if (a->exponent < b->exponent) {
                res.addTerm(b->exponent, b->coefficient);
                b = b->next;
            }
            else {
                res.addTerm(a->exponent, a->coefficient+b->coefficient);
                a = a->next;
                b = b->next;
            }
        }
        if(a == nullptr && b != nullptr) {
            c = b;
        }

        if(a != nullptr && b == nullptr) {
            c = a;
        }

        while(c != nullptr) {
            res.addTerm(c->exponent, c->coefficient);
            c = c->next;
        }
        return res;

    }
    friend Polynomial operator* (const Polynomial &p, const Polynomial &q){
        Polynomial res;
        Term *i, *j;
        for(i = p.head; i != nullptr; i = i->next) {
            for(j = q.head; j != nullptr; j = j->next) {
                res.addTerm(i->exponent + j->exponent, i->coefficient * j->coefficient);
            }
        }
        return res;
    }

    friend ostream & operator << (ostream &out, const Polynomial &p){
        Term *n = p.head;

        if(n == nullptr) {
            out << "0";
            return out;
        }

        if(n->coefficient < 0)
            out << "- ";
        if(abs(n->coefficient) != 1)
            out << abs(n->coefficient);
        if(n->exponent == 0 && abs(n->coefficient) == 1)
            out << "1";
        if(n->exponent == 1)
            out << "x";
        else if(n->exponent != 0)
            out << "x^" << n->exponent;
        n = n->next;
        while(n != nullptr) {
            if(n->coefficient < 0) {
                out << " - ";
            }
            else {
                out << " + ";
            }
            if(abs(n->coefficient) != 1 || n->exponent == 0) {
                out << abs(n->coefficient);
            }
            if(n->exponent == 1) {
                out << "x";
            }
            else if(n->exponent != 0) {
                out << "x^" << n->exponent;
            }
            n = n->next;
        }

        return out;
    }
};

int main() {
    Polynomial p;    // 0
    p.addTerm(1, 3);   // 3x
    p.addTerm(2, 1);   // x^2
    p.addTerm(0, -1);   // -1

    Polynomial q(p);   // x^2 + 3x - 1
    q.addTerm(1, -3);   // -3x

    cout << "P(x) = " << p << endl;
    cout << "P(1) = " << p.evaluate(1) << endl;
    cout << "Q(x) = " << q << endl;
    cout << "Q(1) = " << q.evaluate(1) << endl;
    cout << "(P+Q)(x) = " << p+q << endl;
    cout << "(P*Q)(x) = " << p*q << endl;
}







