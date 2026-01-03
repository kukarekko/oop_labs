#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

//ошибки
class CSVError : public std::runtime_error {
public:
    explicit CSVError(const std::string& msg) : std::runtime_error(msg) {}
};

inline void reportParseError(size_t row, size_t col, const std::string& msg) {
    std::ostringstream oss;
    oss << "Parse error at row " << row << ", column " << col << ": " << msg;
    throw CSVError(oss.str());
}

//конфигурация
    struct CSVSettings {
    char fieldSeparator = ',';
    char lineSeparator = '\n';
    char quote = '"';

    CSVSettings() = default;
    CSVSettings(char f, char l, char q = '"')
        : fieldSeparator(f), lineSeparator(l), quote(q) {}
};

//преобразование строки в tuple
template<int Index, typename Head, typename... Tail>
struct RowConverter {
    static std::tuple<Head, Tail...>
    convert(const std::vector<std::string>& cells, size_t row) {
        constexpr size_t cols = 1 + sizeof...(Tail);//общее количество столбцов
        Head value;
        std::stringstream ss(cells[Index]);
        ss >> value;

        if (ss.fail() || !ss.eof()) {
            reportParseError(row, cols - Index,
                             "Failed to convert \"" + cells[Index] + "\"");
        }

        return std::tuple_cat(
            std::make_tuple(value),
            RowConverter<Index - 1, Tail...>::convert(cells, row));
    }
};

template<int Index, typename... Tail>
struct RowConverter<Index, std::string, Tail...> {
    static std::tuple<std::string, Tail...>
    convert(const std::vector<std::string>& cells, size_t row) {
        return std::tuple_cat(
            std::make_tuple(cells[Index]),
            RowConverter<Index - 1, Tail...>::convert(cells, row));
    }
};

template<typename Head, typename... Tail>
struct RowConverter<0, Head, Tail...> {
    static std::tuple<Head, Tail...>
    convert(const std::vector<std::string>& cells, size_t row) {
        Head value;
        std::stringstream ss(cells[0]);
        ss >> value;

        if (ss.fail() || !ss.eof()) {
            reportParseError(row, 1,
                             "Failed to convert \"" + cells[0] + "\"");
        }

        return std::make_tuple(value);
    }
};

template<typename... Tail>
struct RowConverter<0, std::string, Tail...> {
    static std::tuple<std::string, Tail...>
    convert(const std::vector<std::string>& cells, size_t) {
        return std::make_tuple(cells[0]);
    }
};

template<typename... Types>
class CSVParser {
private:
    std::istream& stream;
    CSVSettings settings;
    size_t currentRow = 0;
    bool eofReached = false;

public:
    explicit CSVParser(std::istream& s, size_t skip = 0)
        : stream(s) {
        skipLines(skip); //конструктор с пропуском строк
    }

    explicit CSVParser(std::istream& s, CSVSettings cfg, size_t skip = 0)
        : stream(s), settings(cfg) {
        skipLines(skip); // конструктор с настройками
    }

    class Iterator {
        CSVParser* reader; //указатель на парсер
        std::tuple<Types...> value;
        bool finished;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::tuple<Types...>;

        Iterator(CSVParser* r, bool end = false)
            : reader(r), finished(end) {
            if (!finished) ++(*this); //сразу читаем 1 строку
        }

        Iterator& operator++() {
            if (finished) return *this;
            try {
                value = reader->readRow();
            } catch (const CSVError& e) {
                if (std::string(e.what()).find("End of file") != std::string::npos)
                    finished = true;
                else
                    throw;
            }
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return finished == other.finished;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        const std::tuple<Types...>& operator*() const {
            return value;
        }
    };

    Iterator begin() { return Iterator(this); }
    Iterator end() { return Iterator(this, true); }

private:
    void skipLines(size_t n) {
        for (size_t i = 0; i < n; ++i) readLine();
    }

    std::tuple<Types...> readRow() {
        std::string line = readLine();
        if (line.empty() && eofReached)
            throw CSVError("End of file");

        ++currentRow;
        auto fields = split(line); //разбиваем строку на ячейки

        //проверяем количество столбцов
        if (fields.size() != sizeof...(Types))
            throw CSVError("Column count mismatch in row " +
                           std::to_string(currentRow));

        std::reverse(fields.begin(), fields.end());
        return RowConverter<sizeof...(Types) - 1, Types...>::convert(fields, currentRow);
    }

    std::string readLine() {
        std::string line;
        if (!std::getline(stream, line)) {
            eofReached = true;
        }
        return line;
    }

    std::vector<std::string> split(const std::string& line) { //разбивает строку на ячейки
        std::vector<std::string> cells;
        std::string cell;
        bool quoted = false;

        for (char c : line) {
            if (c == settings.quote) {
                quoted = !quoted;
            } else if (c == settings.fieldSeparator && !quoted) {
                //если встретили разделитель не внутри кавычек
                cells.push_back(cell);
                cell.clear();
            } else {
                cell += c;
            }
        }
        cells.push_back(cell);
        return cells;
    }
};

#endif
