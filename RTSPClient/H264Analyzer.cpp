#include "H264Analyzer.h"

namespace StreamAnalyze
{
    /**
    Calculate the log base 2 of the argument, rounded up.
    Zero or negative arguments return zero
    Idea from http://www.southwindsgames.com/blog/2009/01/19/fast-integer-log2-function-in-cc/
    */
    int intlog2(int x)
    {
        int log = 0;
        if (x < 0) { x = 0; }
        while ((x >> log) > 0)
        {
            log++;
        }
        if (log > 0 && x == 1 << (log - 1)) { log--; }
        return log;
    }


    void h264_read_pps_rbsp(h264_pps_t* pps, bs_t* b)
    {
        memset(pps, 0, sizeof(h264_pps_t));

        pps->pic_parameter_set_id = bs_read_ue(b);
        pps->seq_parameter_set_id = bs_read_ue(b);
        pps->entropy_coding_mode_flag = bs_read_u1(b);

        //2005年版本此处为pps->pic_order_present_flag，2013年版本为bottom_field_pic_order_in_frame_present_flag
        pps->bottom_field_pic_order_in_frame_present_flag = bs_read_u1(b);
        pps->num_slice_groups_minus1 = bs_read_ue(b);

        if (pps->num_slice_groups_minus1 > 0)
        {
            pps->slice_group_map_type = bs_read_ue(b);
            if (pps->slice_group_map_type == 0)
            {
                for (int i_group = 0; i_group <= pps->num_slice_groups_minus1; i_group++)
                {
                    pps->run_length_minus1[i_group] = bs_read_ue(b);
                }
            }
            else if (pps->slice_group_map_type == 2)
            {
                for (int i_group = 0; i_group < pps->num_slice_groups_minus1; i_group++)
                {
                    pps->top_left[i_group] = bs_read_ue(b);
                    pps->bottom_right[i_group] = bs_read_ue(b);
                }
            }
            else if (pps->slice_group_map_type == 3 ||
                pps->slice_group_map_type == 4 ||
                pps->slice_group_map_type == 5)
            {
                pps->slice_group_change_direction_flag = bs_read_u1(b);
                pps->slice_group_change_rate_minus1 = bs_read_ue(b);
            }
            else if (pps->slice_group_map_type == 6)
            {
                pps->pic_size_in_map_units_minus1 = bs_read_ue(b);
                pps->slice_group_id_bytes = intlog2(pps->num_slice_groups_minus1 + 1);
                pps->slice_group_id.resize(pps->pic_size_in_map_units_minus1 + 1);
                for (int i = 0; i <= pps->pic_size_in_map_units_minus1; i++)
                {
                    pps->slice_group_id[i] = bs_read_u(b, pps->slice_group_id_bytes); // was u(v)
                }
            }
        }
        pps->num_ref_idx_l0_active_minus1 = bs_read_ue(b);
        pps->num_ref_idx_l1_active_minus1 = bs_read_ue(b);
        pps->weighted_pred_flag = bs_read_u1(b);
        pps->weighted_bipred_idc = bs_read_u(b, 2);
        pps->pic_init_qp_minus26 = bs_read_se(b);
        pps->pic_init_qs_minus26 = bs_read_se(b);
        pps->chroma_qp_index_offset = bs_read_se(b);
        pps->deblocking_filter_control_present_flag = bs_read_u1(b);
        pps->constrained_intra_pred_flag = bs_read_u1(b);
        pps->redundant_pic_cnt_present_flag = bs_read_u1(b);

        // we don't need other field
    }
}; // namespace StreamAnalyze
