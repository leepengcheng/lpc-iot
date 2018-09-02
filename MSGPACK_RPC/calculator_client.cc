#include <iostream>
#include "rpc/msgpack.hpp"
#include "rpc/client.h"
#include "rpc/rpc_error.h"
#include "string"

using namespace std;
struct IMG
{
    int width;
    int height;
    MSGPACK_DEFINE_MAP(width, height);
};
int main() {
    rpc::client c("localhost", rpc::constants::DEFAULT_PORT);

    try {
        // IMG img;
        // img.width=400;
        // img.height=600;
        // std::cout << "detect() = ";
		// string five = c.call("detect", img).as<string>();
        // std::cout << five << std::endl;


        std::cout << "add() = ";
		double five = c.call("add", 3,3).as<double>();
        std::cout << five << std::endl;

        std::cout << "sub(3, 2) = ";
        double one = c.call("sub", 3, 2).as<double>();
        std::cout << one << std::endl;

        std::cout << "mul(5, 0) = ";
        double zero = c.call("mul", 5, 0).as<double>();
        std::cout << zero << std::endl;

        std::cout << "div(3, 0) = ";
        double hmm = c.call("div", 3, 0).as<double>();
        std::cout << hmm << std::endl;
    } catch (rpc::rpc_error &e) {
        std::cout << std::endl << e.what() << std::endl;
        std::cout << "in function '" << e.get_function_name() << "': ";

        using err_t = std::tuple<int, std::string>;
        auto err = e.get_error().as<err_t>();
        std::cout << "[error " << std::get<0>(err) << "]: " << std::get<1>(err)
                  << std::endl;
        return 1;
    }

    return 0;
}
