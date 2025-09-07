#include "ans_handler.hpp"
#include "my_math.hpp"
#include <iostream>

int main(){
    double ang;
    AnswerHandler ans;
    std::cin >> ang;
    ans = my_sin(ang);
    std::cout << std::endl << ans.x << ' ' << ans.rez << std::endl;
    return 0;
}