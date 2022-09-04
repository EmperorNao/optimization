#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H


#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include <map>


namespace config {

    template<typename T>
    struct identity { typedef T type; };


    static inline void ltrim(std::string &s);
    static inline void rtrim(std::string &s);
    static inline void trim(std::string &s);

    void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    class Config {

    private:
        std::map<std::string, std::string> content;
        const std::string delimiter;

        template <typename T>
        [[nodiscard]] T _get(std::string key, identity<T>) const {

            return static_cast<T>(this->content);

        }

        [[nodiscard]] std::string _get(std::string key, identity<std::string>) const {
            return this->content.at(key);
        }

        [[nodiscard]] bool _get(std::string key, identity<bool>) const {
            return this->content.at(key) == "true";
        }

        [[nodiscard]] int64_t _get(std::string key, identity<int64_t>) const {
            return std::stoll(this->content.at(key));
        }

        [[nodiscard]] int _get(std::string key, identity<int>) const {
            return std::stoi(this->content.at(key));
        }

        [[nodiscard]] uint64_t _get(std::string key, identity<uint64_t>) const {
            return std::stoull(this->content.at(key));
        }

        [[nodiscard]] uint _get(std::string key, identity<uint>) const {
            return std::stoul(this->content.at(key));
        }

        [[nodiscard]] double _get(std::string key, identity<double>) const {
            return std::stod(this->content.at(key));
        }


    public:

        Config(const std::string _delimiter=":") : delimiter(_delimiter) {}

        void load(std::string filename) {

            std::ifstream file(filename);
            std::string line;

            int number_of_line = 0;
            while (std::getline(file, line)) {
                if (line.empty() || line.starts_with('#')) {
                    continue;
                }
                number_of_line += 1;

                uint64_t pos = line.find(delimiter);
                if (pos == std::string::npos) {
                    throw std::domain_error(
                            "Config file " + filename +
                            " don't match for delimiter '" + delimiter +
                            "' in line " + std::to_string(number_of_line)
                    );
                }

                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1, line.size() - pos - 1);
                trim(key);
                trim(value);

                this->content[key] = value;

            }

        }

        void save(std::string filename, bool append) const {

            std::ofstream file;
            if (append) {
                file.open(filename, std::ios::app);
            }
            else {
                file.open(filename);
            }

            for (auto& pair: content) {
                file << pair.first << delimiter << pair.second << std::endl;
            }

        }

        void merge(const Config& conf, bool allow_override) {
            throw std::domain_error("merge not implemented yet");
            return;
        }

        void refresh() { this->content.clear(); }
        bool is_exist(std::string key) const { return this->content.contains(key); }
        void set(std::string key, std::string value) { this->content[key] = value; }

        template <typename T>
        [[nodiscard]] T get(std::string key) const {

            return this->_get(key, identity<T>());

        };

        template <typename T>
        [[nodiscard]] T get(std::string key, T default_value) const {

            if (this->is_exist(key)) {
                return get<T>(key);
            }
            else {
                return default_value;
            }

        }

    }; // class Config end

} // namespace config end


#endif //CONFIG_CONFIG_H