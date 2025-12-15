#include "orb_extraction.h"

// ================================================================================
// BRIEF Pattern（256个测试点对）
// ================================================================================
static const BriefPattern brief_pattern[BRIEF_LENGTH] = {
    {-8, -3, 9, 5}, {4, 2, 7, -12}, {-11, 9, -8, 2}, {7, -12, 12, -13},
    {2, -13, -2, 12}, {1, -7, 1, 6}, {-2, -10, -2, -4}, {-13, -13, -11, -8},
    {-3, 4, 2, -6}, {5, -1, -8, 7}, {6, -11, 3, -2}, {-9, 8, 4, -5},
    {11, -7, -6, 9}, {-4, 12, 8, -3}, {7, 2, -10, 6}, {-12, -5, 1, 8},
    {3, -8, 12, 4}, {-7, 11, 5, -9}, {9, -2, -4, 13}, {-1, 6, 7, -11},
    {8, -13, -3, 2}, {-10, 4, 11, -6}, {2, 9, -7, 5}, {-5, -12, 6, 3},
    {13, -1, -8, 10}, {-9, 7, 4, -2}, {1, -5, 12, 8}, {-6, 11, -3, 4},
    {10, -8, 2, 7}, {-4, -11, 9, 1}, {5, 6, -12, -7}, {8, 3, -1, 13},
    {-2, -9, 11, 5}, {7, -4, -6, 12}, {-13, 2, 3, -8}, {4, 10, -5, 1},
    {-7, -3, 8, 9}, {12, 6, -11, -2}, {1, -10, 5, 7}, {-8, 4, 13, -6},
    {3, 11, -4, 2}, {-1, -7, 9, 12}, {6, -13, -5, 8}, {10, 1, -9, 4},
    {-12, 7, 2, -3}, {5, -8, 11, 6}, {-2, 13, -7, -1}, {8, -4, 3, 10},
    {4, -11, -6, 2}, {-9, 5, 12, -8}, {1, 7, -3, 13}, {11, -2, 6, -10},
    {-5, 8, 9, 4}, {3, -12, -1, 7}, {-8, -6, 10, 5}, {13, 2, -4, 11},
    {-7, -9, 1, 6}, {12, 3, -11, -5}, {2, 8, 4, -13}, {-10, 7, -2, 9},
    {5, -1, 11, -8}, {-4, 13, 6, 3}, {8, -7, -12, 1}, {-3, 10, 2, -6},
    {9, 4, -5, 12}, {-11, -1, 7, 8}, {1, -9, 13, -4}, {6, 2, -8, 10},
    {-13, 5, 3, -7}, {11, -6, -2, 9}, {4, 8, 12, -11}, {-1, -10, 5, 13},
    {7, 3, -4, 6}, {-12, 9, 2, -8}, {10, -5, -7, 1}, {-3, 11, 8, -13},
    {5, -2, 4, 12}, {-9, 6, -10, 3}, {13, 1, 7, -4}, {-8, -11, 9, 2},
    {2, 5, -6, 10}, {-4, -13, 8, 7}, {11, 3, -1, -9}, {1, 12, -7, 4},
    {-10, -2, 6, 13}, {9, -8, 5, -11}, {-5, 2, 12, 8}, {7, -6, -13, 1},
    {3, 10, -4, -7}, {-12, 4, 11, 6}, {8, -1, -9, 2}, {-3, 13, 5, -10},
    {6, 7, -11, 9}, {-8, -5, 1, 4}, {12, 2, -6, 11}, {-7, 10, 3, -13},
    {9, -4, 8, 5}, {-2, -11, 13, 7}, {4, 1, -12, 6}, {-9, 8, 2, -3},
    {11, -7, -5, 10}, {-1, 3, 12, -8}, {5, 13, -10, 4}, {-6, -4, 7, 9},
    {2, 11, -3, 1}, {-13, 6, 8, -12}, {10, -9, 4, 3}, {-5, 7, -2, 13},
    {9, -6, 11, -1}, {-8, 12, 1, 5}, {3, -10, 6, 8}, {-4, 2, -7, 11},
    {13, 9, -11, 3}, {-1, -5, 7, 12}, {4, 8, -13, -2}, {-9, 6, 10, 1},
    {11, -3, 2, -8}, {-6, 13, 5, 7}, {8, -12, -4, 9}, {1, 4, 11, -10},
    {-7, -1, 6, 2}, {12, 5, -8, 13}, {-11, 9, 3, -4}, {2, -6, 10, 8},
    {-13, 7, -2, 5}, {4, 11, 9, -7}, {-5, -12, 1, 6}, {8, 3, 13, -9},
    {-10, 2, -6, 11}, {7, -4, 12, 1}, {-1, 8, 5, -13}, {9, -11, 4, 6},
    {-8, 10, -3, 2}, {13, -5, 7, -12}, {3, 1, -9, 8}, {-4, 6, 11, -7},
    {12, -10, 2, 5}, {-6, -1, 9, 4}, {1, 13, -11, 7}, {-2, -8, 6, 10},
    {5, 3, 8, -4}, {-13, 11, -7, 1}, {4, -12, 9, 2}, {-3, 7, -10, 13},
    {8, 6, 1, -11}, {-9, 4, 12, -5}, {10, -2, -6, 8}, {-7, 13, 3, 9},
    {2, -1, 11, -8}, {-4, 5, 7, 12}, {6, -13, -10, 1}, {-11, 9, 4, -3},
    {13, 2, -7, 6}, {-5, 8, 10, -12}, {1, -4, 3, 11}, {-8, 7, 9, -1},
    {12, -6, -2, 5}, {-10, 3, 13, 8}, {4, 11, -9, 2}, {-13, -7, 6, 10},
    {7, 1, -5, 9}, {-3, 12, 8, -11}, {5, -8, 2, 4}, {-1, 10, -12, 7},
    {9, 6, 11, -4}, {-6, -2, 1, 13}, {8, -10, 3, -7}, {-9, 5, 12, 2},
    {4, 13, -8, 6}, {-11, 1, 7, -5}, {10, -3, 9, 8}, {-4, -13, 2, 11},
    {3, 7, -1, 5}, {-12, 9, 6, -10}, {13, -6, 4, 1}, {-2, 8, -7, 12},
    {11, 5, -9, 3}, {-8, -4, 2, 7}, {1, 12, 6, -11}, {-13, 8, 10, -2},
    {4, -7, -1, 9}, {-5, 3, 13, 6}, {12, -10, 7, -8}, {-6, 2, 11, 4},
    {9, 1, -4, 5}, {-3, -12, 8, 10}, {7, 13, -11, -1}, {-9, 6, 2, 12},
    {5, -8, 4, 1}, {-2, 11, -6, 9}, {13, -5, 3, -10}, {-7, 8, 12, 6},
    {2, -3, 10, 7}, {-11, 4, -13, 5}, {8, 9, 1, -4}, {-1, -8, 11, 3},
    {6, 12, -7, 2}, {-10, -5, 9, 13}, {3, 1, 4, -12}, {-4, 7, -2, 8},
    {11, -6, 5, 10}, {-9, 13, -8, 1}, {2, 4, 12, -3}, {-13, -1, 6, 9},
    {7, 8, -11, 4}, {-5, 2, 10, -7}, {1, -9, 13, 6}, {-12, 5, 3, -8},
    {10, -4, -6, 11}, {-3, 9, 7, -1}, {8, 2, -10, 5}, {-7, -13, 12, 4},
    {6, 1, -5, 8}, {-11, 10, 3, -9}, {4, -2, 13, 7}, {-8, 6, -4, 12},
    {9, -7, 2, -13}, {-1, 11, 5, 3}, {12, -8, -11, 6}, {-4, 1, 10, 9},
    {7, -5, 8, -2}, {-10, 13, 4, -6}, {3, 9, -7, 11}, {-12, -3, 1, 5},
    {2, 6, -9, 8}, {-6, -11, 13, 10}, {11, 4, 7, -1}, {-5, -8, 2, 12},
    {1, 9, -12, 3}, {-13, 5, 6, -4}, {8, -10, 11, 7}, {-2, 4, -7, 1},
    {5, 12, -6, 9}, {-9, -1, 8, 13}, {10, 3, -11, 2}, {-4, 7, 1, -8},
    {12, -3, 4, 6}, {-7, 11, -10, 5}, {9, -12, 13, -1}, {-6, 2, 8, 10},
    {3, -5, 7, 4}, {-11, 8, -2, 9}, {1, 13, 6, -7}, {-8, -10, 11, 12},
    {4, 5, -13, 3}, {-1, -9, 10, 7}, {8, 6, 2, -11}, {-12, 13, -4, 1},
    {7, -6, 9, 5}, {-3, 4, -8, 12}, {11, 2, 1, -10}, {-5, -7, 13, 8},
    {6, 10, -1, 4}, {-13, 3, 7, -9}, {2, -11, 5, 12}, {-8, 9, -6, 1}
};

