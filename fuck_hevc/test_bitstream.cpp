# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>

# include "H265_stream.hpp"



using namespace std;

void H265_stream::test() {
    auto path = "./Elecard_about_Tomsk_part2_HEVC_720p.mp4.265";

    FILE* pfile;
    pfile = fopen(path, "rb");
    if (pfile == NULL){
        err_show("无法打开文件");
    }
    fseek(pfile, 0, SEEK_END);
    auto length = ftell(pfile);
    vector<uint8_t> h265_data(length+sizeof(size_t));
    rewind(pfile);
    length = fread(h265_data.data(), 1, length, pfile);
    fclose(pfile);

    this->parse(h265_data);








}





