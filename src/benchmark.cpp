#include <string>
#include <iostream>
#include <benchmark/benchmark.h>
#include <glog/logging.h>
#pragma warning(push)
#pragma warning(disable: 4127)
#include <man.pb.h>
#pragma warning(pop)

static void BM_Glog(benchmark::State& state)
{
    FLAGS_logtostderr = 1;
    for (auto _ : state)
    {
        LOG(INFO) << "";
    }
}
BENCHMARK(BM_Glog);

static void BM_StringCreation(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string empty_string;
    }
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state)
{
    std::string x = "hello";
    for (auto _ : state)
    {
        std::string copy(x);
    }
}
BENCHMARK(BM_StringCopy);

static void BM_Protobuf(benchmark::State& state)
{
    for (auto _ : state)
    {
        test::man man;
        man.set_age(18);
        man.set_name("Tom");

        std::string str;
        man.SerializeToString(&str);

        test::man man2;
        man2.ParseFromString(str);
    }
}
BENCHMARK(BM_Protobuf);

BENCHMARK_MAIN();