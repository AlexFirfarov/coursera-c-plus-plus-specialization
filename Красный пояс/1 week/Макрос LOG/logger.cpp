#include <string>
#include <ostream>

class Logger {
public:
    explicit Logger(std::ostream& output_stream) : os(output_stream) {}

    void SetLogLine(bool value) { log_line = value; }
    void SetLogFile(bool value) { log_file= value; }

    void Log(const std::string& message);

    void SetFile(const std::string& filename) {
        file = filename;
    }

    void SetLine(size_t line_num) {
        line = line_num;
    }

private:
    std::ostream& os;
    bool log_line = false;
    bool log_file = false;
    std::string file;
    size_t line;
};

void Logger::Log(const std::string& message) {
    if (log_file && log_line) {
        os << file << ':' << line << ' ';
    } else if (log_file) {
        os << file << ' ';
    } else if (log_line) {
        os << "Line " << line << ' ';
    }
    os << message << '\n';
}

#define LOG(logger, message) \
    logger.SetFile(__FILE__);  \
    logger.SetLine(__LINE__);  \
    logger.Log(message);
