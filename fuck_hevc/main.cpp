# include "common.hpp"
# include "syntax.hpp"

int main() {

    dbg_show( "hevc������ лл.");

    // ��mp4����hevc���ļ�: ./ffmpeg.exe -i .\Elecard_about_Tomsk_part1_HEVC_UHD.mp4 -codec copy -bsf: hevc_mp4toannexb -f hevc ./Elecard_about_Tomsk_part1_HEVC_UHD.mp4.265
    // ʹ��Elecard HEVC Analyzer �鿴 ȷ�Ͽ���
    // ��ʼ�����ֲ����.

    return 0;
}