// ================================================================================
// ✅ 优化后的 detect_stage（使用 Line Buffer + Sliding Window）
// ================================================================================
// ================================================================================
// ✅ Plan B 优化：减少展开 + 简化判断
// ================================================================================
// ================================================================================
// ✅ 修复后的 detect_stage (使用循环队列 Line Buffer)
// ================================================================================
//static void detect_stage_optimized(
//    pixel_t image[IMG_HEIGHT][IMG_WIDTH],
//    ap_uint<8> threshold,
//    hls::stream<Detected> &out_stream,
//    const int cell_w,
//    const int cell_h
//) {
//    #pragma HLS INLINE off
//
//    // ✅ 使用循环队列 Line Buffer
//    static pixel_t line_buffer[LINE_BUFFER_ROWS][IMG_WIDTH];
//    #pragma HLS ARRAY_PARTITION variable=line_buffer dim=1 complete
//    #pragma HLS BIND_STORAGE variable=line_buffer type=ram_t2p impl=bram
//
//    // ✅ 循环指针(避免移动数据)
//    static ap_uint<3> line_ptr = 0;  // 0~6 循环
//
//    // Sliding Window（只需要读取,不需要移动）
//    pixel_t window[WINDOW_SIZE][WINDOW_SIZE];
//    #pragma HLS ARRAY_PARTITION variable=window dim=0 complete
//
//    const int x_min = 16;
//    const int x_max = IMG_WIDTH - 16;
//    const int y_min = 16;
//    const int y_max = IMG_HEIGHT - 16;
//
//    // ✅ 预加载前6行
//    INIT_LINES: for(int init_y = 0; init_y < LINE_BUFFER_ROWS - 1; init_y++) {
//        for(int init_x = 0; init_x < IMG_WIDTH; init_x++) {
//            #pragma HLS PIPELINE II=1
//            line_buffer[init_y][init_x] = image[init_y][init_x];
//        }
//    }
//
//    // 主循环
//    ROW_LOOP: for (int y = 0; y < IMG_HEIGHT; ++y) {
//
//        // ✅ 加载新行到循环队列
//        LOAD_NEW_LINE: for(int x = 0; x < IMG_WIDTH; x++) {
//            #pragma HLS PIPELINE II=1
//            line_buffer[line_ptr][x] = image[y][x];
//        }
//
//        // ✅ 处理当前行
//        COL_LOOP: for (int x = 0; x < IMG_WIDTH; ++x) {
//            #pragma HLS PIPELINE II=1
//            #pragma HLS DEPENDENCE variable=line_buffer inter false
////			#pragma HLS DEPENDENCE variable=line_buffer intra false  // 添加这行
//            // ✅ 从循环队列读取 window (无需移动数据)
//            UPDATE_WINDOW: for (int wy = 0; wy < WINDOW_SIZE; ++wy) {
//                #pragma HLS UNROLL
//
//                // 计算循环索引
//                ap_uint<3> line_idx = (line_ptr + wy + 1);  // (ptr+wy+1) % 8
//                if(line_idx >= LINE_BUFFER_ROWS) line_idx -= LINE_BUFFER_ROWS;
//
//                for (int wx = 0; wx < WINDOW_SIZE; ++wx) {
//                    #pragma HLS UNROLL
//                    int buffer_x = x + wx - (WINDOW_SIZE / 2);
//                    if(buffer_x >= 0 && buffer_x < IMG_WIDTH) {
//                        window[wy][wx] = line_buffer[line_idx][buffer_x];
//                    } else {
//                        window[wy][wx] = 0;
//                    }
//                }
//            }
//
//            // 跳过边界
//            if (y < y_min || y >= y_max || x < x_min || x >= x_max) {
//                continue;
//            }
//
//            // ===== FAST检测 (保持不变) =====
//            pixel_t center = window[3][3];
//            int upper = (int)center + (int)threshold;
//            int lower = (int)center - (int)threshold;
//
//            // ✅ 进一步简化：只用4个主要方向(粗筛)
//            bool quick_reject = true;
//            int samples[4];
//            samples[0] = (int)window[0][3];  // 上
//            samples[1] = (int)window[3][6];  // 右
//            samples[2] = (int)window[6][3];  // 下
//            samples[3] = (int)window[3][0];  // 左
//
//            ap_uint<3> bright = 0, dark = 0;
//            for(int i = 0; i < 4; i++) {
//                #pragma HLS UNROLL
//                if(samples[i] > upper) bright++;
//                if(samples[i] < lower) dark++;
//            }
//
//            // 如果4个方向都不满足,直接跳过
//            if(bright < 3 && dark < 3) continue;
//
//            // ✅ 精确检测(8个采样点)
//            int circle[8];
//            #pragma HLS ARRAY_PARTITION variable=circle complete
//
//            circle[0] = (int)window[0][3];
//            circle[1] = (int)window[1][5];
//            circle[2] = (int)window[3][6];
//            circle[3] = (int)window[5][5];
//            circle[4] = (int)window[6][3];
//            circle[5] = (int)window[5][1];
//            circle[6] = (int)window[3][0];
//            circle[7] = (int)window[1][1];
//
//            ap_uint<4> bright_cnt = 0, dark_cnt = 0;
//
//            // ✅ 完全展开(避免循环)
//            if(circle[0] > upper) bright_cnt++;
//            if(circle[0] < lower) dark_cnt++;
//            if(circle[1] > upper) bright_cnt++;
//            if(circle[1] < lower) dark_cnt++;
//            if(circle[2] > upper) bright_cnt++;
//            if(circle[2] < lower) dark_cnt++;
//            if(circle[3] > upper) bright_cnt++;
//            if(circle[3] < lower) dark_cnt++;
//            if(circle[4] > upper) bright_cnt++;
//            if(circle[4] < lower) dark_cnt++;
//            if(circle[5] > upper) bright_cnt++;
//            if(circle[5] < lower) dark_cnt++;
//            if(circle[6] > upper) bright_cnt++;
//            if(circle[6] < lower) dark_cnt++;
//            if(circle[7] > upper) bright_cnt++;
//            if(circle[7] < lower) dark_cnt++;
//
//            bool is_corner = (bright_cnt >= 5) || (dark_cnt >= 5);
//            if (!is_corner) continue;
//
//            // ===== 响应值计算 (简化) =====
//            ap_uint<12> score = 0;
//
//            // ✅ 只累加前4个差值(加速)
//            for(int i = 0; i < 4; i++) {
//                #pragma HLS UNROLL
//                int diff = circle[i] - (int)center;
//                int abs_diff = (diff >= 0) ? diff : -diff;
//                int val = abs_diff - (int)threshold;
//                if(val > 0) score += (ap_uint<12>)val;
//            }
//
//            // 输出
//            int actual_x = x;
////            int actual_y = y;
//            int actual_y = y-3;   // y - 3
//            // 统一安全边界 = 16，和 BRIEF 保持一致
//            const int SAFE_BORDER = 16;
//
//            if (actual_y < SAFE_BORDER || actual_y >= IMG_HEIGHT - SAFE_BORDER ||
//                actual_x < SAFE_BORDER || actual_x >= IMG_WIDTH  - SAFE_BORDER) {
//                continue;
//            }
//            int cell_x = actual_x / cell_w;
//            int cell_y = actual_y / cell_h;
//            if (cell_x >= GRID_COLS) cell_x = GRID_COLS - 1;
//            if (cell_y >= GRID_ROWS) cell_y = GRID_ROWS - 1;
//
//            Detected d;
//            d.response = score;
//            d.x = (ap_uint<10>)actual_x;
//            d.y = (ap_uint<10>)actual_y;
//            d.cell_idx = (ap_uint<16>)(cell_y * GRID_COLS + cell_x);
//            d.last = 0;
//            out_stream.write(d);
//        }
//
//        // ✅ 更新循环指针
//        line_ptr = (line_ptr + 1) & 0x7;  // % 8
//        if(line_ptr >= LINE_BUFFER_ROWS) line_ptr = 0;
//    }
//
//    // 结束标记
//    Detected term;
//    term.response = 0;
//    term.x = 0;
//    term.y = 0;
//    term.cell_idx = 0xFFFF;
//    term.last = 1;
//    out_stream.write(term);
//}

