#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>


// к заданию 1

template<typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(
    ForwardIterator first, ForwardIterator last, UnaryPredicate pred)
{
    ForwardIterator max_element = last;
    
    for (auto it = first; it != last; ++it) 
        if (pred(*it)) {
            max_element = it;
            break;
        }
    
    if (max_element == last) return last;
    
    for (auto it = std::next(max_element); it != last; ++it)
        if (pred(*it) && *max_element < *it) 
            max_element = it;
    
    return max_element;
}



void TestUniqueMax()
{
    auto IsEven = [](int x) 
    {
        return x % 2 == 0;
    };

    const std::list<int> hill{ 2, 4, 8, 9, 6, 4, 2 };
    auto max_iterator = hill.begin();
    std::advance(max_iterator, 2);

    std::vector<int> numbers(10);
    std::iota(numbers.begin(), numbers.end(), 1);

    // check if 
    // max_element_if(numbers.begin(), numbers.end(), IsEven) == --numbers.end()
    assert(max_element_if(numbers.begin(), numbers.end(), IsEven) == --numbers.end());

    // check if
    // max_element_if(hill.begin(), hill.end(), IsEven) == max_iterator
    assert(max_element_if(hill.begin(), hill.end(), IsEven) == max_iterator);
}

void TestSeveralMax() 
{
    struct IsCapitalized 
    {
        bool operator()(const std::string& s)
        {
            return !s.empty() && std::isupper(s.front());
        }
    };

    const std::forward_list<std::string> text
    { "One", "two", "Three", 
      "One", "Two", "Three", 
      "one", "Two", "three" 
    };
    auto max_iterator = text.begin();
    std::advance(max_iterator, 4);

    // check if 
    // max_element_if(text.begin(), text.end(), IsCapitalized()) == max_iterator
    assert(max_element_if(text.begin(), text.end(), IsCapitalized()) == max_iterator);
}

void TestNoMax() 
{
    const std::vector<int> empty;
    const std::string str = "Non-empty string";

    auto AlwaysTrue = [](int) 
    {
        return true;
    };
    // check if
    // max_element_if(empty.begin(), empty.end(), AlwaysTrue) == empty.end()
    assert(max_element_if(empty.begin(), empty.end(), AlwaysTrue) == empty.end());

    auto AlwaysFalse = [](char)
    {
        return false;
    };
    // check if 
    // max_element_if(str.begin(), str.end(), AlwaysFalse) == str.end() 
    assert(max_element_if(str.begin(), str.end(), AlwaysFalse) == str.end());
}



// к заданию 2

template<typename ForwardIterator, typename UnaryFunction>
void transform_inplace(ForwardIterator first, ForwardIterator last, UnaryFunction func)
{
    for (auto it = first; it != last; ++it) 
        func(*it);
}

void to_uppercase_inplace(char& c)
{
    c = toupper(c);
}
void transform_example(std::string world)
{
    transform_inplace(world.begin(), world.end(), to_uppercase_inplace);
    std::cout << "world = " << std::quoted(world) << '\n';
}



int main(){
    TestUniqueMax();
    TestNoMax();
    TestSeveralMax();
    transform_example("abcdef");
    
    return 0;
}