#pragma once

# define Xt_Nal_unit_type(X__) \
X__(	TRAIL_N	        ,	非TSA，非STSA尾随图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	TRAIL_R	        ,	非TSA，非STSA尾随图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	TSA_N	        ,	TSA图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	TSA_R	        ,	TSA图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	STSA_N	        ,	STSA图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	STSA_R	        ,	STSA图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	RADL_N	        ,	RADL图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	RADL_R	        ,	RADL图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	RASL_N	        ,	RASL图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	RASL_R	        ,	RASL图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	RSV_VCL_N10	    ,	保留的非IRAP SLNR VCL NAL单元类型10	) \
X__(	RSV_VCL_R11	    ,	保留的非IRAP子层参考VCL NAL单元类型11	) \
X__(	RSV_VCL_N12	    ,	保留的非IRAP SLNR VCL NAL单元类型12	) \
X__(	RSV_VCL_R13	    ,	保留的非IRAP子层参考VCL NAL单元类型13	) \
X__(	RSV_VCL_N14	    ,	保留的非IRAP SLNR VCL NAL单元类型14	) \
X__(	RSV_VCL_R15	    ,	保留的非IRAP子层参考VCL NAL单元类型15	) \
X__(	BLA_W_LP	    ,	BLA图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	BLA_W_RADL	    ,	BLA图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	BLA_N_LP	    ,	BLA图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	IDR_W_RADL	    ,	IDR图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	IDR_N_LP	    ,	IDR图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	CRA_NUT	        ,	CRA图片的编码切片片段 slice_segment_layer_rbsp	) \
X__(	RSV_IRAP_VCL22	,	保留的IRAP VCL NAL单元类型	) \
X__(	RSV_IRAP_VCL23	,	保留的IRAP VCL NAL单元类型	) \
X__(	RSV_VCL24	    ,	保留的非IRAP VCL NAL单元类型	) \
X__(	RSV_VCL25	    ,	保留的非IRAP VCL NAL单元类型	) \
X__(	RSV_VCL26	    ,	保留的非IRAP VCL NAL单元类型	) \
X__(	RSV_VCL27	    ,	保留的非IRAP VCL NAL单元类型	) \
X__(	RSV_VCL28	    ,	保留的非IRAP VCL NAL单元类型	) \
X__(	RSV_VCL29	    ,	保留的非IRAP VCL NAL单元类型	) \
X__(	RSV_VCL30	    ,	保留的非IRAP VCL NAL单元类型	) \
X__(	RSV_VCL31	    ,	保留的非IRAP VCL NAL单元类型	) \
X__(	VPS_NUT	        ,	视频参数集video_parameter_set_rbsp	) \
X__(	SPS_NUT	        ,	序列参数集seq_parameter_set_rbsp	) \
X__(	PPS_NUT	        ,	图片参数集pic_parameter_set_rbsp	) \
X__(	AUD_NUT	        ,	访问单元定界符access_unit_delimiter_rbsp	) \
X__(	EOS_NUT	        ,	序列结尾end_of_seq_rbsp	) \
X__(	EOB_NUT	        ,	比特流结尾end_of_bitstream_rbsp	) \
X__(	FD_NUT	        ,	填充数据fill_data_rbsp	) \
X__(	PREFIX_SEI_NUT	,	补充增强信息sei_rbsp	) \
X__(	SUFFIX_SEI_NUT	,	补充增强信息sei_rbsp	) \
X__(	RSV_NVCL41	    ,	预留41	) \
X__(	RSV_NVCL42	    ,	预留42	) \
X__(	RSV_NVCL43	    ,	预留43	) \
X__(	RSV_NVCL44	    ,	预留44	) \
X__(	RSV_NVCL45	    ,	预留45	) \
X__(	RSV_NVCL46	    ,	预留46	) \
X__(	RSV_NVCL47	    ,	预留47	) \
X__(	UNSPEC48	    ,	未指定48	) \
X__(	UNSPEC49	    ,	未指定49	) \
X__(	UNSPEC50	    ,	未指定50	) \
X__(	UNSPEC51	    ,	未指定51	) \
X__(	UNSPEC52	    ,	未指定52	) \
X__(	UNSPEC53	    ,	未指定53	) \
X__(	UNSPEC54	    ,	未指定54	) \
X__(	UNSPEC55	    ,	未指定55	) \
X__(	UNSPEC56	    ,	未指定56	) \
X__(	UNSPEC57	    ,	未指定57	) \
X__(	UNSPEC58	    ,	未指定58	) \
X__(	UNSPEC59	    ,	未指定59	) \
X__(	UNSPEC60	    ,	未指定60	) \
X__(	UNSPEC61	    ,	未指定61	) \
X__(	UNSPEC62	    ,	未指定62	) \
X__(	UNSPEC63	    ,	未指定63	) \