// ================================================================================
static void detect_stage_optimized(
    pixel_t image[IMG_HEIGHT][IMG_WIDTH],
    ap_uint<8> threshold,
    hls::stream<Detected> &out_stream,
    const int cell_w,
    const int cell_h
) {
#pragma HLS INLINE off

    // ✅ 使用循环队列 Line Buffer
    static pixel_t line_buffer[LINE_BUFFER_ROWS][IMG_WIDTH];
#pragma HLS ARRAY_PARTITION variable=line_buffer dim=1 complete
#pragma HLS BIND_STORAGE variable=line_buffer type=ram_t2p impl=bram

    // ✅ 循环指针(避免移动数据)
    static ap_uint<3> line_ptr = 0;  // 0~6 循环

    // Sliding Window（只需要读取,不需要移动）
    pixel_t window[WINDOW_SIZE][WINDOW_SIZE];
#pragma HLS ARRAY_PARTITION variable=window dim=0 complete

    const int SAFE_BORDER = 16;
    const int HALF_WIN    = WINDOW_SIZE / 2; // 7x7 -> 3

    // ✅ 预加载前6行（保留你原有逻辑）
    INIT_LINES: for(int init_y = 0; init_y < LINE_BUFFER_ROWS - 1; init_y++) {
        for(int init_x = 0; init_x < IMG_WIDTH; init_x++) {
#pragma HLS PIPELINE II=1
            line_buffer[init_y][init_x] = image[init_y][init_x];
        }
    }

    // 主循环
    ROW_LOOP: for (int y = 0; y < IMG_HEIGHT; ++y) {

        // ✅ 加载新行到循环队列
        LOAD_NEW_LINE: for(int x = 0; x < IMG_WIDTH; x++) {
#pragma HLS PIPELINE II=1
            line_buffer[line_ptr][x] = image[y][x];
        }

        // ✅ 处理当前行
        COL_LOOP: for (int x = 0; x < IMG_WIDTH; ++x) {
#pragma HLS PIPELINE II=1
#pragma HLS DEPENDENCE variable=line_buffer inter false
            // 1) 从循环队列读取 window
            UPDATE_WINDOW: for (int wy = 0; wy < WINDOW_SIZE; ++wy) {
#pragma HLS UNROLL
                // 计算循环索引
                ap_uint<3> line_idx = (line_ptr + wy + 1);
                if(line_idx >= LINE_BUFFER_ROWS) line_idx -= LINE_BUFFER_ROWS;

                for (int wx = 0; wx < WINDOW_SIZE; ++wx) {
#pragma HLS UNROLL
                    int buffer_x = x + wx - HALF_WIN;
                    if(buffer_x >= 0 && buffer_x < IMG_WIDTH) {
                        window[wy][wx] = line_buffer[line_idx][buffer_x];
                    } else {
                        window[wy][wx] = 0;
                    }
                }
            }

            // 2) 真实中心坐标 (window[3][3] 对应 (x, y-3))
            int actual_x = x;
            int actual_y = y - HALF_WIN;   // y - 3

            // 3) 安全边界判断（和 BRIEF 保持一致：离边界至少 16 像素）
            if (actual_x < SAFE_BORDER || actual_x >= IMG_WIDTH  - SAFE_BORDER ||
                actual_y < SAFE_BORDER || actual_y >= IMG_HEIGHT - SAFE_BORDER) {
                continue;
            }

            // ===================== FAST-16 开始 =====================

            pixel_t center = window[HALF_WIN][HALF_WIN];
            const int Ic   = (int)center;
            const int upper = Ic + (int)threshold;
            const int lower = Ic - (int)threshold;

            // 3.1 四点快速拒绝（上、右、下、左）
            int t_up  = 0;
            int t_low = 0;

            int p_up    = (int)window[0][HALF_WIN];             // (0,-3)
            int p_right = (int)window[HALF_WIN][WINDOW_SIZE-1]; // (3, 0)
            int p_down  = (int)window[WINDOW_SIZE-1][HALF_WIN]; // (0, 3)
            int p_left  = (int)window[HALF_WIN][0];             // (-3,0)

            t_up  += (p_up    > upper);  t_low += (p_up    < lower);
            t_up  += (p_right > upper);  t_low += (p_right < lower);
            t_up  += (p_down  > upper);  t_low += (p_down  < lower);
            t_up  += (p_left  > upper);  t_low += (p_left  < lower);

            if (t_up < 3 && t_low < 3)
                continue;

            // 3.2 16 点圆周（半径 3，落在 7x7 内）
            const int dx16[16] = {
                0,  1,  2,  3,  3,  3,  2,  1,
                0, -1, -2, -3, -3, -3, -2, -1
            };
            const int dy16[16] = {
               -3, -3, -2, -1,  0,  1,  2,  3,
                3,  3,  2,  1,  0, -1, -2, -3
            };

            int p16[16];
#pragma HLS ARRAY_PARTITION variable=p16 complete

        READ_CIRCLE: for (int i = 0; i < 16; ++i) {
#pragma HLS UNROLL
                int wy = HALF_WIN + dy16[i];
                int wx = HALF_WIN + dx16[i];
                p16[i] = (int)window[wy][wx];
            }

            // 3.3 亮/暗标记
            bool bright[16];
            bool dark[16];
#pragma HLS ARRAY_PARTITION variable=bright complete
#pragma HLS ARRAY_PARTITION variable=dark complete

        BRIGHT_DARK: for (int i = 0; i < 16; ++i) {
#pragma HLS UNROLL
                bright[i] = (p16[i] > upper);
                dark[i]   = (p16[i] < lower);
            }

            // 3.4 计算环形最大连续亮/暗 run，要求 >= 9
            int max_run_b = 0, cur_b = 0;
            int max_run_d = 0, cur_d = 0;

        RUNLEN: for (int i = 0; i < 32; ++i) {
#pragma HLS UNROLL
                bool vb = bright[i & 15];
                bool vd = dark[i & 15];

                cur_b = vb ? (cur_b + 1) : 0;
                if (cur_b > max_run_b) max_run_b = cur_b;

                cur_d = vd ? (cur_d + 1) : 0;
                if (cur_d > max_run_d) max_run_d = cur_d;
            }

            // 不满足连续 9 点亮/暗，不是角点
            if (!(max_run_b >= 9 || max_run_d >= 9))
                continue;

            // 3.5 计算完整 score（16 点全部参与）
            ap_uint<12> score = 0;

        SCORE16: for (int i = 0; i < 16; ++i) {
#pragma HLS UNROLL
                int diff = p16[i] - Ic;
                int ad   = (diff >= 0) ? diff : -diff;
                int val  = ad - (int)threshold;
                if (val > 0)
                    score += (ap_uint<12>)val;
            }

            // ===================== FAST-16 结束 =====================

            // 4) 映射到 cell（你原来的逻辑，基于 actual_x/actual_y）
            int cell_x = actual_x / cell_w;
            int cell_y = actual_y / cell_h;
            if (cell_x >= GRID_COLS) cell_x = GRID_COLS - 1;
            if (cell_y >= GRID_ROWS) cell_y = GRID_ROWS - 1;

            Detected d;
            d.response = score;
            d.x = (ap_uint<10>)actual_x;
            d.y = (ap_uint<10>)actual_y;
            d.cell_idx = (ap_uint<16>)(cell_y * GRID_COLS + cell_x);
            d.last = 0;
            out_stream.write(d);
        }

        // ✅ 更新循环指针
        line_ptr = (line_ptr + 1) & 0x7;  // % 8
        if(line_ptr >= LINE_BUFFER_ROWS) line_ptr = 0;
    }

    // 结束标记
    Detected term;
    term.response = 0;
    term.x = 0;
    term.y = 0;
    term.cell_idx = 0xFFFF;
    term.last = 1;
    out_stream.write(term);
}


