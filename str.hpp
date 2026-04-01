#include <algorithm>
#include <cstring>
#include <vector>
#include <cstddef>

class str
{
private:
    char* data_;
    std::size_t len_;

    void allocate_and_copy(const char* s, std::size_t n) {
        data_ = new char[n + 1];
        if (n > 0 && s) {
            std::memcpy(data_, s, n);
        }
        data_[n] = '\0';
        len_ = n;
    }

public:
    // Default constructor: empty string
    str() : data_(nullptr), len_(0) {
        allocate_and_copy("", 0);
    }

    // Construct from single char
    str(const char &c) : data_(nullptr), len_(0) {
        allocate_and_copy(&c, 1);
    }

    // Construct from const char* (rvalue ref as per spec, but we copy)
    str(const char *&& s_) : data_(nullptr), len_(0) {
        const char* s = s_ ? s_ : "";
        allocate_and_copy(s, std::strlen(s));
    }

    // Assign from const char* (rvalue ref as per spec, but we copy)
    str &operator=(const char *&& s_) {
        const char* s = s_ ? s_ : "";
        if (data_ == s) return *this; // unusual self-assign guard
        delete[] data_;
        allocate_and_copy(s, std::strlen(s));
        return *this;
    }

    // Copy constructor
    str(const str &other) : data_(nullptr), len_(0) {
        allocate_and_copy(other.data_, other.len_);
    }

    // Copy assignment
    str &operator=(const str &other) {
        if (this == &other) return *this;
        delete[] data_;
        allocate_and_copy(other.data_, other.len_);
        return *this;
    }

    // Index operator (0-based). No bounds checking
    char &operator[](size_t pos) {
        return data_[pos];
    }

    // Length
    size_t len() const {
        return len_;
    }

    // Join a list of strs using this as separator
    str join(const std::vector<str> &strs) const {
        if (strs.empty()) {
            return str();
        }
        std::size_t total = 0;
        for (const auto &s : strs) total += s.len_;
        std::size_t sep_len = len_;
        if (strs.size() > 1) total += sep_len * (strs.size() - 1);

        str result;
        delete[] result.data_;
        result.data_ = new char[total + 1];
        result.len_ = total;
        std::size_t pos = 0;
        for (std::size_t i = 0; i < strs.size(); ++i) {
            if (strs[i].len_ > 0) {
                std::memcpy(result.data_ + pos, strs[i].data_, strs[i].len_);
                pos += strs[i].len_;
            }
            if (i + 1 < strs.size() && sep_len > 0) {
                std::memcpy(result.data_ + pos, data_, sep_len);
                pos += sep_len;
            }
        }
        result.data_[pos] = '\0';
        return result;
    }

    // Slice [l, r)
    str slice(size_t l, size_t r) const {
        if (l > r) std::swap(l, r);
        l = std::min(l, len_);
        r = std::min(r, len_);
        if (l >= r) {
            return str();
        }
        std::size_t n = r - l;
        str result;
        delete[] result.data_;
        result.data_ = new char[n + 1];
        result.len_ = n;
        std::memcpy(result.data_, data_ + l, n);
        result.data_[n] = '\0';
        return result;
    }

    // Destructor
    ~str() {
        delete[] data_;
        data_ = nullptr;
        len_ = 0;
    }
};
