#pragma once

#include "bs.h"

#include <vector>

namespace StreamAnalyze
{
using std::vector;

typedef struct
{
    int pic_parameter_set_id;
    int seq_parameter_set_id;
    int entropy_coding_mode_flag;
    int pic_order_present_flag; // 2005版本为此字段名 保留，不影响库本身write的编译，但实际不使用
    int bottom_field_pic_order_in_frame_present_flag; // 2013版本为此字段名
    int num_slice_groups_minus1; // todo slice数目
    int slice_group_map_type;
    int run_length_minus1[8]; // up to num_slice_groups_minus1, which is <= 7 in Baseline and Extended, 0 otheriwse
    int top_left[8];
    int bottom_right[8];
    int slice_group_change_direction_flag;
    int slice_group_change_rate_minus1;
    int pic_size_in_map_units_minus1;
    int slice_group_id_bytes;
    vector<int> slice_group_id; // FIXME what size?
    int num_ref_idx_l0_active_minus1;
    int num_ref_idx_l1_active_minus1;
    int weighted_pred_flag;
    int weighted_bipred_idc;
    int pic_init_qp_minus26;
    int pic_init_qs_minus26;
    int chroma_qp_index_offset;
    int deblocking_filter_control_present_flag;
    int constrained_intra_pred_flag;
    int redundant_pic_cnt_present_flag;

    // see if we carry any of the optional headers
    int _more_rbsp_data_present;

    int transform_8x8_mode_flag;
    int pic_scaling_matrix_present_flag;
    int pic_scaling_list_present_flag[8];
    int* ScalingList4x4[6];
    int UseDefaultScalingMatrix4x4Flag[6];
    int* ScalingList8x8[2];
    int UseDefaultScalingMatrix8x8Flag[2];
    int second_chroma_qp_index_offset;
} h264_pps_t;


void h264_read_pps_rbsp(h264_pps_t* pps, bs_t* b);

} // namespace StreamAnalyze
