#include <string>
#include <iostream>
#include <glog/logging.h>
#include "../build/test.pb.h"

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;

    test::man man;
    man.set_age(18);
    man.set_name("Tom");

    std::string str;
    man.SerializeToString(&str);
    LOG(INFO) << str;

    test::man man2;
    man2.ParseFromString(str);
    LOG(INFO) << man2.age();
    LOG(INFO) << man2.name();

    return 0;
}