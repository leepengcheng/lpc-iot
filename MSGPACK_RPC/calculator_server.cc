#include <iostream>
#include "image.h"
#include "rpc/server.h"
#include "rpc/this_handler.h"
#include <opencv2/opencv.hpp>

double divide(double a, double b) {
    if (b == 0.0) {
        rpc::this_handler().respond_error(
                    std::make_tuple(1, "Division by zero"));
    }
    return a / b;
}

struct subtractor {
    double operator()(double a, double b) {
        return a - b;
    }
};

struct multiplier {
    double multiply(double a, double b) {
        return a * b;
    }
};

int main() {
    rpc::server srv(rpc::constants::DEFAULT_PORT);
    subtractor s;
    multiplier m;

    cv::namedWindow("RPC");

    // It's possible to bind non-capturing lambdas
    srv.bind("add", [](double a, double b) { return a + b; });
    // ... arbitrary callables
    srv.bind("sub", s);
    // ... free functions
    srv.bind("div", &divide);
    // ... member functions with captured instances in lambdas
    srv.bind("mul", [&m](double a, double b) { return m.multiply(a, b); });



    srv.bind("get_image", [&](Image &data) {
        std::cout<<"receive msg:"<<data.width<<" "<<data.height<<std::endl;

         for (int x = 0; x < data.width; x++) {
             for (int y = 0; y < data.height; y++) {
                 data.data[x*data.width+y]=255;
             }
         }
        return data;
    });

    srv.bind("get_halcon_image", [&](HalconImageMsg &data) {
        std::cout<<"receive msg:"<<data.width<<" "<<data.height<<std::endl;
        cv::Mat img(data.height,data.width,CV_8UC3);
        for (int x = 0; x < data.width; x++) {
            for (int y = 0; y < data.height; y++) {
                img.at<cv::Vec3b>(y,x)[0]=data.b[y*data.width+x];
                img.at<cv::Vec3b>(y,x)[1]=data.g[y*data.width+x];
                img.at<cv::Vec3b>(y,x)[2]=data.r[y*data.width+x];
            }
        }
        cv::imshow("RPC",img);
//        cv::imwrite("img.png",img);
//        std::cout<<"imwrite msg:img.png"<<std::endl;
//        return 10086;
    });

    srv.run();

    return 0;
}
