#include<stddef.h>
#include <string>
#include "matrix.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <stack>
#include "smart-lumber.hpp"

using namespace std;

// Constructor for the Table Cell class
TableCell::TableCell(size_t value, tuple<size_t, size_t> prev1, tuple<size_t, size_t> prev2):
    value_{value}, prev1_{prev1}, prev2_{prev2}{}

// For printing 
std::ostream& operator<<(ostream& os, const TableCell& tc){
    const auto &[p1row, p1col] = tc.prev1_;
    const auto &[p2row, p2col] = tc.prev2_;

    os << "[" << tc.value_ << ", (" << p1row << ", " << p1col << "), ("
       << p2row << ", " << p2col << ")]";
    return os;
}

// Constructor for the Cut classs
Cut::Cut(tuple<size_t, size_t> start, std::tuple<size_t, size_t> cellCoords):
    start_{start}, cellCoords_{cellCoords}{}


/**
 * @brief Solves the Smart Lumber Problem
 * 
 * @param n length of the wood (number of columns in table)
 * @param m width of the wood (number of rows)
 * @param prices Table of prices to sell a given piece of wood at
 * @return Matrix<TableCell> Returns the DP table.  
 */
Matrix<TableCell> smartLumber(size_t n, size_t m, Matrix<size_t>& prices, mt19937& rng){
    // Initialize DP table and fill in base case
    Matrix<TableCell> dpTable(m, n);
    dpTable(0, 0) = TableCell{prices(0, 0), {0,0}, {0,0}};

    // Threading Mutex
    std::mutex tie_mutex;

    // Valid Selling sizes
    size_t k = prices.numRows();
    size_t l = prices.numCols();

    // Set up tiebreaker distribution and fill in recursive cases
    std::uniform_int_distribution<size_t> cellDist(0, 0xFFFFFFFFFFFFFFFF);
    for (size_t row = 0; row < m; ++row) {
        for (size_t col = 0; col < n; ++col){
            if (row == 0 and col == 0) {
                continue; // base case
            }
            vector<TableCell> tiedCells;
            size_t maxValue = 0;

            if (row < k and col < l) { // sell case
                maxValue = prices(row, col);
                tiedCells.push_back(TableCell{maxValue, {row, col}, {row, col}});
            }
            // Check all horizontal or vertical cuts
            verticalCut(dpTable, row, col, &tiedCells, &maxValue);
            horizontalCut(dpTable, row, col, &tiedCells, &maxValue);

            size_t tiedCellCount = tiedCells.size();
            dpTable(row, col) = tiedCells[cellDist(rng) % tiedCellCount];
        }
    }

    return dpTable; 
}

void verticalCut(Matrix<TableCell>& dpTable, size_t row, size_t col, std::vector<TableCell>* tiedCells, size_t* maxValue){
    for (size_t vcut = 1; vcut < col; ++vcut)
    {
        size_t newValue = dpTable(row, vcut-1).value_ + dpTable(row, col - vcut).value_;
        if (newValue > *maxValue){
            // const lock_guard<std::mutex> foundTie(tie_mutex);
            tiedCells->clear();
            tiedCells->push_back(TableCell{newValue, {row, vcut - 1}, {row, col - vcut}});
            *maxValue = newValue;
        }
        else if (newValue == *maxValue)
        {
            // const lock_guard<std::mutex> foundTie(tie_mutex);
            tiedCells->push_back(TableCell{*maxValue, {row, vcut - 1}, {row, col - vcut}});
        }
    }
}

void horizontalCut(Matrix<TableCell>& dpTable, size_t row, size_t col, std::vector<TableCell>* tiedCells,size_t* maxValue) {
    for (size_t hcut = 1; hcut < row; ++hcut)
    {
        size_t newValue = dpTable(hcut-1,col).value_ + dpTable(row - hcut,col).value_;
        if (newValue > *maxValue){
            tiedCells->clear();
            tiedCells->push_back(TableCell{newValue,{hcut-1,col},{row - hcut,col}});
            *maxValue = newValue;
        } else if (newValue == *maxValue){
            tiedCells->push_back(TableCell{*maxValue, {hcut-1,col},{row - hcut,col}});
        }
    }
}


