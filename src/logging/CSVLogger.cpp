#include "CSVLogger.h"

#include "Logging.h"

/**
 * @brief Opens the file in which the CSV data will be logged.
 * @param filename The name of the file to open.
 */
void CSVLogger::openFile(std::string filename) {
    if (file.is_open()) {
        file.close();
    }

    file.open(filename, std::ios::out);
    if (!file.is_open()) {
        GCS_APP_LOG_ERROR("CSVLogger: Couldn't open file \"{}\".", filename);
    }

    firstLog = true;
    currentFileName = filename;
}

/**
 * @brief Adds a new column to the CSV file.
 * @param column The name of the column to add.
 */
void CSVLogger::addColumn(std::string column) {
    columns.emplace_back(column, 0.f);
}

/**
 * @brief Sets the value of a specific column.
 * @param columnIndex The index of the column to set the value for.
 * @param value The value to set for the column.
 */
void CSVLogger::setValue(size_t columnIndex, float value) {
    if (columnIndex >= columns.size()) {
        GCS_APP_LOG_ERROR("CSVLogger: Couldn't set column value, column index ({}) out of bounds.", columnIndex);
        return;
    }

    columns[columnIndex].currentValue = value;
}

/**
 * @brief Logs the current values of all columns to the CSV file.
 * If the file is not open or no columns have been added, it logs an error or warning.
 */
void CSVLogger::log() {
    if (!file.is_open()) {
        GCS_APP_LOG_ERROR("CSVLogger: Can't log in file \"{}\", file isn't open.", currentFileName);
        return;
    }

    if (columns.empty()) {
        GCS_APP_LOG_WARN("CSVLogger: Tried to log in file \"{}\", but no columns have been created.", currentFileName);
        return;
    }

    if (firstLog) {
        for (size_t i = 0; i < columns.size() - 1; i++) {
            file << columns[i].name << ",";
        }
        file << columns.back().name << '\n';
        firstLog = false;
    }

    for (size_t i = 0; i < columns.size() - 1; i++) {
        file << columns[i].currentValue << ",";
    }
    file << columns.back().currentValue << '\n';
    firstLog = false;

    file.flush();
}

/**
 * @brief Checks if the CSV file is currently open.
 * @returns True if the file is open, false otherwise.
 */
bool CSVLogger::fileIsOpen() const {
    return file.is_open();
}
