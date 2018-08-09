#include <b0/node.h>
#include <b0/publisher.h>
#include <b0/subscriber.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#ifdef _WIN32
#include <Windows.h>
#endif




#ifdef _WIN32
#define SLEEP_MS(x) Sleep(x)
#else
#define SLEEP_MS(x) usleep(x*1000)
#endif


b0::Node* node=NULL;
//int sensorTrigger=0;

void sensorCallback(const std::string &sensTrigger_packedInt)
{
//    sensorTrigger=((int*)sensTrigger_packedInt.c_str())[0];
    printf("%s\n",sensTrigger_packedInt.c_str());
}


int main(int argc,char* argv[])
{

    // Create a B0 node.
    b0::Node _node("subNode");
    node=&_node;

    // 1. Let's subscribe to the sensor, simulation time and pause flag stream:
    b0::Subscriber sub_node(node,"topic1_string",&sensorCallback);

    // 2. Let's prepare publishers for the motor speeds:
//    b0::Publisher pub_node(node,"topic1_string");

    node->init();


//    std::string buff;
//    double dval=1234.0;
    while (!node->shutdownRequested())
    {
//        dval=dval+1.0;
//        buff=(char*)(&dval);
//        pub_node.publish("hello world123");

        // handle B0 messages:
        node->spinOnce();

        // sleep a bit:
        SLEEP_MS(20);
    }
    node->cleanup();
    printf("test just ended!\n");
    return(0);
}

