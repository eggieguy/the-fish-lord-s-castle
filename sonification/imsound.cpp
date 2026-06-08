#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <regex>

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "miniaudio/miniaudio.h"

struct fac {
    int num1;
    int num2;
};
    
fac factorFinder(int num){
    int max = std::floor(std::sqrt(num));
    std::vector<fac> facs;
    fac fact;
    for (int n = 1; n < max; n++){
        if (num % n == 0){
            fact.num1 = n;
            fact.num2 = num/n;
            facs.push_back(fact);
        }
    }
   return facs.back();
}


int main(int argc, char* argv[]){
    if (argc < 2){
        std::cout << "Enter a file next time\n";
        return -1;
    }
    std::regex fileMatch(R"(.*\.mp3)");
    bool match = std::regex_match(argv[1],fileMatch);
    if(!match){
        std::cout << "enter a valid .mp3 file next time\n";
        return -1;
    }
    ma_decoder_config config = ma_decoder_config_init(ma_format_u8,3,30000);
    ma_decoder decoder;
    ma_result result = ma_decoder_init_file(argv[1], &config, &decoder);
    if (result != MA_SUCCESS){
        return -1;
    }

    ma_uint64 frames = 0;
    result = ma_decoder_get_length_in_pcm_frames(&decoder,&frames);
    if (result != MA_SUCCESS){
        return -2;
    }
    size_t samples = frames * 3;
    unsigned char *buffer = new unsigned char[samples];
    ma_uint64 counted = 0;
    result = ma_decoder_read_pcm_frames(&decoder, buffer, samples, &counted);
    if (result != MA_SUCCESS){
        std::cout << "Failure of reading frames\n";
        ma_decoder_uninit(&decoder);
        delete[] buffer;
        return -3;
    }

    ma_decoder_uninit(&decoder);

    fac bigFac = factorFinder(frames);
    stbi_write_jpg("result.jpeg", bigFac.num1, bigFac.num2, 3, buffer, 100 );
    delete[] buffer;


    return 0;
}