// Find points for the line. 
void findPoints(Matrix<TableCell> &dpTable){
    stack<Cut> cutStack;

    // 8 values in this tuple, 4 points
    // Row and col of the start of the cut
    // Row and col of the end of the cut
    // Row and col of upper left of point of the piece
    // Row and col of lower right point of the piece

    // r1, c1, r2, c2, fr1, fc1, fr2, fc2
    vector<tuple<size_t, size_t, size_t, size_t, size_t, size_t, size_t,size_t>> pointsList;
    size_t numRows = dpTable.numRows();
    size_t numCols = dpTable.numCols();

    // Get Wood border
    pointsList.push_back({0, 0, 0, numCols,0,0,0,0});
    pointsList.push_back({0, numCols, numRows, numCols,0,0,0,0});
    pointsList.push_back({numRows, 0, numRows, numCols,0,0,0,0});
    pointsList.push_back({0, 0, numRows, 0,0,0,0,0});

    // Initialize Stack
    cutStack.push({{0, 0}, {numRows-1, numCols-1}});
    while(!cutStack.empty()){
        // Get top cut from stack and pattern match
        Cut c = cutStack.top();
        cutStack.pop();

        auto [cr, cc] = c.cellCoords_; // Cut coordinates
        auto [sr, sc] = c.start_; // Start of the cut
        auto [r1, c1] = dpTable(cr, cc).prev1_;
        auto [r2, c2] = dpTable(cr, cc).prev2_;

        // Sell case. Do nothing
        if (r1 == r2 and c1 == c2 and cr == r1 and cc == c1) { 
            continue;
        }else if ((r1 == r2 and c1 != c2) or (r1 == r2 and c1 == c2 and r1 == cr)){
            // same row = vertical cut
            pointsList.push_back({sr, sc + c1 + 1, sr + r2 + 1, sc + c1 + 1,sr, sc, sr+r1+1, sc+c1+1}); 
            cutStack.push({c.start_, {r1, c1}});
            cutStack.push({{sr, sc + c1 + 1}, {r2,c2}});
        }
        else if ((r1 != r2 and c1 == c2) or (r1 == r2 and c1 == c2 and c1 == cc))
        {
            // same column = horizontal cut
            pointsList.push_back({sr + r1 + 1, sc, sr + r1 + 1, sc + c2 + 1, sr, sc, sr + r1 + 1, sc + c1 + 1});
            cutStack.push({c.start_, {r1, c1}});
            cutStack.push({{sr + r1 + 1, sc}, {r2,c2}});
        } 
        else
        {
            cerr << "Some issue with the cut. Shouldn't reach this!" << endl;
            cerr << dpTable(cr, cc) << endl;
            cerr << cr << " " << cc << endl;
        }
    }
    // Print out all the points. Python reads the stdin through a unix pipe
    for (auto [r1,c1,r2,c2,fr1, fc1, fr2, fc2] : pointsList){
        cout << c1 << " " << r1 << " " << c2 << " " << r2 << " " << fc1 << " " << fr1 <<" " << fc2 <<" "<< fr2  << endl;
    }
}

int main(int argc, char** argv){
    if (argc != 4){
        cerr << "Usage: ./smart-lumber <seed> <length> <width>" << endl;
        exit(0);
    }
    size_t seed = atoll(argv[1]);
    size_t m = atoll(argv[2]); // number of rows
    size_t n = atoll(argv[3]); // number of columns

    // Initialize seed
    if (seed == 0){
        seed = time(0);
    }

    std::mt19937 rng(seed);
    std::normal_distribution<double> distribution(4.0, 0.5);

    // Table proportion is also random.  Between 1/3 and 1/2?
    size_t k = m / 5;
    size_t l = n / 5;
    Matrix<size_t> prices(k, l);
    for (size_t i = 0; i < k; ++i)
    {
        for (size_t j = i; j < l; ++j)
        {
            // Easiest way to build the table to to randomly generate it
            // Scale the area by a random factor to get values
            size_t area = (i + 1) * (j + 1);
            double scale = distribution(rng);
            if (scale < 0.0){
                scale = 0;
            }
            prices(i, j) = area * scale;
            prices(j, i) = area * scale;
        }
    }
    Matrix<TableCell> result = smartLumber(n, m, prices, rng);
    findPoints(result);
    return 0;
}