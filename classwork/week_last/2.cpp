#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

class numfilterbuf : public std::streambuf {
private:
    std::istream *in;
    std::ostream *out;
    
    int cur; 
    
protected:    
    virtual int overflow(int c) override {
        if (c != traits_type::eof()) {
            char ch = static_cast<char>(c);
            if (std::isdigit(ch) || ch == ' ') {
                if (out) {
                    *out << ch;
                    out->flush();
                }
                return c;
            }
        }
        return traits_type::eof();
    }
        
    virtual int uflow() override {
        int result = underflow();
        if (result != traits_type::eof()) {
            cur = traits_type::eof(); 
        }
        return result;
    }
    
    virtual int underflow() override {
        if (cur != traits_type::eof()) 
            return cur;
        
        
        if (in) {
            while (true) {
                int ch = in->get();
                if (ch == traits_type::eof()) {
                    cur = traits_type::eof();
                    return traits_type::eof();
                }
                
                char c = static_cast<char>(ch);
                if (std::isdigit(c) || c == ' ') {
                    cur = ch; 
                    return ch;
                }
            }
        }
        
        return traits_type::eof();
    }
    
public:
    numfilterbuf(std::istream &_in, std::ostream &_out)
        : in(&_in), out(&_out), cur(traits_type::eof())
    {}
};

int main(int argc, char **argv) {
    const char str1[] = "In 4 bytes contains 32 bits";
    const char str2[] = "Unix time starts from Jan 1, 1970";
    std::istringstream str(str1);
    
    numfilterbuf buf(str, std::cout); 
    std::iostream numfilter(&buf); 
    
    std::string val;
    std::getline(numfilter, val);
    numfilter.clear();
    
    std::cout << "Original: '" << str1 << "'" << std::endl;
    std::cout << "Read from numfilter: '" << val << "'" << std::endl;
    
    std::cout << "Original: '" << str2 << "'" << std::endl;
    std::cout << "Written to numfilter: '";
    numfilter << str2;
    std::cout << "'" << std::endl;
    
    return 0;
}