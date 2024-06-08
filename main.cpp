#include <iostream>
#include <vector>
#include <omp.h>
#include "paths.h"
#include "image.hpp"

//#define _USING_OPENMP //uncomment if you want use multithreading

void addBrigtnessImage(ML::Image* img, uint8_t addVal){
#if defined(_OPENMP) && defined(_USING_OPENMP)
    #pragma omp parallel for
#endif
    for(size_t i = 0; i<img->getWidth(); i++){
        for(size_t j = 0; j<img->getHeight(); j++){
            img->addPix(i,j, {addVal, addVal, addVal, 0});
        }
    }
}

void fastAddingBrigtness(const ML::ImageCreator& ic,
                         const std::string& dataPath,
                         const std::string& outPath,
                         const std::string& imgTitle,
                         uint8_t addVal)
{
    auto img = ic.createImage(dataPath + "/" + imgTitle);
    addBrigtnessImage(img, addVal);
    img->write(outPath + "/" + imgTitle);
    std::cout<< imgTitle <<" was created.\n";
}

int main(){
#if defined(_OPENMP) && defined(_USING_OPENMP)
    std::cout << "Using OpenMP. Threads: " << omp_get_max_threads() << "\n";
#endif

    std::string dataPath = std::string(SOURCE_PATH) + "/data";
    std::string outPath = std::string(SOURCE_PATH) + "/out";

    std::filesystem::create_directory(outPath);
    
    ML::ImageCreator ic;

    fastAddingBrigtness(ic, dataPath, outPath, "img1.png", 100);
    fastAddingBrigtness(ic, dataPath, outPath, "img2.png", 40);

    auto img = ic.createImage(dataPath + "/img1.png");
    addBrigtnessImage(img, 220);
    img->write(outPath+"/img3.png");
    std::cout<<"img3.png was created.\n";

    return 0;
}