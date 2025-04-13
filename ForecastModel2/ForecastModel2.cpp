#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;

// Function to read CSV data
vector<vector<string>> readCSV(const string& filename) {
    ifstream file(filename);
    vector<vector<string>> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    return data;
}

// Function to parse a string to double, handling non-numeric values
double parseValue(const string& value) {
    try {
        return stod(value);
    }
    catch (const invalid_argument&) {
        return numeric_limits<double>::quiet_NaN();
    }
}

// Linear regression function to calculate slope and intercept
pair<double, double> linearRegression(const vector<int>& years, const vector<double>& values) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    int n = years.size();

    for (int i = 0; i < n; i++) {
        sumX += years[i];
        sumY += values[i];
        sumXY += years[i] * values[i];
        sumX2 += years[i] * years[i];
    }

    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    return { slope, intercept };
}

// Forecast function using linear regression
vector<double> forecast(const vector<int>& years, const vector<double>& values, int startYear, int endYear) {
    pair<double, double> regression = linearRegression(years, values);
    double slope = regression.first;
    double intercept = regression.second;

    vector<double> predictions;
    for (int year = startYear; year <= endYear; year++) {
        double forecastedValue = slope * year + intercept;
        predictions.push_back(forecastedValue);
    }
    return predictions;
}

// Write forecasted data to a CSV
void writeCSV(const string& filename, const vector<string>& header, const vector<int>& forecastedYears, const vector<vector<double>>& forecastedValues) {
    ofstream file(filename);
    file << "Year,";
    for (const auto& col : header) {
        file << col << ",";
    }
    file << endl;

    for (size_t i = 0; i < forecastedYears.size(); i++) {
        file << forecastedYears[i];
        for (const auto& col : forecastedValues) {
            file << "," << fixed << setprecision(2) << col[i];
        }
        file << endl;
    }
}

int main() {
    // Read the data from the CSV
    vector<vector<string>> data = readCSV("input_data.csv.txt");

    // Get the header and the data for each variable
    vector<string> header = data[0];
    vector<vector<double>> values;
    for (size_t col = 1; col < header.size(); col++) {
        vector<double> columnData;
        for (size_t row = 1; row < data.size(); row++) {
            columnData.push_back(parseValue(data[row][col]));
        }
        values.push_back(columnData);
    }

    // The years to forecast
    vector<int> forecastYears = { 2025, 2026, 2027, 2028, 2029, 2030 };

    // Forecast for each variable (from 2025 to 2030)
    vector<vector<double>> forecastedValues;
    for (size_t i = 0; i < values.size(); i++) {
        vector<double> forecasted = forecast(forecastYears, values[i], 2025, 2030);
        forecastedValues.push_back(forecasted);
    }

    // Write the forecasted data to a new CSV
    writeCSV("forecasted_data.csv", header, forecastYears, forecastedValues);

    cout << "Forecasting complete. Check the 'forecasted_data.csv' file." << endl;
    return 0;
}
