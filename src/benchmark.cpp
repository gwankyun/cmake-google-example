#include <string>
#include <iostream>
#include <benchmark/benchmark.h>
#include <glog/logging.h>
#pragma warning(push)
#pragma warning(disable: 4127)
#include <man.pb.h>
#pragma warning(pop)
#include <sstream>
#if USE_BOOST
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#endif // USE_BOOST
#if USE_MSGPACK
#include <msgpack.hpp>
#endif // USE_MSGPACK

struct Man
{
    int age;
    std::string name;
};

#if USE_MSGPACK
namespace msg
{
    struct Man
    {
        int age;
        std::string name;
        MSGPACK_DEFINE(age, name);
    };
}
#endif // USE_MSGPACK

#if USE_BOOST
namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, Man& man, const unsigned int version)
        {
            (void)version;
            ar& man.age;
            ar& man.name;
        }
    }
}
#endif // USE_BOOST

static void BM_Glog(benchmark::State& state)
{
    FLAGS_logtostderr = 1;
    for (auto _ : state)
    {
        LOG(INFO) << "";
    }
}
//BENCHMARK(BM_Glog);

static void BM_StringCreation(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string empty_string;
    }
}
BENCHMARK(BM_StringCreation);

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
    std::string str;
    test::man man;
    test::man man2;
    for (auto _ : state)
    {
        man.set_age(18);
        man.set_name("Tom");

        man.SerializeToString(&str);

        man2.ParseFromString(str);
    }
}
BENCHMARK(BM_Protobuf);

#if USE_BOOST
static void BM_BoostSerialization(benchmark::State& state)
{
    Man man;
    Man man2;
    std::string buffer;
    for (auto _ : state)
    {
        man.age = 18;
        man.name = "Tom";
        std::stringstream obuffer;
        boost::archive::text_oarchive otext(obuffer);
        otext << man;
        buffer = obuffer.str();

        std::stringstream ibuffer(buffer);
        boost::archive::text_iarchive itext(ibuffer);
        itext >> man2;
    }
}
BENCHMARK(BM_BoostSerialization);
#endif // USE_BOOST

#if USE_MSGPACK
static void BM_Msgpack(benchmark::State& state)
{
    msg::Man man;
    msg::Man man2;

    for (auto _ : state)
    {
        man.age = 18;
        man.name = "Tom";
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, man);

        msgpack::object_handle oh =
            msgpack::unpack(sbuf.data(), sbuf.size());

        msgpack::object obj = oh.get();

        obj.convert(man2);
    }
}
BENCHMARK(BM_Msgpack);
#endif // USE_MSGPACK

BENCHMARK_MAIN();