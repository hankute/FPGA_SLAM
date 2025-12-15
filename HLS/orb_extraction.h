#ifndef ORB_EXTRACTION_H
#define ORB_EXTRACTION_H

#include "ap_int.h"
#include "hls_stream.h"
#include "ap_fixed.h"
#include <stdint.h>

// ================================================================================
// 图像和算法参数配置
// ================================================================================
#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define MAX_KEYPOINTS 2000

// Line Buffer 参数（FAST需要±3行邻域）
#define LINE_BUFFER_ROWS 7
#define WINDOW_SIZE 7

// Grid 参数（用于空间均衡分布）
#define GRID_COLS 12
#define GRID_ROWS 10
#define CELL_COUNT (GRID_COLS * GRID_ROWS)  // 48 cells
#define KEYPOINTS_PER_CELL 17  // ~16 per cell

// BRIEF 描述子参数
#define BRIEF_LENGTH 256

// ================================================================================
// 类型定义
// ================================================================================
typedef ap_uint<8>   pixel_t;       // 8-bit grayscale pixel
typedef ap_int<16>   coord_t;       // 16-bit coordinate
typedef ap_uint<256> descriptor_t;  // 256-bit BRIEF descriptor (32 bytes)

// ================================================================================
// KeyPoint 结构体（8字节对齐，便于AXI传输）
// ================================================================================
struct KeyPoint {
    ap_int<16>  x;          // 2 bytes - x coordinate
    ap_int<16>  y;          // 2 bytes - y coordinate
    ap_uint<16> response;   // 2 bytes - corner response (score)
    ap_uint<8>  octave;     // 1 byte  - pyramid level
    ap_uint<8>  reserved;   // 1 byte  - padding (total = 8 bytes)
};

// ================================================================================
// BRIEF Pattern 结构体（测试点对）
// ================================================================================
struct BriefPattern {
    ap_int<6> x1, y1;  // 第一个测试点相对坐标
    ap_int<6> x2, y2;  // 第二个测试点相对坐标
};

// ================================================================================
// Stream 传输数据结构
// ================================================================================
// 检测阶段输出的中间结果
struct Detected {
    ap_uint<12> response;   // 角点响应值
    ap_uint<10> x;          // x坐标
    ap_uint<10> y;          // y坐标
    ap_uint<16> cell_idx;   // 所属网格索引
    ap_uint<1>  last;       // 结束标志
};

// 聚合阶段输出的关键点包
struct KpPacket {
    KeyPoint   kp;     // 关键点数据
    ap_uint<1> last;   // 结束标志
};

// ================================================================================
// 主要函数声明
// ================================================================================

// 顶层函数：ORB 特征提取（FAST + BRIEF）
void orb_feature_extraction(
    pixel_t image[IMG_HEIGHT][IMG_WIDTH],          // 输入图像
    KeyPoint keypoints[MAX_KEYPOINTS],             // 输出关键点数组
    descriptor_t descriptors[MAX_KEYPOINTS],       // 输出描述子数组
    ap_uint<16> *num_keypoints,                    // 输出关键点数量（指针）
    ap_uint<8> threshold = 10                      // FAST阈值
);

// FAST 角点检测（流式处理，3阶段pipeline）
void fast_corner_detection_streaming(
    pixel_t image[IMG_HEIGHT][IMG_WIDTH],
    KeyPoint keypoints[MAX_KEYPOINTS],
    ap_uint<16> *num_keypoints,
    ap_uint<8> threshold = 10
);

// BRIEF 描述子提取（流式处理）
void brief_descriptor_extraction_streaming(
    pixel_t image[IMG_HEIGHT][IMG_WIDTH],
    KeyPoint keypoints[MAX_KEYPOINTS],
    descriptor_t descriptors[MAX_KEYPOINTS],
    ap_uint<16> num_keypoints
);

#endif // ORB_EXTRACTION_H
