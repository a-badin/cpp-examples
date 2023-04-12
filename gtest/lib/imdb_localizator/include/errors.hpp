#pragma once
#include <stdexcept>

class Error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class FileError : public Error {
public:
    FileError(const std::string& what, const std::string& filename)
        : Error{what},
          filename_{filename} {}
    
    const std::string& getFilename() const {
        return filename_;
    }

private:
    std::string filename_;
};

class ConversionError : public Error {
public:
    ConversionError(const std::string& what, const std::string& input = {})
        : Error{what},
          input_{input} {}
    
    const std::string& getInputString() const {
        return input_;
    }

private:
    std::string input_;
};

class ParseError : public Error {
public:
    ParseError(const std::string& what, size_t lineNumber)
        : Error{what},
          lineNumber_{lineNumber} {}
    
    size_t getLineNumber() const {
        return lineNumber_;
    }

private:
    size_t lineNumber_;
};

class ColumnError : public Error {
public:
    ColumnError(const std::string& what, size_t columnNumber)
        : Error{what},
          columnNumber_{columnNumber} {}

    size_t getColumnNumber() const {
        return columnNumber_;
    }
    
private:
    size_t columnNumber_;
};

class HeaderError : public Error {
public:
    HeaderError(const std::string& what, const std::string& columnName)
        : Error{what},
          columnName_{columnName} {}

    const std::string& getColumnName() const {
        return columnName_;
    }
    
private:
    std::string columnName_;
};
