#include <benchmark/benchmark.h>
#include <chrono>
#include <random>
#include <vector>
#include "random.hpp"

#pragma comment (lib, "shlwapi.lib")

namespace {
	using linear_congruential = std::linear_congruential_engine<std::uint32_t, 8121, 28411, 134456>;
}

namespace uniform {
	static void uniform_STL_mt19937_real(benchmark::State& state) {
		std::mt19937 generator(random::get_seed());
		std::uniform_real_distribution<double> distribution(0, 1);
		for (auto _ : state)
			auto val = distribution(generator);
	}
	BENCHMARK(uniform_STL_mt19937_real)->Complexity();

	static void uniform_STL_lce_real(benchmark::State& state) {
		linear_congruential generator(random::get_seed());

		std::uniform_real_distribution<double> distribution(0, 1);
		for (auto _ : state)
			auto val = distribution(generator);
	}
	BENCHMARK(uniform_STL_lce_real)->Complexity();

	static void Uniform_Random_real(benchmark::State& state) {
		random::uniform_distribution(random::get_seed());
		for (auto _ : state)
			auto val = random::uniform_distribution<double>(0, 1);
	}
	BENCHMARK(Uniform_Random_real)->Complexity();

	static void uniform_STL_mt19937_int(benchmark::State& state) {
		std::mt19937 generator(random::get_seed());
		std::uniform_int_distribution<int> distribution(0, 1);
		for (auto _ : state)
			auto val = distribution(generator);
	}
	BENCHMARK(uniform_STL_mt19937_int)->Complexity();

	static void uniform_STL_lce_int(benchmark::State& state) {
		linear_congruential generator(random::get_seed());

		std::uniform_int_distribution<int> distribution(0, 1);
		for (auto _ : state)
			auto val = distribution(generator);
	}
	BENCHMARK(uniform_STL_lce_int)->Complexity();

	static void uniform_random_int(benchmark::State& state) {
		random::uniform_distribution(random::get_seed());
		for (auto _ : state)
			auto val = random::uniform_distribution<int>(0, 1);
	}
	BENCHMARK(uniform_random_int)->Complexity();
}

namespace normal {
	static void normal_STL_mt19937(benchmark::State& state) {
		std::mt19937 generator(random::get_seed());
		std::normal_distribution<double> distribution(0, 1);
		for (auto _ : state)
			auto val = distribution(generator);
	}
	BENCHMARK(normal_STL_mt19937)->Complexity();

	static void normal_STL_lce(benchmark::State& state) {
		linear_congruential generator(random::get_seed());

		std::normal_distribution<double> distribution(0, 1);
		for (auto _ : state)
			auto val = distribution(generator);
	}
	BENCHMARK(normal_STL_lce)->Complexity();

	static void normal_random(benchmark::State& state) {
		random::uniform_distribution(random::get_seed());
		for (auto _ : state)
			auto val = random::normal_distribution<double>(0, 1);
	}
	BENCHMARK(normal_random)->Complexity();
}

//BENCHMARK_MAIN();
int main(int argc, char** argv) {
	::benchmark::Initialize(&argc, argv);
	if (::benchmark::ReportUnrecognizedArguments(argc, argv)) 
		return 1;
	::benchmark::RunSpecifiedBenchmarks();
	system("pause");
}