// ================================================================================
// agg_stage（聚合阶段，按网格保留最佳关键点）
// ================================================================================
static void agg_stage(
    hls::stream<Detected> &in_stream,
    hls::stream<KpPacket> &out_kp_stream
) {
    #pragma HLS INLINE off

//    // 每个 cell 的关键点缓存
//    static KeyPoint cell_kps[CELL_COUNT][KEYPOINTS_PER_CELL];
//    #pragma HLS BIND_STORAGE variable=cell_kps type=ram_t2p impl=bram
//
//    static ap_uint<8> cell_counts[CELL_COUNT];
//    #pragma HLS BIND_STORAGE variable=cell_counts type=ram_s2p impl=lutram
    // ✅ 关键修改：使用 URAM（延迟更低）
    static KeyPoint cell_kps[CELL_COUNT][KEYPOINTS_PER_CELL];
    #pragma HLS BIND_STORAGE variable=cell_kps type=ram_t2p impl=uram
    #pragma HLS ARRAY_PARTITION variable=cell_kps dim=2 cyclic factor=4  // 部分分区

    static ap_uint<8> cell_counts[CELL_COUNT];
    #pragma HLS BIND_STORAGE variable=cell_counts type=ram_s2p impl=lutram

    // 初始化计数器
    INIT_LOOP: for (int c = 0; c < CELL_COUNT; ++c) {
        #pragma HLS PIPELINE II=1
        cell_counts[c] = 0;
    }

    // 处理检测到的角点
    AGGREGATE_LOOP: while (true) {
        Detected d = in_stream.read();
        if (d.last == 1) break;

        int idx = (int)d.cell_idx;
        if (idx < 0 || idx >= CELL_COUNT) continue;

        ap_uint<8> cnt = cell_counts[idx];

        if (cnt < (ap_uint<8>)KEYPOINTS_PER_CELL) {
            // 直接添加
            int pos = (int)cnt;
            cell_kps[idx][pos].x = (coord_t)d.x;
            cell_kps[idx][pos].y = (coord_t)d.y;
            cell_kps[idx][pos].response = d.response;
            cell_kps[idx][pos].octave = 0;
            cell_counts[idx] = cnt + 1;
        } else {
            // 替换最弱的关键点
            ap_uint<12> minv = cell_kps[idx][0].response;
            int minid = 0;

            FIND_MIN: for (int m = 1; m < KEYPOINTS_PER_CELL; ++m) {
                #pragma HLS PIPELINE II=1
                ap_uint<12> rv = cell_kps[idx][m].response;
                if (rv < minv) {
                    minv = rv;
                    minid = m;
                }
            }

            if (d.response > minv) {
                cell_kps[idx][minid].x = (coord_t)d.x;
                cell_kps[idx][minid].y = (coord_t)d.y;
                cell_kps[idx][minid].response = d.response;
                cell_kps[idx][minid].octave = 0;
            }
        }
    }

    // 输出所有保留的关键点
    OUTPUT_LOOP: for (int c = 0; c < CELL_COUNT; ++c) {
        ap_uint<8> cc = cell_counts[c];
        for (int k = 0; k < KEYPOINTS_PER_CELL; ++k) {
            #pragma HLS PIPELINE II=1
            if (k < cc) {
                KpPacket pk;
                pk.kp = cell_kps[c][k];
                pk.last = 0;
                out_kp_stream.write(pk);
            }
        }
    }

    // 发送结束标记
    KpPacket endp;
    endp.last = 1;
    out_kp_stream.write(endp);
}



