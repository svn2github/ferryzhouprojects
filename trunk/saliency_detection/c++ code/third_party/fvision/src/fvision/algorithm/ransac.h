#ifndef FVISION_RANSAC_H_
#define FVISION_RANSAC_H_

#include <fvision/utils/random_utils.h>
#include <fvision/utils/misc.h>

#include <vector>
#include <iostream>
#include <cassert>

namespace fvision {

class RANSAC_SamplesNumber {
public:
	RANSAC_SamplesNumber(int modelSampleSize) {
		this->s = modelSampleSize;
		this->p = 0.99;
	}
	~RANSAC_SamplesNumber(void) {}

public:
	long calcN(int inliersNumber, int samplesNumber) {
		double e = 1 - (double)inliersNumber / samplesNumber;
		//cout<<"e: "<<e<<endl;
		if (e > 0.9) e = 0.9;
		//cout<<"pow: "<<pow((1 - e), s)<<endl;
		//cout<<log(1 - pow((1 - e), s))<<endl;
		long N = (long)(log(1 - p) / log(1 - pow((1 - e), s)));
		if (N < 0) return (long)1000000000;
		else return N;
	}

private:		
	int s;      //samples size for fitting a model
	double p;   //probability that at least one of the random samples if free from outliers
	            //usually 0.99
};

//fit a model to a set of samples
template <typename M, typename S>
class GenericModelCalculator {
public:
	typedef std::vector<S> Samples;
	virtual M compute(const Samples& samples) = 0;

	virtual ~GenericModelCalculator<M, S>() {}

	//the model calculator may only use a subset of the samples for computing
	//default return empty for both
	virtual const std::vector<int>& getInlierIndices() const { return defaultInlierIndices; };
	virtual const std::vector<int>& getOutlierIndices() const { return defaultOutlierIndices; };

	// if the subclass has a threshold parameter, it need to override the following three functions
	// this is used for algorithms which have a normalization step on input samples
	virtual bool hasThreshold() const { return false; }
	virtual void setThreshold(double threshold) {}
	virtual double getThreshold() const { return 0; }

protected:
	std::vector<int> defaultInlierIndices;
	std::vector<int> defaultOutlierIndices;
};

//evaluate a model to samples
//using a threshold to distinguish inliers and outliers
template <typename M, typename S>
class GenericErrorCaclculator {
public:
	virtual ~GenericErrorCaclculator<M, S>() {}

	typedef std::vector<S> Samples;

	virtual double compute(const M& model, const S& sample) const = 0;

	double computeAverage(const M& model, const Samples& samples) const {
		int n = (int)samples.size();
		if (n == 0) return 0;
		double sum = 0;
		for (int i = 0; i < n; i++) {
			sum += compute(model, samples[i]);
		}
		return sum / n;
	}

	double computeInlierAverage(const M& model, const Samples& samples) const {
		int n = (int)samples.size();
		if (n == 0) return 0;
		double sum = 0;
		double error = 0;
		int inlierNum = 0;
		for (int i = 0; i < n; i++) {
			error = compute(model, samples[i]);
			if (error <= threshold) {
				sum += error;
				inlierNum++;
			}
		}
		if (inlierNum == 0) return 1000000;
		return sum / inlierNum;
	}

public:

	/** set a threshold for classify inliers and outliers
	 */
	void setThreshold(double v) { threshold = v; }

	double getThreshold() const { return threshold; }

	/** classify all samples to inliers and outliers
	 * 
	 */
	void classify(const M& model, const Samples& samples, Samples& inliers, Samples& outliers) const {
		inliers.clear();
		outliers.clear();
		Samples::const_iterator iter = samples.begin();
		for (; iter != samples.end(); ++iter) {
			if (isInlier(model, *iter)) inliers.push_back(*iter);
			else outliers.push_back(*iter);
		}
	}

