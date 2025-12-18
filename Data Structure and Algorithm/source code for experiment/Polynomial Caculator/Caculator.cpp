#include <iostream>
#include "C:\Users\TX\Desktop\Data Structure\include\Caculator.h"
using namespace std;

int main() {
    int n;
    int i = 0;
    LinkList poly[10];
    while(1){
        cout << "=====Univariate Polynomial Caculator=====" << endl;
        cout << "Author: TX                   Version: 1.0" << endl;
        cout << "1.Create a polynomial" << endl;
        cout << "2.Print the polynomial" << endl;
        cout << "3.Destory the polynomial" << endl;
        cout << "4.Add two polynomials" << endl;
        cout << "5.Subtract two polynomials" << endl;
        cout << "6.Multiply two polynomials" << endl;
        cout << "7.Divide two polynomials" << endl;
        cout << "8.Exit" << endl;
        cout << "=========================================" << endl;
        cout << "Please enter your choice: ";
        cin >> n;
        switch(n) {
            case 1:
                CreatePolynomial(poly[i]);
                i++;
                break;
            case 2:
                cout << "Enter the polynomial index to print (1 to " << i << "): ";
                int index;
                cin >> index;
                if (index < 1 || index > i) {
                    cout << "Invalid index." << endl;
                    break;
                }
                PrintPolynomial(poly[index - 1], index);
                break;
            case 3:
                cout << "Enter the polynomial index to destroy (1 to " << i << "): ";
                int delIndex;
                cin >> delIndex;
                if (delIndex < 1 || delIndex > i) {
                    cout << "Invalid index." << endl;
                    break;
                }
                DestroyPolynomial(poly[delIndex - 1]);
                cout << "Polynomial " << delIndex << " destroyed." << endl;
                break;
            case 4:
                cout << "Enter the indices of two polynomials to add (1 to " << i << "): ";
                int idx1, idx2;
                cin >> idx1 >> idx2;
                if (idx1 < 1 || idx1 > i || idx2 < 1 || idx2 > i) {
                    cout << "Invalid indices." << endl;
                    break;
                }
                {
                    LinkList result = AddPolynomials(poly[idx1 - 1], poly[idx2 - 1]);
                    PrintPolynomial(result, 0);
                    DestroyPolynomial(result);
                }
                break;
            case 5:
                cout << "Enter the indices of two polynomials to subtract (1 to " << i << "): ";
                int sidx1, sidx2;
                cin >> sidx1 >> sidx2;
                if (sidx1 < 1 || sidx1 > i || sidx2 < 1 || sidx2 > i) {
                    cout << "Invalid indices." << endl;
                    break;
                }
                {
                    LinkList result = SubtractPolynomials(poly[sidx1 - 1], poly[sidx2 - 1]);
                    PrintPolynomial(result, 0);
                    DestroyPolynomial(result);
                }
                break;
            case 6:
                cout << "Enter the indices of two polynomials to multiply (1 to " << i << "): ";
                int midx1, midx2;
                cin >> midx1 >> midx2;
                if (midx1 < 1 || midx1 > i || midx2 < 1 || midx2 > i) {
                    cout << "Invalid indices." << endl;
                    break;
                }
                {
                    LinkList result = MultiplyPolynomials(poly[midx1 - 1], poly[midx2 - 1]);
                    PrintPolynomial(result, 0);
                    DestroyPolynomial(result);
                }
                break;
            case 7:
                cout << "Enter the indices of two polynomials to divide (1 to " << i << "): ";
                int didx1, didx2;
                cin >> didx1 >> didx2;
                if (didx1 < 1 || didx1 > i || didx2 < 1 || didx2 > i) {
                    cout << "Invalid indices." << endl;
                    break;
                }
                {
                    LinkList quotient, remainder;
                    DividePolynomials(poly[didx1 - 1], poly[didx2 - 1], quotient, remainder);
                    cout << "Quotient: ";
                    PrintPolynomial(quotient, 0);
                    cout << "Remainder: ";
                    PrintPolynomial(remainder, 0);
                    DestroyPolynomial(quotient);
                    DestroyPolynomial(remainder);
                }
                break;
            case 8:
                cout << "Exit" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        cout << endl;
    }
}