// ================================================================================
// collect_stage（收集阶段，写回主存）
// ================================================================================
static void collect_stage(
    hls::stream<KpPacket> &in_kp_stream,
    KeyPoint keypoints[MAX_KEYPOINTS],
    ap_uint<16> *num_keypoints
) {
    #pragma HLS INLINE off

    ap_uint<16> out_cnt = 0;

    COLLECT_LOOP: while (true) {
        #pragma HLS PIPELINE II=1
        KpPacket p = in_kp_stream.read();
        if (p.last == 1) break;

        if (out_cnt < (ap_uint<16>)MAX_KEYPOINTS) {
            keypoints[(int)out_cnt] = p.kp;
            out_cnt++;
        }
    }

    // 写回关键点数量
    num_keypoints[0] = out_cnt;
}

// ================================================================================
// ✅ 优化后的 fast_corner_detection_streaming（调用新的 detect_stage）
// ================================================================================
void fast_corner_detection_streaming(
    pixel_t image[IMG_HEIGHT][IMG_WIDTH],
    KeyPoint keypoints[MAX_KEYPOINTS],
    ap_uint<16> *num_keypoints,
    ap_uint<8> threshold
) {
    #pragma HLS INLINE off

    const int cell_w = IMG_WIDTH / GRID_COLS;
    const int cell_h = IMG_HEIGHT / GRID_ROWS;

    // Stream 缓冲区
    hls::stream<Detected> detected_stream;
    #pragma HLS STREAM variable=detected_stream depth=128

    hls::stream<KpPacket> kp_stream;
    #pragma HLS STREAM variable=kp_stream depth=128

    // DATAFLOW：3阶段 pipeline
    #pragma HLS DATAFLOW

    // 使用优化后的检测函数
    detect_stage_optimized(image, threshold, detected_stream, cell_w, cell_h);
    agg_stage(detected_stream, kp_stream);
    collect_stage(kp_stream, keypoints, num_keypoints);
}

