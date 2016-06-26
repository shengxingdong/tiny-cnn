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

#define IMAGE_DEPTH (1)
#define IMAGE_WIDTH (32)
#define IMAGE_HEIGHT (32)
#define IMAGE_AREA (IMAGE_WIDTH*IMAGE_HEIGHT)
#define IMAGE_SIZE (IMAGE_AREA*IMAGE_DEPTH)

#define X_PADDING 0
#define Y_PADDING 0
#define USE_FLIP 1

#include "file_utility.h"
#include "cv_utility.h"
#include "cv_header.h"

namespace tiny_cnn {

	void string_split(const std::string &s, char delim, std::vector<std::string> &elems);
	void cvimage2vec_timage(const cv::Mat &cv_img, vec_t &img);
/**

 **/
inline void parse_dir_data(const std::string& data_dir,
                          std::vector<vec_t> *train_images,
                          std::vector<label_t> *train_labels,
						  std::vector<vec_t> *test_images,
						  std::vector<label_t> *test_labels)
{
   
	std::ifstream ifs(data_dir+"/dir_label.txt", std::ios::in);
	if (ifs.fail() || ifs.bad()){
		cout << "failed to open dir_label.txt\n";
		return;
	}
	std::vector<string> dirs;
	std::vector<int> labels;
	string line_temp;
	while (getline(ifs, line_temp)) {
		vector<string> elems;
		string_split(line_temp, ' ', elems);

		if (elems.size() != 2){
			cout << "failed to parse\n"; cout << elems.size() << endl;
			return;
		};

		dirs.push_back(data_dir+"/"+elems[0]);
		labels.push_back(atoi(elems[1].c_str()));
	}


	std::vector<vec_t> all_images;
	std::vector<label_t> all_labels;

    uint8_t label;
	std::vector<int> buf(IMAGE_SIZE);
	for (int i = 0; i < dirs.size(); i++){
		vector<string> files;
		get_files(dirs[i], "jpg", files);

		label = labels[i];
		for (int k = 0; k < files.size(); k++){
			cv::Mat image = cv::imread(files[k], 0);//grey image
			resize(image, image, Size(IMAGE_WIDTH, IMAGE_HEIGHT));

			vec_t img;
			cvimage2vec_timage(image, img);
		
			all_images.push_back(img);
			all_labels.push_back(label);

#if USE_FLIP == 1
			Mat fliped_image;
			cv::flip(image, fliped_image, 0);

			vec_t fliped_img;
			cvimage2vec_timage(fliped_image, fliped_img);

			all_images.push_back(fliped_img);
			all_labels.push_back(label);
#endif

		}
	}

	//随机排序后划分train_sample,test_sample
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

void cvimage2vec_timage(const cv::Mat &cv_img, vec_t &img)
{
	uchar *buf = cv_img.data;
	int w = IMAGE_WIDTH + 2 * X_PADDING;
	int h = IMAGE_HEIGHT + 2 * Y_PADDING;
	img.resize(w * h * IMAGE_DEPTH, 0);
	for (int c = 0; c < IMAGE_DEPTH; c++) {
		for (int y = 0; y < IMAGE_HEIGHT; y++) {
			for (int x = 0; x < IMAGE_WIDTH; x++) {
				img[c * w * h + (y + Y_PADDING) * w + x + X_PADDING] = buf[c * IMAGE_AREA + y * IMAGE_WIDTH + x];
			}
		}
	}

}




} // namespace tiny_cnn
