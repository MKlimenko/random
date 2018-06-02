#include "random.hpp"
#include "signalsviewer.hpp"

#include <array>
#include <vector>

void GenerateSignals(CSignalsViewer *sv){
	random::uniform_distribution(random::get_seed());
	std::vector<double> normal(100000);
	for (auto&el : normal)
		el = random::normal_distribution();
	sv->Add(normal);
	sv->Add(normal); // Spectrum

	std::vector<double> hist(100);
	random::histogram(normal, -5.0, 5.0, hist);
	sv->Add(hist);
}