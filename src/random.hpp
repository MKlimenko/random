#ifndef _RANDOM_HPP
#define _RANDOM_HPP

#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cfloat>
#include <type_traits>
#include <vector>

struct random {
	///<summary>Get runtime seed from the <c>std::chrono::high_resolution_clock</c><summary>
	///<returns>Seed in the range of [0, std::uint32_t_max]</returns>
	static auto get_seed() {
		return static_cast<std::uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	
	///<summary>Real uniform distribution</summary>
	///<returns>Uniform distributed value in the range of [0, 1]</returns>
	static double uniform_distribution(std::uint32_t init = 0) {
		static std::uint32_t val = 0x12345;
		if (init) {
			val = init;
			return 0;
		}
		val = val * 214013 + 2531011;
		return ((val & 0x3FFFFFFF) >> 15) / 32767.0;
	}

	///<summary>Uniform distribution</summary>
	///<returns>Uniform distributer value in the range of [min, max]</returns>
	template<typename T>
	static T uniform_distribution(T min, T max) {
		static_assert(std::is_arithmetic<T>::value, "Type is not arithmetic");
		auto val = uniform_distribution();
		return static_cast<T>(val * (max - min) + min);
	}

	///<summary>Real normal distribution</summary>
	///<returns>Real distributed value calculated with Box-Muller algorithm with mean = 0 and sigma = 1</returns>
	static double normal_distribution() {
		static double z0 = 0, z1 = 0;
		static std::int32_t iter = 0;
		if (iter)
			z0 = z1;
		else {
			double x = 0.0, y = 0.0, s = 0.0;
			do {
				x = uniform_distribution() * 2.0 - 1.0;
				y = uniform_distribution() * 2.0 - 1.0;
				s = x * x + y * y;
			} while (s > 1.0 || s < DBL_EPSILON);
			double ss = std::sqrt(-2.0 * std::log(s) / s);
			z0 = x * ss;
			z1 = y * ss;
		}
		iter = 1 - iter;
		return z0;
	}

	///<summary>Real normal distribution</summary>
	///<returns>Real distributed value calculated with Box-Muller algorithm</returns>
	template <typename T>
	static T normal_distribution(double mean, double sigma) {
		return static_cast<T>(normal_distribution() * sigma + mean);
	}

	///<summary>Calculate mean value of the data in the container</summary>
	///<returns>Mean value</returns>
	template <typename Container>
	static double mean(const Container &data) {
		double val = 0.0;
		for (auto&el : data)
			val += el;
		return val / data.size();
	}

	///<summary>Calculate variance of the data in the container</summary>
	///<returns>Variance</returns>
	template <typename Container>
	static double variance(const Container &data) {
		if (data.size() < 2)
			return 0.0;
		double q = 0.0;
		double m = 0.0;
		for (auto&el : data) {
			q += static_cast<double>(el) * el;
			m += signal[i];
		}
		double val = (q - m * m / len) / (len - 1);
		return val;
	}

	///<summary>Calculate sigma of the data in the container</summary>
	///<returns>Sigma</returns>
	template <typename Container>
	static double sigma(const Container &data) {
		return std::sqrt(variance(data));
	}

	///<summary>Calculate histogram for the input data array</summary>
	template <typename T>
	static void histogram(const T* data, std::size_t len, T min, T max, std::vector<double> &hist, std::size_t resolution = 0) {
		if (resolution) {
			hist.clear();
			hist.resize(resolution);
		}
		else
			resolution = hist.size();

		double scale = (resolution - 1.0) / (max - min);
		for (std::size_t i = 0; i < len; ++i) {
			if (data[i] < min || data[i] > max)
				continue;
			std::size_t ind = static_cast<std::size_t>((data[i] - min) * scale);
			++hist[ind];
		}
		double max_val = 0.0;
		for (auto&el : hist)
			if (max_val < el)
				max_val = el;

		for (auto&el : hist)
			el /= max_val;
	}

	///<summary>Calculate histogram for the input data vector</summary>
	template <typename T>
	static void histogram(const std::vector<T> &data, T min, T max, std::vector<double> &hist, std::size_t resolution = 0) {
		histogram(data.data(), data.size(), min, max, hist, resolution);
	}

	///<summary>Calculate histogram for the input data std::array</summary>
	template <typename T, std::size_t size>
	static void histogram(const std::array<T, size> &data, T min, T max, std::vector<double> &hist, std::size_t resolution = 0) {
		histogram(data.data(), size, min, max, hist, resolution);
	}
};

class random_cx {
private:
	///<summary>Convert chars to <c>std::uint32_t</c> due to lack of the <c>constexpr</c> specifier in the <c>std::from_chars</c> function</summary>
	///<returns>x * 10 + y for the representing "xy" string</returns>
	constexpr static auto time_from_string(const char* str, int offset) {
		return static_cast<std::uint32_t>(str[offset] - '0') * 10 +
			static_cast<std::uint32_t>(str[offset + 1] - '0');
	}

	constexpr static std::uint32_t lce_a = 4096;
	constexpr static std::uint32_t lce_c = 150889;
	constexpr static std::uint32_t lce_m = 714025;
public:
	///<summary>Get constexpr seed from the <c>__TIME__</c> macro<summary>
	///<returns>Seed in the range of [0, 89940]</returns>
	constexpr static auto get_seed_constexpr() {
		auto t = __TIME__;
		return time_from_string(t, 0) * 60 * 60 + time_from_string(t, 3) * 60 + time_from_string(t, 6);
	}
	
	///<summary>Constexpr uniform distribution. Passing previous by reference due to the lack of static values in the constexpr context<summary>
	///<returns>Uniformely distributed value in the range of [0, 714025]</returns>
	constexpr static std::uint32_t uniform_distribution(std::uint32_t &previous) {
		previous = ((lce_a * previous + lce_c) % lce_m);
		return previous;
	}

	///<summary>Normalized constexpr uniform distribution. Passing previous by reference due to the lack of static values in the constexpr context<summary>
	///<returns>Uniformely distributed value in the range of [0.0, 1.0]</returns>
	constexpr static double uniform_distribution_n(std::uint32_t &previous) {
		auto dst = uniform_distribution(previous);
		return static_cast<double>(dst) / lce_m;
	}

	///<summary>Get std::array of uniformely distributed values<summary>
	///<returns>std::array of uniformely distributed values in the range of [min, max]</returns>
	template <typename T, std::size_t sz>
	constexpr static auto uniform_distribution(T min, T max) {
		std::array<T, sz> dst{};
		auto previous = get_seed_constexpr();
		for (auto &el : dst) 
			el = static_cast<T>(uniform_distribution_n(previous) * (max - min) + min);
		
		return dst;
	}

	///<summary>Get std::array of normally distributed values. Normal distribution is being approximated by the Irwin-Hall distribution<summary>
	///<returns>std::array of normally distributed values with mean = 0 and sigma = 1</returns>
	template <typename T, std::size_t sz, std::size_t irwin_numbers = 12>
	constexpr static auto normal_distribution(double mean, double sigma) {
		std::array<T, sz> dst{};
		auto previous = get_seed_constexpr();
		for (auto &el : dst) {
			double val = 0;
			for (std::size_t i = 0; i < irwin_numbers; ++i)
				val += uniform_distribution_n(previous);

			el = val / (irwin_numbers / 12) - irwin_numbers / 2;
		}

		return dst;
	}
};

#endif // !_RANDOM_HPP
