#include "CSVLogger.h"

#include "Logging.h"

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

void CSVLogger::addColumn(std::string column) {
    columns.emplace_back(column, 0.f);
}

void CSVLogger::setValue(size_t columnIndex, float value) {
    if (columnIndex >= columns.size()) {
        GCS_APP_LOG_ERROR("CSVLogger: Couldn't set column value, column index ({}) out of bounds.", columnIndex);
        return;
    }

    columns[columnIndex].currentValue = value;
}

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

bool CSVLogger::fileIsOpen() {
    return file.is_open();
}
