# include "common.hpp"
# include "syntax.hpp"

int fuck();


int main() {

    dbg_show << "hevc是垃圾 谢谢.";

    // 从mp4分离hevc到文件: ./ffmpeg.exe -i .\Elecard_about_Tomsk_part1_HEVC_UHD.mp4 -codec copy -bsf: hevc_mp4toannexb -f hevc ./Elecard_about_Tomsk_part1_HEVC_UHD.mp4.265
    // 使用Elecard HEVC Analyzer 查看 确认可以
    // 开始参照手册解码.

    detail_show<<fuck();

    return 0;
}