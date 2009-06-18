#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <cv.h>

namespace fvision {

	//===================== Template Functions ===================>

	template <typename T>
	void save(const char* filename, const T& t) {
		std::ofstream ofs(filename);
		ofs<<t;
		ofs.close();
	}

	template <typename T>
	void save(const std::string& filename, const T& t) {
		std::ofstream ofs(filename.c_str());
		ofs<<t;
		ofs.close();
	}

	template <typename T>
	void load(const std::string& filename, T& t){
		std::ifstream ifs(filename.c_str());
		ifs>>t;
		ifs.close();
	}

	/** write a std::vector to a file
	 * format:
	 * n
	 * v[0]
	 * v[1]
	 * ...
	 */
	template <typename T>
	void saveV(const std::string& filename, const std::vector<T>& ts) {
		std::ofstream ofs(filename.c_str());
		ofs<<ts.size()<<std::endl;
		for (size_t i = 0; i < ts.size(); i++) {
			ofs<<ts[i]<<std::endl;
		}
		ofs.close();
	}

	/** load a std::vector from a file
	 * format:
	 * n
	 * v[0]
	 * v[1]
	 * ...
	 */
	template <typename T>
	void loadV(const std::string& filename, std::vector<T>& ts) {
		std::ifstream ifs(filename.c_str());
		int n;
		ifs>>n;
		for (int i = 0; i < n; i++) {
			T t;
			ifs>>t;
			ts.push_back(t);
			ifs.get();
		}
		ifs.close();
	}

	/** write a std::vector to a file with no number of elements
	 * format:
	 * v[0]
	 * v[1]
	 * ...
	 */
	template <typename T>
	void saveVWithNoNumber(const std::string& filename, const std::vector<T>& ts) {
		std::ofstream ofs(filename.c_str());
		for (size_t i = 0; i < ts.size(); i++) {
			ofs<<ts[i]<<std::endl;
		}
		ofs.close();
	}

	/** load a std::vector from a file with no number of elements
	 * format:
	 * v[0]
	 * v[1]
	 * ...
	 */
	template <typename T>
	void loadVWithNoNumber(const std::string& filename, std::vector<T>& ts) {
		std::ifstream ifs(filename.c_str());
		while (!ifs.eof()) {
			//skip to next non whitespace character
			while(true) {
				char c = ifs.get();
				if (c == ' ' || c == '\r' || c == '\n') continue;
				else {
					ifs.unget();
					break;
				}
			}

			if (ifs.eof()) break;
			T t;
			ifs>>t;
			ts.push_back(t);
		}
		ifs.close();
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const std::vector<T>& vs) {
		for (size_t i = 0; i < vs.size(); i++) {
			os<<vs[i]<<endl;
		}
		return os;
	}

	template<typename T>
	void outputToSameLine(std::ostream& os, const std::vector<T>& vs) {
		for (size_t i = 0; i < vs.size(); i++) {
			os<<vs[i]<<" ";
		}
	}

	template<typename T>
	void outputToTwoDimensionVector(std::ostream& os, const std::vector<std::vector<T>>& vss) {
		for (size_t i = 0; i < vss.size(); i++) {
			outputToSameLine(os, vss[i]);
			os<<endl;
		}
	}

}