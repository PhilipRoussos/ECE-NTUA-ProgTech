#include <iostream>
#include <array>
#include <iomanip>
#include <stdexcept>

using namespace std;

class ChessBoardArray {
protected:
    class Row {
    public:
        Row(ChessBoardArray &a, int i)
            : array(a), row(i) {}

        int & operator[](int i) const{
            return array.select(row, i);
        }
    private:
        ChessBoardArray &array;
        int row;
    };

    class ConstRow {
    public:
        ConstRow(const ChessBoardArray &a, int i)
            : array(a), row(i) {}

        int operator[](int i) const {
            return array.select(row, i);
        }
    private:
        const ChessBoardArray &array;
        int row;
    };

public:
    ChessBoardArray(unsigned size = 0, unsigned base = 0){
        basecol = base;
        baserow = base;
        numcols = size;
        numrows = size;
        data = new int[(size * size + 1)/2](); //////////////////
    }

    ChessBoardArray(const ChessBoardArray &a) {
        data = new int[(a.numrows * a.numcols + 1) / 2](); ///////////////////////////
        baserow = a.baserow;
        basecol = a.basecol;
        numrows = a.numrows;
        numcols = a.numcols;
        for (unsigned i = 0; i < (numrows * numcols + 1) / 2 ; i++) { ///////////////////////
            data[i] = a.data[i];
        }
    }

    ~ChessBoardArray(){
        delete [] data;
    }

    ChessBoardArray &operator=(const ChessBoardArray &a) {
        delete [] data;
        baserow = a.baserow;
        basecol = a.basecol;
        numrows = a.numrows;
        numcols = a.numcols;
        data = new int[(numcols * numrows + 1) / 2]();
        for (unsigned i = 0; i < (numcols * numrows + 1) / 2 ; i++) {//////////////
            data[i] = a.data[i];
        }
        return *this;
    };


    int &select(int i, int j) {
        return data[loc(i, j)];
    }

    int select(int i, int j) const {
        return data[loc(i, j)];
    }

    const Row operator[](int i) {
        return Row(*this, i);
    }

    const ConstRow operator[](int i) const {
        return ConstRow(*this, i);
    }

    friend ostream &operator<<(ostream &out, const ChessBoardArray &a) {
        for (unsigned i = 0; i < a.numrows; i++) {
            for (unsigned j = 0; j < a.numcols; j++) {
                if ((i + j) % 2 == 1) {
                    out << setw(4) << "0";
                }
                else {
                    out << setw(4) << a.select(i + a.baserow, j + a.basecol);
                }
            }
            out << endl;
        }
        /*for(unsigned i = 0; i < (numrows * numcols + 1) / 2; i++) {
            if (numcols % 2 == 1) {
                if ((i + 1) % ((a.numcols+1) / 2) != 0 || i == 0) { //(numcols / 2) != 0 ... << a,data[i] << setw4 << "0";
                    out << setw(4) << a.data[i] << setw(4) << "0";
                }
                else {
                    out << setw(4) << a.data[i] << endl;
                }
            }
        }*/
        return out;
    }

protected:
    int *data;
    unsigned baserow, basecol;
    unsigned numrows, numcols;

    unsigned loc(int i, int j) const {   /*throw (out_of_range)*/
        int di = i - baserow, dj = j - basecol;
        if (di < 0 || dj < 0 || di >= numrows || dj >= numcols || (di + dj) % 2 == 1) {
            throw out_of_range("invalid");
        }
        else {
            //return (di * numcols) / 2 + di % 2 + (dj /2)*(numcols%2);
            return (di * numcols + dj) / 2;
        }
    }
};

int main()
{
    ChessBoardArray a(5, 1); // size 4x4, rows and columns numbered from 1
    a[3][1] = 42;
    a[4][4] = 17;
    try { a[2][1] = 7; }
    catch (out_of_range &e) { cout << "a[2][1] is black" << endl; }
    try { cout << a[1][2] << endl; }
    catch (out_of_range &e) { cout << "and so is a[1][2]" << endl; }
    cout << a;
    return 0;
}