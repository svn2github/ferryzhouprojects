#ifndef FVISION_MODEL_EVALUATION_TEMPLATE_H_
#define FVISION_MODEL_EVALUATION_TEMPLATE_H_

#include "math.h"
#include "template_utils.h"
#include "misc.h"

#include <vector>

namespace fvision {

//first set with samples and errors
//then call filterWithThreshold
template <typename Sample>
class ModelEvaluationResults {
public:
	ModelEvaluationResults<Sample>() {}

public:
	typedef std::vector<Sample> Samples;
	typedef std::vector<double> Errors;
	typedef std::vector<int> Indices;

public:
	void filterWithThreshold(double threshold) {
		averageError = getMean(errors);
		getInlierOutlierIndices(errors, threshold, inlierIndices, outlierIndices);
		inlierErrors = getSubSet(errors, inlierIndices);
		outlierErrors = getSubSet(errors, outlierIndices);
		averageInlierError = getMean(inlierErrors);
		averageOutlierError = getMean(outlierErrors);
		inlierSamples = getSubSet(samples, inlierIndices);
		outlierSamples = getSubSet(samples, outlierIndices);
	}

	double getInliersRate() const {
		if (samples.size() == 0) return 0;
		return (double)inlierIndices.size() / samples.size();
	}

public:
	Samples samples;
	Errors errors;
	double averageError;
	Indices inlierIndices;
	Indices outlierIndices;
	Errors inlierErrors;
	Errors outlierErrors;
	double averageInlierError;
	double averageOutlierError;
	Samples inlierSamples;
	Samples outlierSamples;

};

template <typename Model, typename Sample, typename ErrorCaculator>
class ModelEvaluator {
public:
	ModelEvaluator<Model, Sample, ErrorCaculator>(ErrorCaculator* errorCalculator, double threshold) {
		this->errorCalculator = errorCalculator;
		this->threshold = threshold;
	}

	~ModelEvaluator<Model, Sample, ErrorCaculator>() {
		delete errorCalculator;
	}

public:
	typedef std::vector<Sample> Samples;


	const ModelEvaluationResults<Sample>& compute(const Model& model, const Samples& samples) {
		results.samples = samples;
		results.errors.resize(samples.size());
		for (size_t i = 0; i < samples.size(); i++) {
			results.errors[i] = errorCalculator->compute(model, samples[i]);
		}
		results.filterWithThreshold(threshold);
		return results;
	}

public:
	ModelEvaluationResults<Sample> results;

private:
	ErrorCaculator* errorCalculator;
	double threshold;
};

}
#endif // FVISION_MODEL_EVALUATION_TEMPLATE_H_