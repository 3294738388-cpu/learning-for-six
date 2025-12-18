#ifndef CACULATOR_H
#define CACULATOR_H

#include <iostream>
using namespace std;

typedef struct LNode {
    int coeff;
    int exp;
    struct LNode *next;
} LNode, *LinkList;

void InitList_L(LinkList &L);
void CreatePolynomial(LinkList &poly);
void PrintPolynomial(LinkList &poly);
void DestroyPolynomial(LinkList &poly);
LinkList AddPolynomials(LinkList &poly1, LinkList &poly2);
LinkList SubtractPolynomials(LinkList &poly1, LinkList &poly2);
LinkList MultiplyPolynomials(LinkList &poly1, LinkList &poly2);
void DividePolynomials(LinkList &poly1, LinkList &poly2, LinkList &quotient, LinkList &remainder);

void InitList_L(LinkList &L) {
    L = NULL;
}

void CreatePolynomial(LinkList &poly) {
    InitList_L(poly);
    int n, coeff, exp;
    cout << "Enter the number of terms in the polynomial: ";
    cin >> n;
    cout << "Enter each term as coefficient and exponent (e.g., 3 2 for 3x^2):" << endl;
    for (int i = 0; i < n; i++) {
        cin >> coeff >> exp;
        LNode *newNode = new LNode{coeff, exp, NULL};
        if (!poly || poly->exp < exp) {
            newNode->next = poly;
            poly = newNode;
        } else {
            LNode *p = poly;
            while (p->next && p->next->exp >= exp) {
                p = p->next;
            }
            if (p->exp == exp) {
                p->coeff += coeff;
                delete newNode;
            } else {
                newNode->next = p->next;
                p->next = newNode;
            }
        }
    }
}

void PrintPolynomial(LinkList &poly, int index) {
    cout << "Polynomial " << index << ": ";
    if (!poly) {
        cout << "0" << endl;
        return;
    }
    LNode *p = poly;
    while (p) {
        if (p != poly && p->coeff > 0) cout << "+";
        if (p->exp == 0) {
            cout << p->coeff;
        } else if (p->exp == 1) {
            cout << p->coeff << "x";
        } else {
            cout << p->coeff << "x^" << p->exp;
        }
        p = p->next;
    }
    cout << endl;
}

void DestroyPolynomial(LinkList &poly) {
    LNode *p = poly;
    while (p) {
        LNode *temp = p;
        p = p->next;
        delete temp;
    }
    poly = NULL;
}

LinkList AddPolynomials(LinkList &poly1, LinkList &poly2) {
    LinkList result;
    InitList_L(result);
    LNode *p1 = poly1, *p2 = poly2, *tail = NULL;
    while (p1 || p2) {
        int coeff, exp;
        if (p1 && (!p2 || p1->exp > p2->exp)) {
            coeff = p1->coeff;
            exp = p1->exp;
            p1 = p1->next;
        } else if (p2 && (!p1 || p2->exp > p1->exp)) {
            coeff = p2->coeff;
            exp = p2->exp;
            p2 = p2->next;
        } else {
            coeff = p1->coeff + p2->coeff;
            exp = p1->exp;
            p1 = p1->next;
            p2 = p2->next;
        }
        if (coeff != 0) {
            LNode *newNode = new LNode{coeff, exp, NULL};
            if (!result) {
                result = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }
    return result;
}

LinkList SubtractPolynomials(LinkList &poly1, LinkList &poly2) {
    LinkList result;
    InitList_L(result);
    LNode *p1 = poly1, *p2 = poly2, *tail = NULL;
    while (p1 || p2) {
        int coeff, exp;
        if (p1 && (!p2 || p1->exp > p2->exp)) {
            coeff = p1->coeff;
            exp = p1->exp;
            p1 = p1->next;
        } else if (p2 && (!p1 || p2->exp > p1->exp)) {
            coeff = p2->coeff;
            exp = p2->exp;
            p2 = p2->next;
        } else {
            coeff = p1->coeff - p2->coeff;
            exp = p1->exp;
            p1 = p1->next;
            p2 = p2->next;
        }
        if (coeff != 0) {
            LNode *newNode = new LNode{coeff, exp, NULL};
            if (!result) {
                result = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }
    return result;
}

LinkList MultiplyPolynomials(LinkList &poly1, LinkList &poly2) {
    LinkList result;
    InitList_L(result);
    for (LNode *p1 = poly1; p1; p1 = p1->next) {
        LinkList temp;
        InitList_L(temp);
        for (LNode *p2 = poly2; p2; p2 = p2->next) {
            int coeff = p1->coeff * p2->coeff;
            int exp = p1->exp + p2->exp;
            LNode *newNode = new LNode{coeff, exp, NULL};
            if (!temp || temp->exp < exp) {
                newNode->next = temp;
                temp = newNode;
            } else {
                LNode *p = temp;
                while (p->next && p->next->exp >= exp) {
                    p = p->next;
                }
                if (p->exp == exp) {
                    p->coeff += coeff;
                    delete newNode;
                } else {
                    newNode->next = p->next;
                    p->next = newNode;
                }
            }
        }
        LinkList newResult = AddPolynomials(result, temp);
        DestroyPolynomial(result);
        DestroyPolynomial(temp);
        result = newResult;
    }
    return result;
}

void DividePolynomials(LinkList &poly1, LinkList &poly2, LinkList &quotient, LinkList &remainder) {
    InitList_L(quotient);
    InitList_L(remainder);
    if (!poly2) {
        cout << "Error: Division by zero polynomial." << endl;
        return;
    }
    DestroyPolynomial(remainder);
    for (LNode *p = poly1; p; p = p->next) {
        LNode *newNode = new LNode{p->coeff, p->exp, NULL};
        if (!remainder) {
            remainder = newNode;
        } else {
            LNode *tail = remainder;
            while (tail->next) tail = tail->next;
            tail->next = newNode;
        }
    }
    while (remainder && remainder->exp >= poly2->exp) {
        int coeff = remainder->coeff / poly2->coeff;
        int exp = remainder->exp - poly2->exp;
        LNode *newNode = new LNode{coeff, exp, NULL};
        if (!quotient) {
            quotient = newNode;
        } else {
            LNode *tail = quotient;
            while (tail->next) tail = tail->next;
            tail->next = newNode;
        }
        LinkList temp;
        InitList_L(temp);
        for (LNode *p = poly2; p; p = p->next) {
            int t_coeff = coeff * p->coeff;
            int t_exp = exp + p->exp;
            LNode *t_node = new LNode{t_coeff, t_exp, NULL};
            if (!temp || temp->exp < t_exp) {
                t_node->next = temp;
                temp = t_node;
            } else {
                LNode *q = temp;
                while (q->next && q->next->exp >= t_exp) {
                    q = q->next;
                }
                if (q->exp == t_exp) {
                    q->coeff += t_coeff;
                    delete t_node;
                } else {
                    t_node->next = q->next;
                    q->next = t_node;
                }
            }
        }
        LinkList newRemainder = SubtractPolynomials(remainder, temp);
        DestroyPolynomial(remainder);
        DestroyPolynomial(temp);
        remainder = newRemainder;
    }
}

#endif
