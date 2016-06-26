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
#include <iostream>
#include <memory>
#include "tiny_cnn/tiny_cnn.h"
#include "dir_data_parser.h"

using namespace tiny_cnn;
using namespace tiny_cnn::activation;
using namespace tiny_cnn::layers;

using namespace std;

void sample1_convnet(const string& data_dir_path = "D:/data/smoking/cnn_train");

int main(int argc, char** argv) {
	sample1_convnet();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// learning convolutional neural networks (LeNet-5 like architecture)
void sample1_convnet(const string& data_dir_path) {
    // construct LeNet-5 architecture
    network<sequential> nn;
    adagrad optimizer;

    // connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
    static const bool connection [] = {
        O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
        O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
        O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
        X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
        X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
        X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
    };
#undef O
#undef X

    nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6) // 32x32 in, 5x5 kernel, 1-6 fmaps conv
       << average_pooling_layer<tan_h>(28, 28, 6, 2) // 28x28 in, 6 fmaps, 2x2 subsampling
       << convolutional_layer<tan_h>(14, 14, 5, 6, 16,
                                     connection_table(connection, 6, 16)) // with connection-table
       << average_pooling_layer<tan_h>(10, 10, 16, 2)
       << convolutional_layer<tan_h>(5, 5, 5, 16, 120)
       << fully_connected_layer<tan_h>(120, 2);

    std::cout << "load models..." << std::endl;

    // load MNIST dataset
    std::vector<label_t> train_labels, test_labels;
    std::vector<vec_t> train_images, test_images;

    //parse_mnist_labels(data_dir_path + "/train-labels.idx1-ubyte", &train_labels);
    //parse_mnist_images(data_dir_path + "/train-images.idx3-ubyte", &train_images, -1.0, 1.0, 2, 2);
    //parse_mnist_labels(data_dir_path + "/t10k-labels.idx1-ubyte", &test_labels);
    //parse_mnist_images(data_dir_path + "/t10k-images.idx3-ubyte", &test_images, -1.0, 1.0, 2, 2);

	//parse_face_phone(data_dir_path + "/face_phoneing_data-L.txt", &train_images, &train_labels, &test_images, &test_labels);
	parse_dir_data(data_dir_path, &train_images, &train_labels, &test_images, &test_labels);

#if 1
    std::cout << "start learning" << std::endl;

    progress_display disp(train_images.size());
    timer t;
    int minibatch_size = 10;

    optimizer.alpha *= std::sqrt(minibatch_size);

    // create callback
    auto on_enumerate_epoch = [&](){
        std::cout << t.elapsed() << "s elapsed." << std::endl;

        tiny_cnn::result res = nn.test(test_images, test_labels);

        std::cout << res.num_success << "/" << res.num_total << std::endl;

        disp.restart(train_images.size());
        t.restart();
    };

    auto on_enumerate_minibatch = [&](){ 
        disp += minibatch_size; 
    };
    
    // training
    nn.train<mse>(optimizer, train_images, train_labels, minibatch_size, 20, on_enumerate_minibatch, on_enumerate_epoch);

    std::cout << "end training." << std::endl;
	// save networks
	std::ofstream ofs("LeNet-weights-Smoking");
	ofs << nn;
#else
	std::ifstream ifs("LeNet-weights-Smoking");
	ifs >> nn;
#endif
    // test and show results
	cout << "testing...\n";
	clock_t s = clock();
    nn.test(test_images, test_labels).print_detail(std::cout);
	std::vector<vec_t> result = nn.test(test_images);
	cout << "test image num: " << test_images.size() << endl;
	cout << "time used per image: " << (clock() - s) / (test_images.size() + 0.0) << endl;
	cin >> s;

}
