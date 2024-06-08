#include <iostream>
#include <vector>
#include <omp.h>
#include "paths.h"
#include "image.hpp"

int main(){
    std::cout<<"123";
    std::vector<int> c(1000, 0), a(1000, 0), b(1000, 0);

#if defined(_OPENMP)
    std::cout<< omp_get_max_threads();
    #pragma omp parallel for
#endif
    for(int i = 0; i<1000; i++){
        b[i] = i;
        c[i] = (a[i] + b[i]);
    }
    std::cout<<"Hi\n";

    ML::Image* img = new ML::ImagePNG();
    std::cout<<img->getWidth()<<" "<<img->getHeight()<<"\n";

    img->read(std::string(SOURCE_PATH) + "/data/img1.png");
    std::cout<<img->getWidth()<<" "<<img->getHeight()<<"\n";

    img->read(std::string(SOURCE_PATH) + "/data/img2.png");
    std::cout<<img->getWidth()<<" "<<img->getHeight()<<"\n";

    //img->setPix(20, 50, {255, 255, 255, 128});
    uint8_t addVal = 10;
    for(size_t i = 0; i<img->getWidth(); i++){
        for(size_t j = 0; j<img->getHeight(); j++){
            img->addPix(i,j, {addVal, addVal, addVal, 0});
        }
    }
    img->write(std::string(SOURCE_PATH) + "/data/img3.png");

    return 0;
}