// ================================================================================
// ✅ 优化后的 brief_descriptor_extraction_streaming
// ================================================================================
void brief_descriptor_extraction_streaming(
    pixel_t image[IMG_HEIGHT][IMG_WIDTH],
    KeyPoint keypoints[MAX_KEYPOINTS],
    descriptor_t descriptors[MAX_KEYPOINTS],
    ap_uint<16> num_keypoints
) {
    #pragma HLS INLINE OFF

    KEYPOINT_LOOP: for(ap_uint<16> k = 0; k < num_keypoints; k++) {
        #pragma HLS LOOP_TRIPCOUNT min=100 max=700

        coord_t kp_x = keypoints[k].x;
        coord_t kp_y = keypoints[k].y;
        descriptor_t descriptor = 0;

        // ✅ 优化1: 预计算边界（减少关键路径）
        bool kp_in_safe_region = (kp_x >= 16 && kp_x < IMG_WIDTH-16 &&
                                  kp_y >= 16 && kp_y < IMG_HEIGHT-16);

        BRIEF_BITS: for(int b = 0; b < BRIEF_LENGTH; b++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS DEPENDENCE variable=image inter false

            // ✅ 优化2: 分级寄存器插入
            #pragma HLS LATENCY min=1 max=2

            ap_int<6> x1 = brief_pattern[b].x1;
            ap_int<6> y1 = brief_pattern[b].y1;
            ap_int<6> x2 = brief_pattern[b].x2;
            ap_int<6> y2 = brief_pattern[b].y2;

            // ✅ 优化3: 简化边界检查（大多数情况跳过）
            int p1_x = kp_x + x1;
            int p1_y = kp_y + y1;
            int p2_x = kp_x + x2;
            int p2_y = kp_y + y2;

            pixel_t pixel1, pixel2;
            bool valid;

            if(kp_in_safe_region) {
                // ✅ 快速路径：不检查边界
                pixel1 = image[p1_y][p1_x];
                pixel2 = image[p2_y][p2_x];
                valid = true;
            } else {
                // 慢速路径：详细边界检查
                valid = (p1_x >= 0 && p1_x < IMG_WIDTH &&
                        p1_y >= 0 && p1_y < IMG_HEIGHT &&
                        p2_x >= 0 && p2_x < IMG_WIDTH &&
                        p2_y >= 0 && p2_y < IMG_HEIGHT);

                if(valid) {
                    pixel1 = image[p1_y][p1_x];
                    pixel2 = image[p2_y][p2_x];
                }
            }

            // ✅ 优化4: 使用三元运算简化逻辑
            descriptor[b] = (valid && (pixel1 < pixel2)) ? 1 : 0;
        }

        descriptors[k] = descriptor;
    }
}