	/** classify all samples to inliers and outliers, output indices
	 * 
	 */
	void classify(const M& model, const Samples& samples, std::vector<int>& inlierIndices, std::vector<int>& outlierIndices) const {
		inlierIndices.clear();
		outlierIndices.clear();
		Samples::const_iterator iter = samples.begin();
		int i = 0;
		for (; iter != samples.end(); ++iter, ++i) {
			if (isInlier(model, *iter)) inlierIndices.push_back(i);
			else outlierIndices.push_back(i);
		}
	}

	/** classify all samples to inliers and outliers
	 * 
	 */
	void classify(const M& model, const Samples& samples, 
		std::vector<int>& inlierIndices, std::vector<int>& outlierIndices, 
		Samples& inliers, Samples& outliers) const {

		inliers.clear();
		outliers.clear();
		inlierIndices.clear();
		outlierIndices.clear();
		Samples::const_iterator iter = samples.begin();
		int i = 0;
		for (; iter != samples.end(); ++iter, ++i) {
			if (isInlier(model, *iter)) {
				inliers.push_back(*iter);
				inlierIndices.push_back(i);
			}
			else {
				outliers.push_back(*iter);
				outlierIndices.push_back(i);
			}
		}
	}

	int calcInliersNumber(const M& model, const Samples& samples) const {
		int n = 0;
		for (int i = 0; i < (int)samples.size(); i++) {
			if (isInlier(model, samples[i])) ++n;
		}
		return n;
	}

	bool isInlier(const M& model, const S& sample) const {
		return (compute(model, sample) <= threshold);
	}

private:
	double threshold;
};

/** generic RANSAC framework
 * make use of a model calculator and an error calculator
 * M is the model type, need to support copy assignment operator and default constructor
 * S is the sample type.
 *
 * Interface:
 *  M compute(samples); input a set of samples, output a model. 
 *  after compute, inliers and outliers can be retrieved
 * 
 */
template <typename M, typename S>
class Ransac : public GenericModelCalculator<M, S> {
public:
	typedef std::vector<S> Samples;

	/** Constructor
	 * 
	 * @param pmc a GenericModelCalculator object
	 * @param modelSampleSize how much samples are used to fit a model
	 * @param pec a GenericErrorCaclculator object
	 */
	Ransac(GenericModelCalculator<M, S>* pmc, int modelSampleSize, GenericErrorCaclculator<M, S>* pec) {
		this->pmc = pmc;
		this->modelSampleSize = modelSampleSize;
		this->pec = pec;
		this->maxSampleCount = 500;
		this->minInliersNum = 1000000;

		this->verbose = false;
	}

	const GenericErrorCaclculator<M, S>* getErrorCalculator() const { return pec; }

	virtual ~Ransac() {
		delete pmc;
		delete pec;
	}

	void setMaxSampleCount(int n) {
		this->maxSampleCount = n;
	}

	void setMinInliersNum(int n) {
		this->minInliersNum = n;
	}

	virtual bool hasThreshold() const { return true; }

	virtual void setThreshold(double threshold) {
		pec->setThreshold(threshold);
	}

