/*
    Copyright (c) 2013, Taiga Nomi
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include "tiny_cnn/util/util.h"
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <string>
using namespace std;

#define FacePhone_IMAGE_DEPTH (1)
#define FacePhone_IMAGE_WIDTH (18)
#define FacePhone_IMAGE_HEIGHT (24)
#define FacePhone_IMAGE_AREA (FacePhone_IMAGE_WIDTH*FacePhone_IMAGE_HEIGHT)
#define FacePhone_IMAGE_SIZE (FacePhone_IMAGE_AREA*FacePhone_IMAGE_DEPTH)


namespace tiny_cnn {

	void string_split(const std::string &s, char delim, std::vector<std::string> &elems);

/**

 **/
inline void parse_face_phone(const std::string& filename,
                          std::vector<vec_t> *train_images,
                          std::vector<label_t> *train_labels,
						  std::vector<vec_t> *test_images,
						  std::vector<label_t> *test_labels)
{
   
    std::ifstream ifs(filename.c_str(), std::ios::in);
	if (ifs.fail() || ifs.bad()){
		cout << "failed to open file\n";
		return;
	}

	std::vector<vec_t> all_images;
	std::vector<label_t> all_labels;

    uint8_t label;
	std::vector<int> buf(FacePhone_IMAGE_SIZE);

	string line_temp;
    while ( getline(ifs, line_temp) ) {

		vector<string> elems;
		string_split(line_temp, ' ', elems);

		if (elems.size() != FacePhone_IMAGE_SIZE + 1){
			cout << "failed to parse\n";
			cout << elems.size() << endl;
			return;
		};

		for (int i = 1; i < elems.size(); i++){
			vector<string> idx_val;
			string_split(elems[i], ':', idx_val);
			if (idx_val.size() != 2){
				cout << "failed to parse\n";
				cout << idx_val.size() << endl;
				return;
			}
			buf[i - 1] = atoi(idx_val[1].c_str());
		}

		label = 0;
		if (line_temp[0] == '+'){
			label = 1;
		}
		if (line_temp[0] == '-'){
			label = 0;
		}

		vec_t img;
		int x_padding = 7;
		int y_padding = 4;
		int w = FacePhone_IMAGE_WIDTH + 2 * x_padding;
		int h = FacePhone_IMAGE_HEIGHT + 2 * y_padding;
		img.resize(w * h * FacePhone_IMAGE_DEPTH, 0);
		for (int y = 0; y < FacePhone_IMAGE_HEIGHT; y++) {
			for (int x = 0; x < FacePhone_IMAGE_WIDTH; x++) {
				img[(y + y_padding) * w + x + x_padding] = buf[y * FacePhone_IMAGE_WIDTH + x];
			}
		}

		all_images.push_back(img);
		all_labels.push_back(label);
    }

	vector<int> idxs;
	for (int i = 0; i < all_images.size(); i++){
		idxs.push_back(i);
	}

	random_shuffle(idxs.begin(), idxs.end());

	float train_test_r = 0.9;
	int partion = idxs.size() * train_test_r;
	for (int i = 0; i < partion; i++){
		int idx = idxs[i];
		train_images->push_back(all_images[idx]);
		train_labels->push_back(all_labels[idx]);
	}

	for (int i = partion; i < idxs.size(); i++){
		int idx = idxs[i];
		test_images->push_back(all_images[idx]);
		test_labels->push_back(all_labels[idx]);
	}

	cout << "image size :" << (*train_images)[0].size() << endl;
	cout << "train sample num: " << train_images->size() << endl;
	cout << "test sample num: " << test_images->size() << endl;

}


void string_split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	elems.clear();

	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		int st = item.find_first_not_of(" \t");
		int et = item.find_last_not_of(" \t");

		if (item == "") continue;
		if (et < st || st < 0) continue;

		string str = item.substr(st, et - st + 1);

		elems.push_back(str);
	}

}


} // namespace tiny_cnn