// ================================================================================
// 顶层函数：orb_feature_extraction
// ================================================================================
void orb_feature_extraction(
    pixel_t image[IMG_HEIGHT][IMG_WIDTH],
    KeyPoint keypoints[MAX_KEYPOINTS],
    descriptor_t descriptors[MAX_KEYPOINTS],
    ap_uint<16> *num_keypoints,
    ap_uint<8> threshold
) {
    // ✅ AXI 接口配置
    #pragma HLS INTERFACE m_axi port=image offset=slave bundle=gmem0 depth=307200 \
                          max_read_burst_length=256 num_read_outstanding=16
    #pragma HLS INTERFACE m_axi port=keypoints offset=slave bundle=gmem1 depth=700 \
                          max_write_burst_length=64 num_write_outstanding=16
    #pragma HLS INTERFACE m_axi port=descriptors offset=slave bundle=gmem2 depth=700 \
                          max_write_burst_length=64 num_write_outstanding=16
    #pragma HLS INTERFACE m_axi port=num_keypoints offset=slave bundle=gmem1 depth=1

    #pragma HLS INTERFACE s_axilite port=image bundle=control
    #pragma HLS INTERFACE s_axilite port=keypoints bundle=control
    #pragma HLS INTERFACE s_axilite port=descriptors bundle=control
    #pragma HLS INTERFACE s_axilite port=num_keypoints bundle=control
    #pragma HLS INTERFACE s_axilite port=threshold bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    // 局部缓存（减少主存访问）
    static KeyPoint local_keypoints[MAX_KEYPOINTS];
    #pragma HLS BIND_STORAGE variable=local_keypoints type=ram_s2p impl=uram

    static descriptor_t local_descriptors[MAX_KEYPOINTS];
    #pragma HLS BIND_STORAGE variable=local_descriptors type=ram_s2p impl=uram

    ap_uint<16> detected_keypoints = 0;

    // ✅ FAST 检测（3阶段 pipeline）
    fast_corner_detection_streaming(image, local_keypoints, &detected_keypoints, threshold);

//    // ✅ BRIEF 描述子提取
//    brief_descriptor_extraction_streaming(image, local_keypoints, local_descriptors, detected_keypoints);

    // ✅ 写回主存
    WRITE_RESULTS: for(ap_uint<16> i = 0; i < detected_keypoints; i++) {
        #pragma HLS PIPELINE II=1
        keypoints[i] = local_keypoints[i];
//        descriptors[i] = local_descriptors[i];
        descriptors[i] = 0;
    }

    // ✅ 写回关键点数量
    num_keypoints[0] = detected_keypoints;
}








