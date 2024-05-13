#ifndef SMART_LUMBER_HPP_INCLUDED
#define SMART_LUMBER_HPP_INCLUDED

#include<stddef.h>
#include "matrix.hpp"
#include <iostream>
#include <tuple>
#include <random>
#include <mutex>
#include <vector>

struct TableCell
{
    size_t value_;
    std::tuple<size_t, size_t> prev1_;
    std::tuple<size_t, size_t> prev2_;
    bool sell_;

    TableCell() = default;
    TableCell(size_t value, std::tuple<size_t, size_t> prev1, std::tuple<size_t, size_t> prev2, bool sell);
    // Sell case: current cell location is prev1 and prev2.

};

struct Cut
{
    std::tuple<size_t, size_t> start_;
    std::tuple<size_t, size_t> cellCoords_;
    // TableCell cell_;
    Cut() = default;
    Cut(std::tuple<size_t, size_t> start, std::tuple<size_t, size_t> cellCoord);
};

// Solves the smart lumber problem. M rows and N columns
Matrix<TableCell> smartLumber(size_t n, size_t m, Matrix<size_t>& prices, std::mt19937& rng);
void  findPoints(Matrix<TableCell> &dpTable);

void verticalCut(Matrix<TableCell> &dpTable, size_t row, size_t col, std::vector<TableCell> *tiedCells, size_t *maxValue); //, std::mutex& tie_lock);
void horizontalCut(Matrix<TableCell>& dpTable, size_t row, size_t col, std::vector<TableCell>* tiedCells, size_t* maxValue);//, std::mutex& tie_lock);


std::ostream &operator<<(std::ostream &os, const TableCell &tc);

#endif