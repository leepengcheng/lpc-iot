#include <iostream>
#include "image.h"
#include "rpc/client.h"
#include "rpc/rpc_error.h"
#include "string"
#include <opencv2/opencv.hpp>

int main() {
    rpc::client c("localhost", rpc::constants::DEFAULT_PORT);

    try {
        // std::cout << "add() = ";
        // double five = c.call("add", 3, 3).as<double>();
        // std::cout << five << std::endl;

        // std::cout << "sub(3, 2) = ";
        // double one = c.call("sub", 3, 2).as<double>();
        // std::cout << one << std::endl;

        // std::cout << "mul(5, 0) = ";
        // double zero = c.call("mul", 5, 0).as<double>();
        // std::cout << zero << std::endl;

        // std::cout << "div(3, 0) = ";
        // double hmm = c.call("div", 3, 0).as<double>();
        // std::cout << hmm << std::endl;

        cv::Mat img = cv::imread("/media/lee/workspace/GitWorkSpace/lpc-dnn/object-detection/yolo/imgs/img3.jpg",
                                 cv::IMREAD_UNCHANGED);
        int width = img.size().width;
        int height = img.size().height;
        Image img_msg;
        img_msg.width = width;
        img_msg.height = height;
        std::vector<uchar> buffer(img.data, img.data + width * height * 3);
        img_msg.data = buffer;


        auto result_obj = c.async_call("get_image",img_msg);
        auto result = result_obj.get().as<Image>();
        img.data=result.data.data();

        cv::imshow("img", img);
        cv::waitKey(0);
    }

    catch (rpc::rpc_error &e) {
        std::cout << std::endl
                  << e.what() << std::endl;
        std::cout << "in function '" << e.get_function_name() << "': ";

        using err_t = std::tuple<int, std::string>;
        auto err = e.get_error().as<err_t>();
        std::cout << "[error " << std::get<0>(err) << "]: " << std::get<1>(err)
                  << std::endl;
        return 1;
    }

    return 0;
}
