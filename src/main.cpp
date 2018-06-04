#include "random.hpp"
#include "signalsviewer.hpp"

#include <array>
#include <vector>

void Runtime(CSignalsViewer *sv) {
	random::uniform_distribution(random::get_seed());
	std::vector<double> normal(50000);
	for (auto&el : normal)
		el = random::normal_distribution();
	sv->Add(normal);
	sv->Add(normal); // Spectrum

	std::vector<double> hist(100);
	random::histogram(normal, -5.0, 5.0, hist);
	sv->Add(hist);
}

void Compiletime(CSignalsViewer *sv) {
	constexpr auto cx_u = random_cx::uniform_distribution<double, 1000>(3.0, 5.0);
	//sv->Add(cx_u.data(), cx_u.size());
	constexpr auto cx_n = random_cx::normal_distribution<double, 50000>(0.0, 1.0);
	sv->Add(cx_n.data(), cx_n.size());
	sv->Add(cx_n.data(), cx_n.size()); // Spectrum
	
	std::vector<double> hist(100);
	random::histogram(cx_n, -5.0, 5.0, hist);
	sv->Add(hist);
}

void GenerateSignals(CSignalsViewer *sv){
	Runtime(sv);

	Compiletime(sv);
}