	virtual double getThreshold() const {
		return pec->getThreshold();
	}

public:
	/** Given samples, compute a model that has most inliers. Assume the samples size is larger or equal than model sample size
	 * inliers, outliers, inlierIndices and outlierIndices are stored
	 * 
	 */
	M compute(const Samples& samples) {
		clear();

		int pointsNumber = (int)samples.size();

		assert(pointsNumber >= modelSampleSize);

		long N = 100000;
		int sampleCount = 0;
		RANSAC_SamplesNumber ransac(modelSampleSize);

		M bestModel;
		int maxInliersNumber = 0;

		bool stop = false;
		while (sampleCount < N && sampleCount < maxSampleCount && !stop) {

			Samples nsamples;
			randomlySampleN(samples, nsamples, modelSampleSize);

			M sampleModel = pmc->compute(nsamples);
			if (maxInliersNumber == 0) bestModel = sampleModel;  //init bestModel

			int inliersNumber = pec->calcInliersNumber(sampleModel, samples);
			if (verbose) std::cout<<"inliers number: "<<inliersNumber<<std::endl;

			if (inliersNumber > maxInliersNumber) {
				bestModel = sampleModel;
				maxInliersNumber = inliersNumber;
				N = ransac.calcN(inliersNumber, pointsNumber);
				if (maxInliersNumber > minInliersNum) stop = true;
			}

			if (verbose) std::cout<<"N: "<<N<<std::endl;

			sampleCount ++;
		}

		if (verbose) std::cout<<"sampleCount: "<<sampleCount<<std::endl;

		finalModel = computeUntilConverge(bestModel, maxInliersNumber, samples);
		
		pec->classify(finalModel, samples, inlierIndices, outlierIndices, inliers, outliers);

		inliersRate = (double)inliers.size() / samples.size();

		return finalModel;
	}

	const Samples& getInliers() const { return inliers; }
	const Samples& getOutliers() const { return outliers; }

	const std::vector<int>& getInlierIndices() const { return inlierIndices; }
	const std::vector<int>& getOutlierIndices() const { return outlierIndices; }

	double getInliersAverageError() const {
		return pec->computeAverage(finalModel, inliers);
	}

	double getInliersRate() const {
		return inliersRate;
	}

	void setVerbose(bool v) {
		verbose = v;
	}

private:
	void randomlySampleN(const Samples& samples, Samples& nsamples, int sampleSize) {
		std::vector<int> is = ranis((int)samples.size(), sampleSize);
		for (int i = 0; i < sampleSize; i++) {
			nsamples.push_back(samples[is[i]]);
		}
	}

	/** from initial model, iterate to find the best model.
	 *
	 */
	M computeUntilConverge(M initModel, int initInliersNum, const Samples& samples) {
		if (verbose) {
			std::cout<<"iterate until converge...."<<std::endl;
			std::cout<<"init inliers number: "<<initInliersNum<<std::endl;
		}

		M bestModel = initModel;
		M newModel = initModel;

		int lastInliersNum = initInliersNum;

		Samples newInliers, newOutliers;
		pec->classify(initModel, samples, newInliers, newOutliers);
		double lastInlierAverageError = pec->computeAverage(initModel, newInliers);

		if (verbose) std::cout<<"init inlier average error: "<<lastInlierAverageError<<std::endl;

		while (true && (int)newInliers.size() >= modelSampleSize) {

			//update new model with new inliers, the new model does not necessarily have more inliers
			newModel = pmc->compute(newInliers);

			pec->classify(newModel, samples, newInliers, newOutliers);

			int newInliersNum = (int)newInliers.size();
			double newInlierAverageError = pec->computeAverage(newModel, newInliers);

			if (verbose) {
				std::cout<<"new inliers number: "<<newInliersNum<<std::endl;
				std::cout<<"new inlier average error: "<<newInlierAverageError<<std::endl;
			}
			if (newInliersNum < lastInliersNum) break;
			if (newInliersNum == lastInliersNum && newInlierAverageError >= lastInlierAverageError) break;

			//update best model with the model has more inliers
			bestModel = newModel;

			lastInliersNum = newInliersNum;
			lastInlierAverageError = newInlierAverageError;
		}

		return bestModel;
	}

	void clear() {
		inliers.clear();
		outliers.clear();
		inlierIndices.clear();
		outlierIndices.clear();
	}

private:
	GenericModelCalculator<M, S>* pmc;
	GenericErrorCaclculator<M, S>* pec;
	int modelSampleSize;

	int maxSampleCount;
	int minInliersNum;

	M finalModel;

	Samples inliers;
	Samples outliers;

	std::vector<int> inlierIndices;
	std::vector<int> outlierIndices;

	double inliersRate;

private:
	bool verbose;

};

}
#endif // FVISION_RANSAC_H_